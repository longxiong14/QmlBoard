#include "hshapenodes.h"

#include <QDebug>
#include <QList>

#include "../Common/hcommons.h"
#include "../Common/hsgnodecommon.h"
#include "hdragnode.h"
#define DEBUG qDebug() << __FUNCTION__ << __LINE__

void updateShapeNodeIndexPoint(HFillNode *shape, int index,
                               const QPointF &point) {
  auto node = shape->get();
  if (node) {
    auto gnode = dynamic_cast<QSGGeometryNode *>(node);
    if (gnode) {
      auto geo = gnode->geometry();
      auto count = geo->vertexCount();
      if (count > index && index >= 0) {
        auto point_list =
            static_cast<QSGGeometry::Point2D *>(geo->vertexData());
        QSGGeometry::Point2D pt;
        pt.set(float(point.x()), float(point.y()));
        point_list[index] = pt;
      }
    }
  }
}

void moveDragNode(QSGNode *drag_node, const QList<QPointF> &list) {
  for (int i = 0; i < list.size(); i++) {
    HDragNode *drag = dynamic_cast<HDragNode *>(drag_node->childAtIndex(i));
    if (drag) {
      drag->moveTo(list[i]);
    }
  }
}

HShapeLineNode::HShapeLineNode() { _shape_type = "shape line"; }

HShapeLineNode::HShapeLineNode(const QPointF &begin, const QPointF &end,
                               const QJsonObject &param)
    : HFillNode(QList<QPointF>{begin, end}, GL_LINES, param) {
  _shape_type = "shape line";
}

void HShapeLineNode::move(const QPointF &p) {
  HFillNode::move(p);
  auto list = getPointList();
  if (_drag_node) {
    moveDragNode(_drag_node, list);
    flushMayiLine();
  }
}

QSGNode *HShapeLineNode::buildDragNode() {
  QSGNode *node = new QSGNode();
  auto pts = getPointList();
  for (int i = 0; i < pts.size(); i++) {
    HDragNode *n = HDragNode::buildNode(pts[i], 5 * getLineWidth(), id());
    n->setPointIndex(i);
    n->setCurSor(Qt::CursorShape::SizeAllCursor);
    node->appendChildNode(n);
  }
  return node;
}

void HShapeLineNode::updateIndexPoint(int index, const QPointF &point) {
  updateShapeNodeIndexPoint(this, index, point);
  HNodeBase::updateIndexPoint(index, point);
}

HShapeRectNode::HShapeRectNode() { _shape_type = "shape rect"; }

HShapeRectNode::HShapeRectNode(const QRectF &rect, const QJsonObject &param)
    : HFillNode(rect, GL_LINE_LOOP, param) {
  _shape_type = "shape rect";
}

QSGNode *HShapeRectNode::buildDragNode() {
  QSGNode *node = new QSGNode();
  auto rect = getBoundRect();
  createRectDragNode(node, rect, id(), 5 * getLineWidth());
  return node;
}

void HShapeRectNode::move(const QPointF &p) {
  HFillNode::move(p);
  updateDragNodes(_drag_node, getBoundRect());
}

void HShapeRectNode::updateIndexPoint(int index, const QPointF &point) {
  auto rect = getBoundRect();
  QRectF out_rect;
  QList<QPointF> list;
  if (0 == updateRectDragNode(index, rect, point, out_rect)) {
    list = HCommon::BuildRectList(out_rect);
    if (!list.empty()) drawPoints(list);
    updateDragNodes(_drag_node, out_rect);
    flushMayiLine();
  }
}

void HShapeRectNode::updateDragNodes(QSGNode *drag_node, const QRectF &rect) {
  if (drag_node) {
    int size = drag_node->childCount();
    auto map = getRectDragNodeMap(rect);
    for (int i = 0; i < size; i++) {
      auto n = dynamic_cast<HDragNode *>(drag_node->childAtIndex(i));
      if (n && map.contains(n->getPointIndex())) {
        n->moveTo(map[n->getPointIndex()]._point);
      }
    }
  }
}

void HShapeRectNode::createRectDragNode(QSGNode *node, const QRectF &rect,
                                        const QUuid &id, float size) {
  if (!node) return;
  QMap<int, dragNodeMsg> map = getRectDragNodeMap(rect);
  for (const auto &k : map.keys()) {
    HDragNode *n = HDragNode::buildNode(map.value(k)._point, size, id);
    n->setPointIndex(k);
    n->setCurSor(map.value(k)._cursor);
    node->appendChildNode(n);
  }
}

int HShapeRectNode::updateRectDragNode(int index, const QRectF &rect,
                                       const QPointF &point, QRectF &out_rect) {
  switch (index) {
    case 0:
      if (point.x() >= rect.right() || point.y() >= rect.bottom()) return -1;
      out_rect = HCommon::BuildRect(rect.bottomRight(), point);
      break;
    case 1:
      if (point.x() <= rect.left() || point.y() <= rect.top()) return -1;
      out_rect = HCommon::BuildRect(rect.topLeft(), point);
      break;
    case 2:
      if (point.x() <= rect.left() || point.y() >= rect.bottom()) return -1;
      out_rect = HCommon::BuildRect(rect.bottomLeft(), point);
      break;
    case 3:
      if (point.x() >= rect.right() || point.y() <= rect.top()) return -1;
      out_rect = HCommon::BuildRect(rect.topRight(), point);
      break;
    case 4:
      if (point.y() >= rect.bottom()) return -1;
      out_rect = HCommon::BuildRect(QPointF(rect.left(), point.y()),
                                    rect.bottomRight());
      break;
    case 5:
      if (point.y() <= rect.top()) return -1;
      out_rect =
          HCommon::BuildRect(QPointF(rect.right(), point.y()), rect.topLeft());
      break;
    case 6:
      if (point.x() >= rect.right()) return -1;
      out_rect = HCommon::BuildRect(QPointF(point.x(), rect.top()),
                                    rect.bottomRight());
      break;
    case 7:
      if (point.x() <= rect.left()) return -1;
      out_rect =
          HCommon::BuildRect(QPointF(point.x(), rect.top()), rect.bottomLeft());
      break;
  }
  return 0;
}

QMap<int, HShapeRectNode::dragNodeMsg> HShapeRectNode::getRectDragNodeMap(
    const QRectF &rect) {
  auto c = rect.center();
  QMap<int, dragNodeMsg> map{
      {0, {rect.topLeft(), Qt::SizeFDiagCursor}},
      {1, {rect.bottomRight(), Qt::SizeFDiagCursor}},
      {2, {rect.topRight(), Qt::SizeBDiagCursor}},
      {3, {rect.bottomLeft(), Qt::SizeBDiagCursor}},
      {4, {QPointF(c.x(), rect.top()), Qt::SizeVerCursor}},
      {5, {QPointF(c.x(), rect.bottom()), Qt::SizeVerCursor}},
      {6, {QPointF(rect.left(), c.y()), Qt::SizeHorCursor}},
      {7, {QPointF(rect.right(), c.y()), Qt::SizeHorCursor}}};
  return map;
}

HShapeCurveNode::HShapeCurveNode() { _shape_type = "shape curve"; }

HShapeCurveNode::HShapeCurveNode(const QList<QPointF> &list,
                                 const QJsonObject &param)
    : HFillNode(list, GL_LINE_STRIP, param) {
  _shape_type = "shape curve";
}

void HShapeCurveNode::move(const QPointF &p) {
  HFillNode::move(p);
  if (_drag_node) {
    moveDragNode(_drag_node, getPointList());
  }
}

QSGNode *HShapeCurveNode::buildDragNode() {
  QSGNode *node = new QSGNode();
  auto pts = getPointList();
  for (int i = 0; i < pts.size(); i++) {
    HDragNode *n = HDragNode::buildNode(pts[i], 5 * getLineWidth(), id());
    n->setPointIndex(i);
    n->setCurSor(Qt::CursorShape::SizeAllCursor);
    node->appendChildNode(n);
  }
  return node;
}

void HShapeCurveNode::updateIndexPoint(int index, const QPointF &point) {
  updateShapeNodeIndexPoint(this, index, point);
  HNodeBase::updateIndexPoint(index, point);
}

HShapePolyNode::HShapePolyNode() { _shape_type = "shape poly"; }

HShapePolyNode::HShapePolyNode(const QList<QPointF> &list,
                               const QJsonObject &param)
    : HFillNode(list, GL_LINE_STRIP, param) {
  _shape_type = "shape poly";
}

void HShapePolyNode::move(const QPointF &p) {
  HFillNode::move(p);
  if (_drag_node) {
    moveDragNode(_drag_node, getPointList());
  }
}

QSGNode *HShapePolyNode::buildDragNode() {
  QSGNode *node = new QSGNode();
  auto points = getPointList();
  int size = points.size();
  for (int i = 0; i < size - 1; i++) {
    auto drag = HDragNode::buildNode(points[i], 5 * getLineWidth(), id());
    drag->setPointIndex(i);
    drag->setCurSor(Qt::CursorShape::SizeAllCursor);
    if (0 == i) drag->setFollowIndex(size - 1);
    node->appendChildNode(drag);
  }
  return node;
}

void HShapePolyNode::updateIndexPoint(int index, const QPointF &point) {
  if (_drag_node) {
    auto gnode = dynamic_cast<HDragNode *>(_drag_node->childAtIndex(index));
    if (gnode) {
      auto follow = gnode->getFollowIndex();
      updateShapeNodeIndexPoint(this, index, point);
      HNodeBase::updateIndexPoint(index, point);
      if (-1 != follow) {
        updateShapeNodeIndexPoint(this, follow, point);
      }
    }
  }
}

HShapeFillRectNode::HShapeFillRectNode() { _shape_type = "shape fill rect"; }

HShapeFillRectNode::HShapeFillRectNode(const QRectF &rect,
                                       const QJsonObject &param)
    : HFillNode(rect, GL_QUADS, param) {
  _shape_type = "shape fill rect";
}

void HShapeFillRectNode::move(const QPointF &p) {
  HFillNode::move(p);
  HShapeRectNode::updateDragNodes(_drag_node, getBoundRect());
}

QSGNode *HShapeFillRectNode::buildDragNode() {
  QSGNode *node = new QSGNode();
  auto rect = getBoundRect();
  HShapeRectNode::createRectDragNode(node, rect, id(), 5 * getLineWidth());
  return node;
}

void HShapeFillRectNode::updateIndexPoint(int index, const QPointF &point) {
  auto rect = getBoundRect();
  QRectF out_rect;
  QList<QPointF> list;
  if (0 == HShapeRectNode::updateRectDragNode(index, rect, point, out_rect)) {
    list = HCommon::BuildRectList(out_rect);
    if (!list.empty()) drawPoints(list);
    HShapeRectNode::updateDragNodes(_drag_node, out_rect);
    flushMayiLine();
  }
}

HShapeCircleNode::HShapeCircleNode() { _shape_type = "shape circle"; }

HShapeCircleNode::HShapeCircleNode(const QPointF &center, double radius,
                                   const QJsonObject &param)
    : HFillNode(HCommon::BuildCircle(center, radius, 360), GL_LINE_LOOP,
                param) {
  _shape_type = "shape circle";
}

HShapeFillCircleNode::HShapeFillCircleNode() {
  _shape_type = "shape fill circle";
}

HShapeFillCircleNode::HShapeFillCircleNode(const QPointF &center, double radius,
                                           const QJsonObject &param)
    : HFillNode(HCommon::BuildCircle(center, radius, 360), GL_POLYGON, param) {
  _shape_type = "shape fill circle";
}

HShapeCrossNode::HShapeCrossNode() { _shape_type = "shape cross"; }

HShapeCrossNode::HShapeCrossNode(const QPointF &center, double size,
                                 const QJsonObject &param)
    : HFillNode(QList<QPointF>{center,
                               {center.x(), center.y() - size},
                               center,
                               {center.x(), center.y() + size},
                               center,
                               {center.x() - size, center.y()},
                               center,
                               {center.x() + size, center.y()}},
                GL_LINES, param) {
  _shape_type = "shape cross";
}

HShapeXNode::HShapeXNode() { _shape_type = "shape x node"; }

HShapeXNode::HShapeXNode(const QPointF &center, double size,
                         const QJsonObject &param)
    : HFillNode(QList<QPointF>{center,
                               {center.x() - size, center.y() - size},
                               center,
                               {center.x() + size, center.y() + size},
                               center,
                               {center.x() + size, center.y() - size},
                               center,
                               {center.x() - size, center.y() + size}},
                GL_LINES, param) {
  _shape_type = "shape x node";
}
