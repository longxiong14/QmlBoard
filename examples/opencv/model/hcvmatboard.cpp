#include "hcvmatboard.h"

#include <QDebug>

#include "../../Nodes/hnodebase.h"
#include "hcvmatnode2.h"

HCVMatBoard::HCVMatBoard(QQuickItem *parent) : HImageMapBoard(parent) {}

int HCVMatBoard::setMatNode(std::shared_ptr<HCVMatNode2> node) {
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
  auto node = std::make_shared<HCVMatNode2>(mat);
  return setMatNode(node);
}

cv::Mat HCVMatBoard::getMatNode() {
  cv::Mat out;
  if (_mat_node) out = _mat_node->getMat();
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

void HCVMatBoard::setMatMsg(cv::Mat mat) {
  QString s = QString("width:%1\nheight:%2\nchannels:%3\n")
                  .arg(mat.cols)
                  .arg(mat.rows)
                  .arg(mat.channels());
  setMsg(s);
}
