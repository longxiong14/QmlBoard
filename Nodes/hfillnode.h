#ifndef HFILLNODE_H
#define HFILLNODE_H

#include <QSGGeometryNode>

#include "hnodebase.h"
class HBOARD_EXPORT HFillNode : public HNodeBase, public QSGGeometryNode {
 public:
  HFillNode(const QList<QPoint>& points, const QColor& color,
            unsigned long type = 0);
  HFillNode(const QRect& rect, const QColor& color, unsigned long type = 0);

  virtual QSGNode* get() override;
  virtual QSGNode* build(HBoard*) override;
  virtual QRect getBoundRect() override;
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

// class HBOARD_EXPORT HPolyNode
//        : public HNodeBase,
//        public QSGGeometryNode
//{
// public:
//    HPolyNode(const QList<QPoint>& points, const QColor& color);
//    virtual QSGNode* get()override;
//    virtual QSGNode * build(HBoard *) override;
//    virtual QRect getBoundRect() override;
//    virtual void move(const QPoint &p) override;
//    virtual void moveTo(const QPoint &p) override;
//    virtual void changedSelectStatus() override;
//    virtual void drawPoints(const QList<QPoint> &points) override;
//};

#endif  // HFILLNODE_H
