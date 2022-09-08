﻿#ifndef HSHAPENODES_H
#define HSHAPENODES_H

#include <QJsonObject>
#include <QRectF>

#include "../HBoard_global.h"
#include "hfillnode.h"

class HBOARD_EXPORT HShapeLineNode : public HFillNode {
 public:
  HShapeLineNode(const QPointF& begin, const QPointF& end,
                 const QJsonObject& param);

  virtual QSGNode* buildDragNode() override;

  virtual void updateIndexPoint(int index, const QPointF& point) override;
};

class HBOARD_EXPORT HShapeRectNode : public HFillNode {
 public:
  HShapeRectNode(const QRectF& rect, const QJsonObject& param);
};

class HBOARD_EXPORT HShapeCurveNode : public HFillNode {
 public:
  HShapeCurveNode(const QList<QPointF>& list, const QJsonObject& param);
};

class HBOARD_EXPORT HShapeFillRectNode : public HFillNode {
 public:
  HShapeFillRectNode(const QRectF& rect, const QJsonObject& param);
};

class HBOARD_EXPORT HShapeCircleNode : public HFillNode {
 public:
  HShapeCircleNode(const QPointF& center, double radius,
                   const QJsonObject& param);
};

class HBOARD_EXPORT HShapeFillCircleNode : public HFillNode {
 public:
  HShapeFillCircleNode(const QPointF& center, double radius,
                       const QJsonObject& param);
};

class HBOARD_EXPORT HShapeCrossNode : public HFillNode {
 public:
  HShapeCrossNode(const QPointF& center, double size, const QJsonObject& param);
};

class HBOARD_EXPORT HShapeXNode : public HFillNode {
 public:
  HShapeXNode(const QPointF& center, double size, const QJsonObject& param);
};

#endif  // HSHAPENODES_H
