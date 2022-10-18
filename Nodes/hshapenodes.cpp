#include "hshapenodes.h"

#include <QDebug>
#include <QList>

#include "../Common/hcommons.h"
#include "../Common/hsgnodecommon.h"
#include "../hboard.h"
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

HShapeLineNode::HShapeLineNode() {}

HShapeLineNode::HShapeLineNode(const QPointF &begin, const QPointF &end,
                               const QJsonObject &param)
    : HFillNode(QList<QPointF>{begin, end}, GL_LINES, param) {}

void HShapeLineNode::move(const QPointF &p) {
  HFillNode::move(p);
  auto list = getPointList();
  //  if (_drag_node) {
  //    moveDragNode(_drag_node, list);
  //    flushMayiLine();
  //  }
}

HNodeBase::NODETYPE HShapeLineNode::nodeType() { return NODETYPE::SHAPELINE; }

QSGNode *HShapeLineNode::buildDragNode(HBoard *board) {
  if (!board) return nullptr;
  if (_drag_node) return _drag_node;
  _drag_node = new QSGNode();
  auto pts = getPointList();
  for (int i = 0; i < pts.size(); i++) {
    HDragNode *n = HDragNode::buildNode(board->LCS2WCS(pts[i]),
                                        double(3 + getLineWidth()), id());
    n->setPointIndex(i);
    n->setCurSor(Qt::CursorShape::SizeAllCursor);
    _drag_node->appendChildNode(n);
  }
  return _drag_node;
}

void HShapeLineNode::updateIndexPoint(int index, const QPointF &point) {
  updateShapeNodeIndexPoint(this, index, point);
  HNodeBase::updateIndexPoint(index, point);
}

// void HShapeLineNode::updateDragNodePoint(HBoard *board) {
//  if (_drag_node && board) {
//    auto count = _drag_node->childCount();
//    auto pts = getPointList();
//    for (int i = 0; i < count; i++) {
//      auto node = dynamic_cast<HDragNode *>(_drag_node->childAtIndex(i));
//      if (node) {
//        auto index = node->getPointIndex();
//        if (index >= 0 && index < pts.size()) {
//          node->moveTo(board->LCS2WCS(pts[i]));
//        }
//      }
//    }
//  }
//}

HShapeRectNode::HShapeRectNode() {}

HShapeRectNode::HShapeRectNode(const QRectF &rect, const QJsonObject &param)
    : HFillNode(rect, GL_LINE_LOOP, param) {}

QSGNode *HShapeRectNode::buildDragNode(HBoard *board) {
  if (!board) return nullptr;
  if (_drag_node) return _drag_node;
  _drag_node = new QSGNode();
  auto rect = getBoundRect();
  auto tl = board->LCS2WCS(rect.topLeft());
  auto br = board->LCS2WCS(rect.bottomRight());
  createRectDragNode(_drag_node, QRectF(tl, br), id(),
                     double(3 + getLineWidth()));
  return _drag_node;
}

void HShapeRectNode::move(const QPointF &p) {
  HFillNode::move(p);
  //  updateDragNodes(_drag_node, getBoundRect());
}

HNodeBase::NODETYPE HShapeRectNode::nodeType() {
  return HNodeBase::NODETYPE::SHAPERECT;
}

void HShapeRectNode::updateIndexPoint(int index, const QPointF &point) {
  auto rect = getBoundRect();
  QRectF out_rect;
  QList<QPointF> list;
  if (0 == updateRectDragNode(index, rect, point, out_rect)) {
    list = HCommon::BuildRectList(out_rect);
    if (!list.empty()) drawPoints(list);
    //    updateDragNodes(_drag_node, out_rect);
    flushMayiLine();
  }
}

int HShapeRectNode::save(QJsonObject &o) {
  auto rect = getBoundRect();
  QJsonObject r;
  r.insert("x", rect.x());
  r.insert("y", rect.y());
  r.insert("width", rect.width());
  r.insert("height", rect.height());
  o.insert("rect", r);
  return HNodeBase::save(o);
}

int HShapeRectNode::load(const QJsonObject &o) {
  HNodeBase::load(o);
  QJsonObject r = o.value("rect").toObject();
  if (!r.empty()) {
    QRectF rect(r.value("x").toDouble(), r.value("y").toDouble(),
                r.value("width").toDouble(), r.value("height").toDouble());

    setOurGeometry(HCommon::BuildRectList(rect), GL_LINE_LOOP);
    setColor(getColor(o.value("param").toObject()));
  }
  return 0;
}

void HShapeRectNode::updateDragNodePoint(HBoard *board) {
  if (!board || !_drag_node) return;
  auto bound = getBoundRect();
  auto rect = QRectF(board->LCS2WCS(bound.topLeft()),
                     board->LCS2WCS(bound.bottomRight()));
  QMap<int, dragNodeMsg> map = getRectDragNodeMap(rect);
  for (int i = 0; i < _drag_node->childCount(); i++) {
    auto drag = dynamic_cast<HDragNode *>(_drag_node->childAtIndex(i));
    if (drag) {
      auto index = drag->getPointIndex();
      if (map.contains(index)) {
        drag->moveTo(map[index]._point);
      }
    }
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
                                        const QUuid &id, double size) {
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

HShapeCurveNode::HShapeCurveNode() {}

HShapeCurveNode::HShapeCurveNode(const QList<QPointF> &list,
                                 const QJsonObject &param)
    : HFillNode(list, GL_LINE_STRIP, param) {}

HNodeBase::NODETYPE HShapeCurveNode::nodeType() {
  return HNodeBase::NODETYPE::SHAPECURVE;
}

void HShapeCurveNode::move(const QPointF &p) {
  HFillNode::move(p);
  //  if (_drag_node) {
  //    moveDragNode(_drag_node, getPointList());
  //  }
}

QSGNode *HShapeCurveNode::buildDragNode(HBoard *board) {
  if (!board) return nullptr;
  if (_drag_node) return _drag_node;
  _drag_node = new QSGNode();
  auto pts = getPointList();
  for (int i = 0; i < pts.size(); i++) {
    HDragNode *n = HDragNode::buildNode(board->LCS2WCS(pts[i]),
                                        double(3 + getLineWidth()), id());
    n->setPointIndex(i);
    n->setCurSor(Qt::CursorShape::SizeAllCursor);
    _drag_node->appendChildNode(n);
  }
  return _drag_node;
}

void HShapeCurveNode::updateIndexPoint(int index, const QPointF &point) {
  updateShapeNodeIndexPoint(this, index, point);
  HNodeBase::updateIndexPoint(index, point);
}

// void HShapeCurveNode::updateDragNodePoint(HBoard *board) {
//  if (_drag_node && board) {
//    auto count = _drag_node->childCount();
//    auto pts = getPointList();
//    for (int i = 0; i < count; i++) {
//      auto node = dynamic_cast<HDragNode *>(_drag_node->childAtIndex(i));
//      if (node) {
//        auto index = node->getPointIndex();
//        if (index >= 0 && index < pts.size()) {
//          node->moveTo(board->LCS2WCS(pts[i]));
//        }
//      }
//    }
//  }
//}

HShapePolyNode::HShapePolyNode() {}

HShapePolyNode::HShapePolyNode(const QList<QPointF> &list,
                               const QJsonObject &param)
    : HFillNode(list, GL_LINE_STRIP, param) {}

void HShapePolyNode::move(const QPointF &p) {
  HFillNode::move(p);
  //  if (_drag_node) {
  //    moveDragNode(_drag_node, getPointList());
  //  }
}

HNodeBase::NODETYPE HShapePolyNode::nodeType() {
  return HNodeBase::NODETYPE::SHAPEPOLY;
}

QSGNode *HShapePolyNode::buildDragNode(HBoard *board) {
  if (!board) return nullptr;
  if (_drag_node) return _drag_node;
  _drag_node = new QSGNode();
  auto points = getPointList();
  int size = points.size();
  for (int i = 0; i < size - 1; i++) {
    auto drag = HDragNode::buildNode(board->LCS2WCS(points[i]),
                                     double(3 + getLineWidth()), id());
    drag->setPointIndex(i);
    drag->setCurSor(Qt::CursorShape::SizeAllCursor);
    if (0 == i) drag->setFollowIndex(size - 1);
    _drag_node->appendChildNode(drag);
  }
  return _drag_node;
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

// void HShapePolyNode::updateDragNodePoint(HBoard *board) {
//  if (_drag_node && board) {
//    auto count = _drag_node->childCount();
//    auto pts = getPointList();
//    for (int i = 0; i < count; i++) {
//      auto node = dynamic_cast<HDragNode *>(_drag_node->childAtIndex(i));
//      if (node) {
//        auto index = node->getPointIndex();
//        if (index >= 0 && index < pts.size()) {
//          auto point = board->LCS2WCS(pts[i]);
//          node->moveTo(point);
//        }
//      }
//    }
//  }
//}

HShapeFillRectNode::HShapeFillRectNode() {}

HShapeFillRectNode::HShapeFillRectNode(const QRectF &rect,
                                       const QJsonObject &param)
    : HFillNode(rect, GL_QUADS, param) {}

void HShapeFillRectNode::move(const QPointF &p) {
  HFillNode::move(p);
  //  HShapeRectNode::updateDragNodes(_drag_node, getBoundRect());
}

HNodeBase::NODETYPE HShapeFillRectNode::nodeType() {
  return HNodeBase::NODETYPE::SHAPEFILLRECT;
}

QSGNode *HShapeFillRectNode::buildDragNode(HBoard *board) {
  if (!board) return nullptr;
  if (_drag_node) return _drag_node;
  _drag_node = new QSGNode();
  auto rect = getBoundRect();
  auto tl = board->LCS2WCS(rect.topLeft());
  auto br = board->LCS2WCS(rect.bottomRight());
  HShapeRectNode::createRectDragNode(_drag_node, QRectF(tl, br), id(),
                                     double(3 + getLineWidth()));
  return _drag_node;
}

void HShapeFillRectNode::updateIndexPoint(int index, const QPointF &point) {
  auto rect = getBoundRect();
  QRectF out_rect;
  QList<QPointF> list;
  if (0 == HShapeRectNode::updateRectDragNode(index, rect, point, out_rect)) {
    list = HCommon::BuildRectList(out_rect);
    if (!list.empty()) drawPoints(list);
    flushMayiLine();
  }
}

int HShapeFillRectNode::save(QJsonObject &o) {
  auto rect = getBoundRect();
  QJsonObject r;
  r.insert("x", rect.x());
  r.insert("y", rect.y());
  r.insert("width", rect.width());
  r.insert("height", rect.height());
  o.insert("rect", r);
  return HNodeBase::save(o);
}

int HShapeFillRectNode::load(const QJsonObject &o) {
  HNodeBase::load(o);
  QJsonObject r = o.value("rect").toObject();
  if (!r.empty()) {
    QRectF rect(r.value("x").toDouble(), r.value("y").toDouble(),
                r.value("width").toDouble(), r.value("height").toDouble());

    setOurGeometry(HCommon::BuildRectList(rect), GL_QUADS);
    setColor(getColor(o.value("param").toObject()));
  }
  return 0;
}

void HShapeFillRectNode::updateDragNodePoint(HBoard *board) {
  if (!board || !_drag_node) return;
  auto bound = getBoundRect();
  auto rect = QRectF(board->LCS2WCS(bound.topLeft()),
                     board->LCS2WCS(bound.bottomRight()));
  QMap<int, HShapeRectNode::dragNodeMsg> map =
      HShapeRectNode::getRectDragNodeMap(rect);
  for (int i = 0; i < _drag_node->childCount(); i++) {
    auto drag = dynamic_cast<HDragNode *>(_drag_node->childAtIndex(i));
    if (drag) {
      auto index = drag->getPointIndex();
      if (map.contains(index)) {
        drag->moveTo(map[index]._point);
      }
    }
  }
}

HShapeCircleNode::HShapeCircleNode() {}

HShapeCircleNode::HShapeCircleNode(const QPointF &center,
                                   const QJsonObject &param)
    : HFillNode(HCommon::BuildCircle(center,
                                     0 == param.value("radius").toInt()
                                         ? 50
                                         : param.value("radius").toInt(),
                                     360),
                GL_LINE_LOOP, param) {}

HNodeBase::NODETYPE HShapeCircleNode::nodeType() {
  return HNodeBase::NODETYPE::SHAPECIRCLE;
}

void HShapeCircleNode::setParam(const QJsonObject &p) {
  auto r = p.value("radius").toInt();
  auto radius = (0 == r ? 50 : r);
  auto list = HCommon::BuildCircle(getBoundRect().center(), radius, 360);
  drawPoints(list);
  flushMayiLine();
  HFillNode::setParam(p);
}

int HShapeCircleNode::save(QJsonObject &o) {
  QJsonObject center;
  auto c = getBoundRect().center();
  center.insert("x", c.x());
  center.insert("y", c.y());
  o.insert("center", center);
  return HNodeBase::save(o);
}

int HShapeCircleNode::load(const QJsonObject &o) {
  HNodeBase::load(o);
  QJsonObject center = o.value("center").toObject();
  QPointF c(center.value("x").toDouble(), center.value("y").toDouble());
  double r = 50;
  if (_param.contains("radius")) {
    r = _param.value("radius").toDouble();
  }
  auto pts = HCommon::BuildCircle(c, r, 360);
  setOurGeometry(pts, GL_LINE_LOOP);
  setColor(getColor(_param));
  return 0;
}

HShapeFillCircleNode::HShapeFillCircleNode() {}

HNodeBase::NODETYPE HShapeFillCircleNode::nodeType() {
  return HNodeBase::NODETYPE::SHAPEFILLCIRCLE;
}

HShapeFillCircleNode::HShapeFillCircleNode(const QPointF &center,
                                           const QJsonObject &param)
    : HFillNode(HCommon::BuildCircle(center,
                                     0 == param.value("radius").toInt()
                                         ? 50
                                         : param.value("radius").toInt(),
                                     360),
                GL_POLYGON, param) {}

void HShapeFillCircleNode::setParam(const QJsonObject &p) {
  auto r = p.value("radius").toInt();
  auto radius = (0 == r ? 50 : r);
  auto list = HCommon::BuildCircle(getBoundRect().center(), radius, 360);
  drawPoints(list);
  flushMayiLine();
  HFillNode::setParam(p);
}

int HShapeFillCircleNode::save(QJsonObject &o) {
  QJsonObject center;
  auto c = getBoundRect().center();
  center.insert("x", c.x());
  center.insert("y", c.y());
  o.insert("center", center);
  return HNodeBase::save(o);
}

int HShapeFillCircleNode::load(const QJsonObject &o) {
  HNodeBase::load(o);
  QJsonObject center = o.value("center").toObject();
  QPointF c(center.value("x").toDouble(), center.value("y").toDouble());
  double r = 50;
  if (_param.contains("radius")) {
    r = _param.value("radius").toDouble();
  }
  auto pts = HCommon::BuildCircle(c, r, 360);
  setOurGeometry(pts, GL_POLYGON);
  setColor(getColor(_param));
  return 0;
}

HShapeCrossNode::HShapeCrossNode() : _size(50) {}

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
                GL_LINES, param),
      _size(size) {}

HNodeBase::NODETYPE HShapeCrossNode::nodeType() {
  return HNodeBase::NODETYPE::SHAPECROSS;
}

int HShapeCrossNode::save(QJsonObject &o) {
  QJsonObject center;
  auto c = getBoundRect().center();
  center.insert("x", c.x());
  center.insert("y", c.y());
  o.insert("center", center);
  o.insert("cross_size", _size);
  return HNodeBase::save(o);
}

int HShapeCrossNode::load(const QJsonObject &o) {
  HNodeBase::load(o);
  QJsonObject c = o.value("center").toObject();
  QPointF center(c.value("x").toDouble(), c.value("y").toDouble());
  _size = o.value("cross_size").toDouble();
  auto list = QList<QPointF>{center, {center.x(), center.y() - _size},
                             center, {center.x(), center.y() + _size},
                             center, {center.x() - _size, center.y()},
                             center, {center.x() + _size, center.y()}};
  setOurGeometry(list, GL_LINES);
  setColor(getColor(_param));
  return 0;
}

HShapeXNode::HShapeXNode() : _size(50) {}

HNodeBase::NODETYPE HShapeXNode::nodeType() {
  return HNodeBase::NODETYPE::SHAPEXNODE;
}

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
                GL_LINES, param),
      _size(size) {}

int HShapeXNode::save(QJsonObject &o) {
  QJsonObject center;
  auto c = getBoundRect().center();
  center.insert("x", c.x());
  center.insert("y", c.y());
  o.insert("center", center);
  o.insert("x_size", _size);
  return HNodeBase::save(o);
}

int HShapeXNode::load(const QJsonObject &o) {
  HNodeBase::load(o);
  QJsonObject c = o.value("center").toObject();
  QPointF center(c.value("x").toDouble(), c.value("y").toDouble());
  _size = o.value("x_size").toDouble();
  auto list = QList<QPointF>{center, {center.x(), center.y() - _size},
                             center, {center.x(), center.y() + _size},
                             center, {center.x() - _size, center.y()},
                             center, {center.x() + _size, center.y()}};
  setOurGeometry(list, GL_LINE_LOOP);
  setColor(getColor(_param));
  return 0;
}

HShapeEllipseNode::HShapeEllipseNode() {}

HShapeEllipseNode::HShapeEllipseNode(const QRectF &rect,
                                     const QJsonObject &param) {
  setParam(param);
  auto list = HCommon::BuildEllipse(rect, 360);
  setOurGeometry(list, GL_LINE_LOOP);
  setColor(getColor(_param));
}

QSGNode *HShapeEllipseNode::buildDragNode(HBoard *board) {
  if (!board) return nullptr;
  if (_drag_node) return _drag_node;
  _drag_node = new QSGNode();
  auto rect = getBoundRect();
  auto tl = board->LCS2WCS(rect.topLeft());
  auto br = board->LCS2WCS(rect.bottomRight());
  HShapeRectNode::createRectDragNode(_drag_node, QRectF(tl, br), id(),
                                     double(3 + getLineWidth()));
  return _drag_node;
}

void HShapeEllipseNode::move(const QPointF &p) {
  HFillNode::move(p);
  //  HShapeRectNode::updateDragNodes(_drag_node, getBoundRect());
}

void HShapeEllipseNode::updateIndexPoint(int index, const QPointF &point) {
  auto rect = getBoundRect();
  QRectF out_rect;
  QList<QPointF> list;
  if (0 == HShapeRectNode::updateRectDragNode(index, rect, point, out_rect)) {
    list = HCommon::BuildEllipse(out_rect, 360);
    if (!list.empty()) drawPoints(list);
    //    HShapeRectNode::updateDragNodes(_drag_node, out_rect);
    flushMayiLine();
  }
}

HNodeBase::NODETYPE HShapeEllipseNode::nodeType() {
  return NODETYPE::SHAPEELLIPSE;
}

void HShapeEllipseNode::updateDragNodePoint(HBoard *board) {
  if (!board || !_drag_node) return;
  auto bound = getBoundRect();
  auto rect = QRectF(board->LCS2WCS(bound.topLeft()),
                     board->LCS2WCS(bound.bottomRight()));
  QMap<int, HShapeRectNode::dragNodeMsg> map =
      HShapeRectNode::getRectDragNodeMap(rect);
  for (int i = 0; i < _drag_node->childCount(); i++) {
    auto drag = dynamic_cast<HDragNode *>(_drag_node->childAtIndex(i));
    if (drag) {
      auto index = drag->getPointIndex();
      if (map.contains(index)) {
        drag->moveTo(map[index]._point);
      }
    }
  }
}

int HShapeEllipseNode::save(QJsonObject &o) {
  auto rect = getBoundRect();
  QJsonObject r;
  r.insert("x", rect.x());
  r.insert("y", rect.y());
  r.insert("width", rect.width());
  r.insert("height", rect.height());
  o.insert("rect", r);
  return HNodeBase::save(o);
}

int HShapeEllipseNode::load(const QJsonObject &o) {
  HNodeBase::load(o);
  QJsonObject r = o.value("rect").toObject();
  double x = r.value("x").toDouble(), y = r.value("y").toDouble(),
         width = r.value("width").toDouble(),
         height = r.value("height").toDouble();
  QRectF rect(x, y, width, height);
  auto list = HCommon::BuildEllipse(rect, 360);
  setOurGeometry(list, GL_LINE_LOOP);
  setColor(getColor(_param));
  return 0;
}

HShapeFillEllipseNode::HShapeFillEllipseNode() {}

HShapeFillEllipseNode::HShapeFillEllipseNode(const QRectF &rect,
                                             const QJsonObject &param) {
  setParam(param);
  auto list = HCommon::BuildEllipse(rect, 360);
  setOurGeometry(list, GL_POLYGON);
  setColor(getColor(_param));
}

HNodeBase::NODETYPE HShapeFillEllipseNode::nodeType() {
  return NODETYPE::SHAPEFILLELLIPSE;
}

int HShapeFillEllipseNode::load(const QJsonObject &o) {
  HNodeBase::load(o);
  QJsonObject r = o.value("rect").toObject();
  double x = r.value("x").toDouble(), y = r.value("y").toDouble(),
         width = r.value("width").toDouble(),
         height = r.value("height").toDouble();
  QRectF rect(x, y, width, height);
  auto list = HCommon::BuildEllipse(rect, 360);
  setOurGeometry(list, GL_POLYGON);
  setColor(getColor(_param));
  return 0;
}
