#include "huicontrol.h"
//#include <opencv2/
#include <QDebug>
#include <memory>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#include "../../Handles/hhandleflyweight.h"
#include "../../hboardmanager.h"
#include "../hqdebug.h"
#include "../model/hcvmatboard.h"
#include "../model/hcvmatnode.h"
#include "../model/hcvmatnode_rotate.h"
#include "../model/hmulthandles.h"
#include "../model/htrans.h"
#include "../model/huimodel.h"
#define DEBUG qDebug() << __FUNCTION__ << __LINE__

#define SIGNLE                                     \
  auto board = getSingleSource();                  \
  if (!board) {                                    \
    sigError("please select single source board"); \
    return -1;                                     \
  }                                                \
  auto dest = getDest();                           \
  if (!dest) {                                     \
    sigError("please select dest");                \
    return -1;                                     \
  }

#define qsTr(str) _translator->trans(str)

HUIControl::HUIControl(QObject *parent)
    : QObject(parent), _model(nullptr), _translator(nullptr) {
  _translator = std::shared_ptr<HTranslator>(new HTranslator());
  HHandleFlyWeight::registHandle<HMultHandles>();
}

void HUIControl::setModel(HUIModel *ptr) { _model = ptr; }

HTranslator *HUIControl::getTranslate() {
  if (_translator) return _translator.get();
  return nullptr;
}

int HUIControl::openImage(const QString &path) {
  DEBUG << path;
  if (!_model) {
    sigError(qsTr("_model is nullptr"));
    return -1;
  }
  auto board = getSingleSource();
  if (!board) {
    sigError(qsTr("please select single source board"));
    return -1;
  }
  auto node =
      cv::imread(path.toLocal8Bit().toStdString(), cv::IMREAD_UNCHANGED);
  return board->setMatNode(node);
}

int HUIControl::blur(const QJsonObject &size, const QJsonObject &point) {
  SIGNLE
  DEBUG << size << point;
  auto mat = board->getMatNode();
  cv::Mat dst;
  cv::blur(mat, dst, getSize(size), getPoint(point),
           cv::BorderTypes::BORDER_DEFAULT);
  return dest->setMatNode(dst);
}

int HUIControl::translate(const QJsonObject &dlt_point) {
  SIGNLE

  auto mat = board->getMatNode();
  cv::Point dlt = getPoint(dlt_point);
  cv::Mat dst;
  //  cv::Mat m = (cv::Mat_<double>(3, 3) << 1, 0, dlt.x, 0, 1, dlt.y, 0, 0, 1);
  cv::Mat m = (cv::Mat_<double>(2, 3) << 1, 0, dlt.x, 0, 1, dlt.y);
  DEBUG << m;
  cv::warpAffine(mat, dst, m, mat.size());
  return dest->setMatNode(dst);
}

int HUIControl::maskBoard(const QString &name, bool checked) {
  auto board = getBoardByName(name);
  if (board->getMatNode().empty()) {
    sigError(qsTr("unload image..."));
  }
  return 0;
}

int HUIControl::openInrange(const QString &name, const QString &file) {
  auto board = HBoardManager::getInstance()->getBoard(name);
  if (!board) return -1;
  auto b = dynamic_cast<HCVMatBoard *>(board);
  if (!b) return -1;
  cv::Mat mat =
      cv::imread(file.toLocal8Bit().toStdString(), cv::IMREAD_UNCHANGED);
  if (mat.empty()) return -1;
  b->setMatNode(mat);
  return 0;
}

int HUIControl::test() {
  //  auto board = getSingleSource();
  //  if (!board) {
  //    sigError("please select single source board");
  //    return -1;
  //  }
  //  auto node = std::make_shared<HCVMatRotateNode>(
  //      "C:\\Users\\xiaolong\\Pictures\\bgr.png");
  //  board->pushNode(node);

  cv::RNG rng;
  cv::Mat mask = cv::Mat::zeros(256, 256, CV_8UC1);
  for (int r = 0; r < 256; r++) {
    auto ptr = mask.ptr<uchar>(r);
    for (int c = 0; c < 256; c++) {
      ptr[c] = uchar(rng.uniform(0, 255));
    }
  }
  cv::imshow("mask", mask);

  auto dest = (mask == 3);
  cv::imshow("dest", dest);

  cv::waitKey();
  return 0;
}

HCVMatBoard *HUIControl::getBoardByName(const QString &name) {
  auto *board = HBoardManager::getInstance()->getBoard(name);
  if (!board) return nullptr;
  auto b = dynamic_cast<HCVMatBoard *>(board);
  return b;
}

HCVMatBoard *HUIControl::getSingleSource() {
  if (!_model) return nullptr;
  auto source = _model->sourceChecked();
  int count = 0;
  QString name;
  for (const auto &k : source.keys()) {
    if (source.value(k).toBool()) {
      name = k;
      count++;
    }
  }
  if (count > 1) return nullptr;
  return getBoardByName(name);
}

HCVMatBoard *HUIControl::getDest() {
  if (!_model) return nullptr;
  auto source = _model->dest();
  auto *board = HBoardManager::getInstance()->getBoard(source);
  if (!board) return nullptr;
  auto b = dynamic_cast<HCVMatBoard *>(board);
  return b;
}

cv::Size HUIControl::getSize(const QJsonObject &param) {
  return cv::Size(param.value("size_width").toInt(),
                  param.value("size_height").toInt());
}

cv::Point HUIControl::getPoint(const QJsonObject &param) {
  return cv::Size(param.value("point_x").toInt(),
                  param.value("point_y").toInt());
}
