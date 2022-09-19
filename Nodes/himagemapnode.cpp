#include "himagemapnode.h"

#include <QDebug>
#include <QPainter>

#include "hfillnode.h"
#define DEBUG qDebug() << __FUNCTION__ << __LINE__

HImageMapNodeDelegate::HImageMapNodeDelegate(const QRectF &rect)
    : _fill_node(new HFillNode(rect)) {}

QSGNode *HImageMapNodeDelegate::build(HBoard *b) {
  return _fill_node->build(b);
}

QSGNode *HImageMapNodeDelegate::get() { return _fill_node->get(); }

QRectF HImageMapNodeDelegate::getBoundRect() {
  return _fill_node->getBoundRect();
}

QList<QPointF> HImageMapNodeDelegate::getPointList() {
  return _fill_node->getPointList();
}

void HImageMapNodeDelegate::changedSelectStatus() {
  _fill_node->changedSelectStatus();
}

bool HImageMapNodeDelegate::isSelect() { return _fill_node->isSelect(); }

void HImageMapNodeDelegate::move(const QPointF &p) { _fill_node->move(p); }

void HImageMapNodeDelegate::moveTo(const QPointF &p) { _fill_node->moveTo(p); }

void HImageMapNodeDelegate::drawPoints(const QList<QPointF> &pts) {
  _fill_node->drawPoints(pts);
}

void HImageMapNodeDelegate::updateDrawMode(unsigned long mode) {
  _fill_node->updateDrawMode(mode);
}

void HImageMapNodeDelegate::setVisible(bool flag) {
  _fill_node->setVisible(flag);
}

bool HImageMapNodeDelegate::visible() { return _fill_node->visible(); }

void HImageMapNodeDelegate::timeOut() { _fill_node->timeOut(); }

int HImageMapNodeDelegate::setText(const QString &text, const QRectF &position,
                                   int pixel_size) {
  return _fill_node->setText(text, position, pixel_size);
}

QString HImageMapNodeDelegate::getText() { return _fill_node->getText(); }

bool HImageMapNodeDelegate::enableHome() { return _fill_node->enableHome(); }

void HImageMapNodeDelegate::setEnableHome(bool f) {
  _fill_node->setEnableHome(f);
}

void HImageMapNodeDelegate::setDestory(bool flag) {
  _fill_node->setDestory(flag);
}

void HImageMapNodeDelegate::setFlag(HNodeBase::NODEFLAG flag, bool open) {
  _fill_node->setFlag(flag, open);
}

HNodeBase::NODETYPE HImageMapNodeDelegate::nodeType() {
  return NODETYPE::MAPINAGE;
}

void HImageMapNodeDelegate::setRect(const QRectF &rect) {
  _fill_node->setRect(rect);
}

int HImageMapNodeDelegate::save(QJsonObject &) { return -1; }

int HImageMapNodeDelegate::load(const QJsonObject &) { return -1; }

int HImageMapNodeDelegate::save(const QString &) { return -1; }

int HImageMapNodeDelegate::load(const QString &) { return -1; }

HImageMapNode::HImageMapNode() : HImageMapNodeDelegate() {}

HImageMapNode::HImageMapNode(const QString &path, const QPointF &start_point)
    : HImageMapNodeDelegate(), _image(path) {
  setRect(QRectF(start_point, _image.size()));
}

HImageMapNode::HImageMapNode(const QImage &mat, const QPointF &start_point)
    : _image(mat) {
  setRect(QRectF(start_point, _image.size()));
}

HImageMapNode::HImageMapNode(const HImageMapNode &node) {
  setRect(node._fill_node->getBoundRect());
  _image = node._image;
}

HImageMapNode &HImageMapNode::operator=(const HImageMapNode &node) {
  setRect(node._fill_node->getBoundRect());
  _image = node._image;
  return *this;
}

HImageMapNode::~HImageMapNode() {}

void HImageMapNode::updateMat(HBoard *, const QImage &mat,
                              const QPointF &start) {
  QPainter p(&_image);
  auto roi = QRectF(start, mat.size());
  p.drawImage(roi, mat);
}

QImage HImageMapNode::getImage() { return _image; }

void HImageMapNode::setImage(const QImage &i) { _image = i; }

QImage HImageMapNode::getImage(const QRectF &roi, double scale) {
  auto s = QRectF(0, 0, roi.width() * scale, roi.height() * scale);
  QImage out(s.toRect().size(), _image.format());
  QPainter painter(&out);
  painter.drawImage(s, _image, roi);
  return out;
}
