#include "hdragnode.h"

#include <QDebug>
#include <QSize>

#include "../Common/hcommons.h"
#include "../Common/hsgnodecommon.h"
#define DEBUG qDebug() << __FUNCTION__ << __LINE__
HDragNode::HDragNode()
    : QSGGeometryNode(), _cursor(Qt::CursorShape::ArrowCursor) {}

HDragNode *HDragNode::buildNode(const QPointF &center, int size,
                                const QUuid &parent) {
  QRectF rect(center - QPointF(size / 2, size / 2), QSize(size, size));
  auto points = HCommon::BuildRectList(rect);
  auto node = new HDragNode();
  auto geo = HSGNodeCommon::buildGeometry(points, GL_QUADS);
  auto c = HSGNodeCommon::buildColor(Qt::red);
  node->setGeometry(geo);
  node->setMaterial(c);
  node->setFlag(QSGNode::OwnsMaterial);
  node->setFlag(QSGNode::OwnsGeometry);
  node->setParent(parent);
  return node;
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

void HDragNode::setCurSor(const QCursor &cursor) { _cursor = cursor; }

QCursor HDragNode::getCursor() { return _cursor; }

void HDragNode::setParent(const QUuid &p) { _parent = p; }

QUuid HDragNode::getParent() { return _parent; }
