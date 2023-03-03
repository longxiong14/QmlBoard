#include "hdragnode.h"

#include <QDebug>
#include <QSize>

#include "../Common/hcommons.h"
#include "../Common/hplanvector.h"
#include "../Common/hsgnodecommon.h"
#define DEBUG qDebug() << __FUNCTION__ << __LINE__
HDragNode::HDragNode()
    : QSGGeometryNode(), _cursor(Qt::CursorShape::ArrowCursor) {
  setFollowIndex(-1);
}

HDragNode *HDragNode::buildNode(const QPointF &center, double size,
                                const QUuid &parent) {
  //  QRectF rect(center - QPointF(double(size / 2), double(size / 2)),
  //              QSizeF(double(size), double(size)));
  auto points = HCommon::BuildCircle(center, double(size), 360);
  auto node = new HDragNode();
  auto geo = HSGNodeCommon::buildGeometry(points, GL_POLYGON);
  auto c = HSGNodeCommon::buildColor(Qt::red);
  node->setGeometry(geo);
  node->setMaterial(c);
  node->setFlag(QSGNode::OwnsMaterial);
  node->setFlag(QSGNode::OwnsGeometry);
  node->setParent(parent);
  node->_size = size;
  node->setCenter(center);
  return node;
}

void HDragNode::moveTo(const QPointF &center) {
  //  QRectF rect(center - QPointF(double(_size / 2), double(_size / 2)),
  //              QSizeF(double(_size), double(_size)));
  //  auto points = HCommon::BuildRectList(rect);
  auto points = HCommon::BuildCircle(center, double(_size), 360);
  auto geo = HSGNodeCommon::buildGeometry(points, GL_POLYGON);
  auto c = HSGNodeCommon::buildColor(Qt::red);
  setGeometry(geo);
  setMaterial(c);
  setFlag(QSGNode::OwnsMaterial);
  setFlag(QSGNode::OwnsGeometry);
}

bool HDragNode::pointIn(const QPointF &point, double scale) {
  auto geo = geometry();
  if (!geo) return false;
  auto count = geo->vertexCount();
  auto point_list = static_cast<QSGGeometry::Point2D *>(geo->vertexData());
  QList<QPointF> list;
  for (int i = 0; i < count; i++) {
    auto p = point_list[i];
    list.push_back(QPointF(double(p.x), double(p.y)));
  }
  auto rect = HCommon::FindRect(list);
  bool inrect = HCommon::PointInRect(point, rect);
  if (inrect) {
    HPlanVector vec;
    auto dis = vec.dis(rect.center(), point);
    if (dis < _size) {
      return true;
    }
  }
  return false;
}

void HDragNode::setPointIndex(int index) { insert("point_index", index); }

int HDragNode::getPointIndex() { return value("point_index").toInt(); }

void HDragNode::setFollowIndex(int index) { insert("follow_index", index); }

int HDragNode::getFollowIndex() { return value("follow_index").toInt(); }

void HDragNode::setCurSor(const QCursor &cursor) { _cursor = cursor; }

QCursor HDragNode::getCursor() { return _cursor; }

void HDragNode::setParent(const QUuid &p) { _parent = p; }

QUuid HDragNode::getParent() { return _parent; }

void HDragNode::setCenter(const QPointF &c) { _center = c; }

QPointF HDragNode::getCenter() { return _center; }
