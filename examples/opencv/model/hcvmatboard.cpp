#include "hcvmatboard.h"

#include <QDebug>

#include "../../Nodes/hnodebase.h"
#include "hcvmatnode.h"
#define DEBUG qDebug() << __FUNCTION__ << __LINE__

HCVMatBoard::HCVMatBoard(QQuickItem *parent) : HImageMapBoard(parent) {
  init();
  connections();
}

void HCVMatBoard::init() { setMask(false); }

int HCVMatBoard::setMatNode(std::shared_ptr<HCVMatNode> node) {
  if (!node) return -1;
  removeMatNode();
  node->setFlag(HNodeBase::NODEFLAG::CANSELECT, false);
  setMatMsg(node->getMat());
  pushNode(node);
  //  home();
  _mat_node = node;
  return 0;
}

int HCVMatBoard::setMatNode(cv::Mat mat) {
  if (mat.empty()) return -1;
  auto node = std::make_shared<HCVMatNode>(mat);
  return setMatNode(node);
}

cv::Mat HCVMatBoard::getMatNode() {
  cv::Mat out;
  if (_mat_node) out = _mat_node->getMat();
  return out;
}

cv::Mat HCVMatBoard::getMaskNode() {
  cv::Mat out;
  if (_mask_node) out = _mask_node->getMat();
  return out;
}

void HCVMatBoard::removeMatNode() {
  if (_mat_node) {
    removeNode(_mat_node->id());
    _mat_node = nullptr;
  }
}

void HCVMatBoard::setMsg(const QString &msg) {
  if (_msg != msg) {
    _msg = msg;
    msgChanged();
  }
}

QString HCVMatBoard::msg() { return _msg; }

void HCVMatBoard::setMask(bool checked) {
  if (_mask != checked) {
    _mask = checked;
    maskChanged();
  }
}

bool HCVMatBoard::mask() { return _mask; }

void HCVMatBoard::setHistogram(const QJsonArray &array) {
  _histogram = array;
  histogramChanged();
}

QJsonArray HCVMatBoard::histogram() { return _histogram; }

void HCVMatBoard::setMatMsg(cv::Mat mat) {
  QString s = QString("width:%1\nheight:%2\nchannels:%3\n")
                  .arg(mat.cols)
                  .arg(mat.rows)
                  .arg(mat.channels());
  setMsg(s);
}

void HCVMatBoard::connections() {
  connect(this, &HCVMatBoard::maskChanged, this, &HCVMatBoard::slotMask);
}

void HCVMatBoard::slotMask() {
  if (_mask_node) {
    removeNode(_mask_node->id());
    _mask_node = nullptr;
  }
  if (mask()) {
    if (_mat_node) {
      cv::Mat m =
          cv::Mat(_mat_node->getMat().size(), CV_8UC4, cv::Scalar::all(0));
      DEBUG << "mat size : " << m.cols << m.rows;
      _mask_node = std::make_shared<HCVMatNode>(m);
      pushNode(_mask_node);
    }
  } else {
    if (_mask_node) {
      removeNode(_mask_node->id());
      _mask_node = nullptr;
    }
  }
}
