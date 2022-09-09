#ifndef HSHAPENODES_H
#define HSHAPENODES_H

#include <QJsonObject>
#include <QRectF>

#include "../HBoard_global.h"
#include "hfillnode.h"

class HBOARD_EXPORT HShapeLineNode : public HFillNode {
 public:
  HShapeLineNode();
  HShapeLineNode(const QPointF& begin, const QPointF& end,
                 const QJsonObject& param);

  virtual void move(const QPointF& p) override;

  virtual QSGNode* buildDragNode() override;

  virtual void updateIndexPoint(int index, const QPointF& point) override;
};

class HBOARD_EXPORT HShapeRectNode : public HFillNode {
 protected:
  struct dragNodeMsg {
    QPointF _point;
    Qt::CursorShape _cursor;
  };

 public:
  HShapeRectNode();
  HShapeRectNode(const QRectF& rect, const QJsonObject& param);

  virtual QSGNode* buildDragNode() override;

  virtual void move(const QPointF& p) override;

  virtual void updateIndexPoint(int index, const QPointF& point) override;

 public:
  static void updateDragNodes(QSGNode* drag_node, const QRectF& rect);

  static void createRectDragNode(QSGNode* node, const QRectF& rect,
                                 const QUuid& id, float size);

  static int updateRectDragNode(int index, const QRectF& rect,
                                const QPointF& point, QRectF& out_rect);

  static QMap<int, dragNodeMsg> getRectDragNodeMap(const QRectF& rect);
};

class HBOARD_EXPORT HShapeCurveNode : public HFillNode {
 public:
  HShapeCurveNode();
  HShapeCurveNode(const QList<QPointF>& list, const QJsonObject& param);

  virtual void move(const QPointF& p) override;

  virtual QSGNode* buildDragNode() override;

  virtual void updateIndexPoint(int index, const QPointF& point) override;
};

class HBOARD_EXPORT HShapePolyNode : public HFillNode {
 public:
  HShapePolyNode();
  HShapePolyNode(const QList<QPointF>& list, const QJsonObject& param);

  virtual void move(const QPointF& p) override;

  virtual QSGNode* buildDragNode() override;

  virtual void updateIndexPoint(int index, const QPointF& point) override;
};

class HBOARD_EXPORT HShapeFillRectNode : public HFillNode {
 public:
  HShapeFillRectNode();
  HShapeFillRectNode(const QRectF& rect, const QJsonObject& param);

  virtual void move(const QPointF& p) override;

  virtual QSGNode* buildDragNode() override;

  virtual void updateIndexPoint(int index, const QPointF& point) override;
};

class HBOARD_EXPORT HShapeCircleNode : public HFillNode {
 public:
  HShapeCircleNode();
  HShapeCircleNode(const QPointF& center, double radius,
                   const QJsonObject& param);
};

class HBOARD_EXPORT HShapeFillCircleNode : public HFillNode {
 public:
  HShapeFillCircleNode();
  HShapeFillCircleNode(const QPointF& center, double radius,
                       const QJsonObject& param);
};

class HBOARD_EXPORT HShapeCrossNode : public HFillNode {
 public:
  HShapeCrossNode();
  HShapeCrossNode(const QPointF& center, double size, const QJsonObject& param);
};

class HBOARD_EXPORT HShapeXNode : public HFillNode {
 public:
  HShapeXNode();
  HShapeXNode(const QPointF& center, double size, const QJsonObject& param);
};

#endif  // HSHAPENODES_H
