#ifndef HFILLNODE_H
#define HFILLNODE_H

#include <QSGGeometryNode>

#include "hnodebase.h"
class HBOARD_EXPORT HFillNode : public HNodeBase, public QSGGeometryNode {
 public:
  HFillNode(const QList<QPoint>& points, const QColor& color,
            unsigned long type = GL_LINE_LOOP);
  HFillNode(const QRect& rect, const QColor& color,
            unsigned long type = GL_LINE_LOOP);

  virtual QSGNode* get() override;
  virtual QSGNode* build(HBoard*) override;
  virtual QRect getBoundRect() override;
  virtual QList<QPoint> getPointList() override;
  virtual void move(const QPoint& p) override;
  virtual void moveTo(const QPoint& p) override;
  virtual void changedSelectStatus() override;
  virtual void drawPoints(const QList<QPoint>& points) override;
  virtual void setColor(const QColor& color) override;

 protected:
  QSGGeometry* buildGeometry(const QList<QPoint>& points,
                             unsigned long type = false);
  void setOurGeometry(const QList<QPoint>& points, unsigned long type = false);
};

#endif  // HFILLNODE_H
