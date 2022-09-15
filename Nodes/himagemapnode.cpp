#include "himagemapnode.h"

#include <QDebug>

#include "hfillnode.h"
#define DEBUG qDebug() << __FUNCTION__ << __LINE__
HImageMapNode::HImageMapNode(const QString &path, const QPointF &start_point)
    : _image(path), _scale(0) {
  DEBUG << _image.size();
  _fill_node = std::make_shared<HFillNode>(QRectF(start_point, _image.size()));
}

HImageMapNode::HImageMapNode(const QImage &mat, const QPointF &start_point)
    : _image(mat), _scale(0) {
  _fill_node = std::make_shared<HFillNode>(QRectF(start_point, _image.size()));
}

HImageMapNode::~HImageMapNode() {}

QSGNode *HImageMapNode::build(HBoard *b) { return _fill_node->build(b); }

QSGNode *HImageMapNode::get() { return _fill_node->get(); }

QRectF HImageMapNode::getBoundRect() { return _fill_node->getBoundRect(); }

QList<QPointF> HImageMapNode::getPointList() {
  return _fill_node->getPointList();
}

void HImageMapNode::changedSelectStatus() {
  return _fill_node->changedSelectStatus();
}

bool HImageMapNode::isSelect() { return _fill_node->isSelect(); }

void HImageMapNode::move(const QPointF &p) { _fill_node->move(p); }

void HImageMapNode::moveTo(const QPointF &p) { _fill_node->moveTo(p); }

void HImageMapNode::drawPoints(const QList<QPointF> &pts) {
  _fill_node->drawPoints(pts);
}

HNodeBase::NODETYPE HImageMapNode::nodeType() { return NODETYPE::MAPINAGE; }

void HImageMapNode::updateDrawMode(unsigned long mode) {
  _fill_node->updateDrawMode(mode);
}

void HImageMapNode::setVisible(bool flag) { _fill_node->setVisible(flag); }

bool HImageMapNode::visible() { return _fill_node->visible(); }

void HImageMapNode::timeOut() { _fill_node->timeOut(); }

int HImageMapNode::setText(const QString &text, const QRectF &position,
                           int pixel_size) {
  return _fill_node->setText(text, position, pixel_size);
}

QString HImageMapNode::getText() { return _fill_node->getText(); }

bool HImageMapNode::enableHome() { return _fill_node->enableHome(); }

void HImageMapNode::setEnableHome(bool f) {
  return _fill_node->setEnableHome(f);
}

void HImageMapNode::setDestory(bool flag) { _fill_node->setDestory(flag); }

void HImageMapNode::setFlag(HNodeBase::NODEFLAG flag, bool open) {
  return _fill_node->setFlag(flag, open);
}

int HImageMapNode::load(const QString &path) { return -1; }

int HImageMapNode::save(const QString &path) { return -1; }

QImage HImageMapNode::getImage() { return _image; }

QImage HImageMapNode::getScaleImage(double scale) {
  if (std::fabs(scale - _scale) > 1E-10) {
    _scale = scale;
    _scale_image = _image.scaled(int(_image.width() * scale),
                                 int(_image.height() * scale));
  }
  return _scale_image;
}
