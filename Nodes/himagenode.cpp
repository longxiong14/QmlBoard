#include "himagenode.h"

#include <QDebug>
#include <QImage>
#include <QQuickWindow>
#include <QSGNode>
#include <QSGSimpleTextureNode>

#include "../Common/hcommons.h"
#include "hboard.h"
#define DEBUG qDebug() << __FUNCTION__ << " " << __LINE__ << " "

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

QRect HImageNode::getBoundRect() { return _rect.toRect(); }

QList<QPoint> HImageNode::getPointList() {
  auto r = getBoundRect();
  return HCommon::BuildRectList(r.topLeft(), r.bottomRight());
}

void HImageNode::changedSelectStatus() {
  _select = !_select;
  DEBUG << _select;
}

void HImageNode::move(const QPoint &p) {
  auto r = rect();
  auto tl = r.topLeft();
  tl += p;
  _rect = QRectF();
  _rect.setTopLeft(tl);
  _rect.setSize(r.size());
  setRect(_rect);
}

HNodeBase::SELECTTYPE HImageNode::selectType() { return INAREA; }

void HImageNode::setVisible(bool flag) {
  DEBUG << flag;
  if (flag) {
    setRect(0, 0, 0, 0);
  } else {
    if (_select) changedSelectStatus();
    setRect(_rect);
  }
}

bool HImageNode::visible() {
  auto r = rect().toRect();
  auto s = _rect.toRect();
  return r.x() == s.x() && r.y() == s.y() && r.width() == s.width() &&
         r.height() == s.height();
}
