#include "hshapenodes.h"

#include <QList>

#include "../Common/hcommons.h"
HShapeLineNode::HShapeLineNode(const QPointF &begin, const QPointF &end,
                               const QJsonObject &param)
    : HFillNode(QList<QPointF>{begin, end}, GL_LINES, param) {}

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
