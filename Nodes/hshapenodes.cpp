#include "hshapenodes.h"

#include <QDebug>
#include <QList>

#include "../Common/hcommons.h"
#include "../Common/hsgnodecommon.h"
#include "hdragnode.h"
#define DEBUG qDebug() << __FUNCTION__ << __LINE__
HShapeLineNode::HShapeLineNode(const QPointF &begin, const QPointF &end,
                               const QJsonObject &param)
    : HFillNode(QList<QPointF>{begin, end}, GL_LINES, param) {}

QSGNode *HShapeLineNode::buildDragNode() {
  QSGNode *node = new QSGNode();
  auto pts = getPointList();
  auto p = 20;
  for (int i = 0; i < pts.size(); i++) {
    HDragNode *n = HDragNode::buildNode(pts[i], p, id());
    n->setPointIndex(i);
    n->setCurSor(Qt::CursorShape::SizeAllCursor);
    node->appendChildNode(n);
  }
  return node;
}

void HShapeLineNode::updateIndexPoint(int index, const QPointF &point) {
  DEBUG << index << point;
  if (_drag_node) {
  }
  auto node = get();
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

HShapeRectNode::HShapeRectNode(const QRectF &rect, const QJsonObject &param)
    : HFillNode(rect, GL_LINE_LOOP, param) {}

HShapeCurveNode::HShapeCurveNode(const QList<QPointF> &list,
                                 const QJsonObject &param)
    : HFillNode(list, GL_LINE_STRIP, param) {}

HShapeFillRectNode::HShapeFillRectNode(const QRectF &rect,
                                       const QJsonObject &param)
    : HFillNode(rect, GL_QUADS, param) {}

HShapeCircleNode::HShapeCircleNode(const QPointF &center, double radius,
                                   const QJsonObject &param)
    : HFillNode(HCommon::BuildCircle(center, radius, 360), GL_LINE_LOOP,
                param) {}

HShapeFillCircleNode::HShapeFillCircleNode(const QPointF &center, double radius,
                                           const QJsonObject &param)
    : HFillNode(HCommon::BuildCircle(center, radius, 360), GL_POLYGON, param) {}

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
                GL_LINES, param) {}

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
                GL_LINES, param) {}
