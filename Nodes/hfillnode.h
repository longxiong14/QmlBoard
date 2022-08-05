#ifndef HFILLNODE_H
#define HFILLNODE_H

#include <QSGGeometryNode>

#include "hnodebase.h"
class HBOARD_EXPORT HFillNode : public HNodeBase, public QSGGeometryNode {
 public:
  HFillNode(const QList<QPointF>& points, unsigned long type = GL_LINE_LOOP,
            const QJsonObject& p = QJsonObject());
  HFillNode(const QRectF& rect, unsigned long type = GL_LINE_LOOP,
            const QJsonObject& p = QJsonObject());

  virtual QSGNode* get() override;
  virtual QSGNode* build(HBoard*) override;
  virtual QRectF getBoundRect() override;
  virtual QList<QPointF> getPointList() override;
  virtual void move(const QPointF& p) override;
  virtual void moveTo(const QPointF& p) override;
  virtual void drawPoints(const QList<QPointF>& points) override;
  virtual void setColor(const QColor& color) override;
  virtual void setParam(const QJsonObject& p) override;

 protected:
  QSGGeometry* buildGeometry(const QList<QPointF>& points,
                             unsigned long type = false);
  void setOurGeometry(const QList<QPointF>& points, unsigned long type = false);

  QColor getColor(const QJsonObject& p);
};

#endif  // HFILLNODE_H
