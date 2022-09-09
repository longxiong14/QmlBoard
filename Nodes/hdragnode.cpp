#include "hdragnode.h"

#include <QDebug>
#include <QSize>

#include "../Common/hcommons.h"
#include "../Common/hsgnodecommon.h"
#define DEBUG qDebug() << __FUNCTION__ << __LINE__
HDragNode::HDragNode()
    : QSGGeometryNode(), _cursor(Qt::CursorShape::ArrowCursor) {
  setFollowIndex(-1);
}

HDragNode *HDragNode::buildNode(const QPointF &center, float size,
                                const QUuid &parent) {
  QRectF rect(center - QPointF(double(size / 2), double(size / 2)),
              QSizeF(double(size), double(size)));
  auto points = HCommon::BuildRectList(rect);
  auto node = new HDragNode();
  auto geo = HSGNodeCommon::buildGeometry(points, GL_QUADS);
  auto c = HSGNodeCommon::buildColor(Qt::red);
  node->setGeometry(geo);
  node->setMaterial(c);
  node->setFlag(QSGNode::OwnsMaterial);
  node->setFlag(QSGNode::OwnsGeometry);
  node->setParent(parent);
  node->_size = size;
  return node;
}

void HDragNode::moveTo(const QPointF &center) {
  QRectF rect(center - QPointF(double(_size / 2), double(_size / 2)),
              QSizeF(double(_size), double(_size)));
  auto points = HCommon::BuildRectList(rect);
  auto geo = HSGNodeCommon::buildGeometry(points, GL_QUADS);
  auto c = HSGNodeCommon::buildColor(Qt::red);
  setGeometry(geo);
  setMaterial(c);
  setFlag(QSGNode::OwnsMaterial);
  setFlag(QSGNode::OwnsGeometry);
}

bool HDragNode::pointIn(const QPointF &point) {
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
  return HCommon::PointInRect(point, rect);
}

void HDragNode::setPointIndex(int index) { insert("point_index", index); }

int HDragNode::getPointIndex() { return value("point_index").toInt(); }

void HDragNode::setFollowIndex(int index) { insert("follow_index", index); }

int HDragNode::getFollowIndex() { return value("follow_index").toInt(); }

void HDragNode::setCurSor(const QCursor &cursor) { _cursor = cursor; }

QCursor HDragNode::getCursor() { return _cursor; }

void HDragNode::setParent(const QUuid &p) { _parent = p; }

QUuid HDragNode::getParent() { return _parent; }
