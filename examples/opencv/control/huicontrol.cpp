#include "huicontrol.h"

#include <QDebug>
#include <memory>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#include "../../Handles/hhandleflyweight.h"
#include "../../hboardmanager.h"
#include "../hqdebug.h"
#include "../model/hcvmatboard.h"
#include "../model/hcvmatnode2.h"
#include "../model/hmulthandles.h"
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

HUIControl::HUIControl(QObject *parent) : QObject(parent) {
  HHandleFlyWeight::registHandle<HMultHandles>();
}

void HUIControl::setModel(HUIModel *ptr) { _model = ptr; }

int HUIControl::openImage(const QString &path) {
  DEBUG << path;
  if (!_model) {
    sigError("_model is nullptr");
    return -1;
  }
  auto board = getSingleSource();
  if (!board) {
    sigError("please select single source board");
    return -1;
  }
  auto node =
      cv::imread(path.toLocal8Bit().toStdString(), cv::IMREAD_UNCHANGED);
  return board->setMatNode(node);
}

int HUIControl::blur(const QJsonObject &size, const QJsonObject &point) {
  SIGNLE

  auto mat = board->getMatNode();
  cv::Mat dst;
  cv::blur(mat, dst, getSize(size), getPoint(point),
           cv::BorderTypes::BORDER_DEFAULT);
  return dest->setMatNode(dst);
}

int HUIControl::test() {
  //    cv::blur()

  return 0;
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
  auto *board = HBoardManager::getInstance()->getBoard(name);
  if (!board) return nullptr;
  auto b = dynamic_cast<HCVMatBoard *>(board);
  return b;
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
