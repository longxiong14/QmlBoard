#include "himagenode.h"

#include <QDebug>
#include <QImage>
#include <QQuickWindow>
#include <QSGNode>
#include <QSGSimpleTextureNode>

#include "../Common/hcommons.h"
#include "hboard.h"
#define DEBUG qDebug() << __FUNCTION__ << " " << __LINE__ << " "

HImageNode::HImageNode() {}

HImageNode::HImageNode(const QString &path, const QRectF &rect)
    : HNodeBase(), _path(path), _rect(rect) {}

QSGNode *HImageNode::build(HBoard *board) {
  if (board) {
    QImage image;
    image.load(_path);
    if (image.isNull()) {
      DEBUG << "image is null";
      return nullptr;
    }
    QSGTexture *texture = board->window()->createTextureFromImage(image);
    setTexture(texture);
    if (_rect.size().isNull()) {
      _rect.setSize(image.size());
    }
    setRect(_rect);
  }
  return this;
}

QSGNode *HImageNode::get() { return this; }

QRectF HImageNode::getBoundRect() {
  auto r = rect();
  return QRectF((r.x()), (r.y()), (r.width()), (r.height()));
}

QList<QPointF> HImageNode::getPointList() {
  auto r = getBoundRect();
  return HCommon::BuildRectList(r.topLeft(), r.bottomRight());
}

void HImageNode::move(const QPointF &p) {
  auto r = rect();
  auto tl = r.topLeft();
  tl += p;
  _rect = QRectF();
  _rect.setTopLeft(tl);
  _rect.setSize(r.size());
  setRect(_rect);

  HNodeBase::move(p);
}

HNodeBase::NODETYPE HImageNode::nodeType() { return RECTANGLE; }

void HImageNode::setPath(const QString &path) { _path = path; }

QString HImageNode::getPath() { return _path; }
