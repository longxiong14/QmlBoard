#include "himagenode.h"

#include <QDebug>
#include <QImage>
#include <QQuickWindow>
#include <QSGNode>
#include <QSGSimpleTextureNode>

#include "../Common/hcommons.h"
#include "hboard.h"
#define DEBUG qDebug() << __FUNCTION__ << " " << __LINE__ << " "

HImageNode::HImageNode(const QString &path, const QRect &rect)
    : HNodeBase(), _path(path) {}

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

QRect HImageNode::getBoundRect() {
  auto r = rect();
  return QRect(static_cast<int>(r.x()), static_cast<int>(r.y()),
               static_cast<int>(r.width()), static_cast<int>(r.height()));
}

QList<QPoint> HImageNode::getPointList() {
  auto r = getBoundRect();
  return HCommon::BuildRectList(r.topLeft(), r.bottomRight());
}

void HImageNode::changedSelectStatus() { _select = !_select; }

void HImageNode::move(const QPoint &p) {
  auto r = rect();
  auto tl = r.topLeft();
  tl += p;
  QRectF s = QRectF();
  s.setTopLeft(tl);
  s.setSize(r.size());
  setRect(s);
}
