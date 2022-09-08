#include "hshapenodes.h"

#include <QList>

#include "../Common/hcommons.h"
#include "../Common/hsgnodecommon.h"
#include "hdragnode.h"
HShapeLineNode::HShapeLineNode(const QPointF &begin, const QPointF &end,
                               const QJsonObject &param)
    : HFillNode(QList<QPointF>{begin, end}, GL_LINES, param) {}

HDragNode *HShapeLineNode::buildDragNode() {
  HDragNode *node = new HDragNode();
  auto pts = getPointList();
  auto p = 20;
  for (int i = 0; i < pts.size(); i++) {
    QRectF rect(pts[i] - QPointF(p / 2, p / 2), QSize(p, p));
    auto gnode = HSGNodeCommon::buildGeometryNode(HCommon::BuildRectList(rect),
                                                  Qt::red, GL_QUADS);
    node->setFlag(i);
    node->appendChildNode(gnode);
  }
  return node;
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
