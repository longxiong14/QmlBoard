#ifndef HFILLNODE_H
#define HFILLNODE_H

#include <QSGGeometryNode>

#include "../Storage/hstoragebase.h"
#include "hnodebase.h"
class HBOARD_EXPORT HFillNode : public HNodeBase /*, public QSGGeometryNode*/ {
 public:
  HFillNode(const QList<QPointF> &points, unsigned long type = GL_LINE_LOOP,
            const QJsonObject &p =
                QJsonObject({{"b", 0}, {"g", 0}, {"r", 255}, {"a", 255}}));
  HFillNode(const QRectF &rect = QRectF(), unsigned long type = GL_LINE_LOOP,
            const QJsonObject &p =
                QJsonObject({{"b", 0}, {"g", 0}, {"r", 255}, {"a", 255}}));

 public:
  virtual QSGNode *get() override;
  virtual QSGNode *build(HBoard *) override;
  virtual QRectF getBoundRect() override;
  virtual QList<QPointF> getPointList() override;
  virtual void move(const QPointF &p) override;
  virtual void drawPoints(const QList<QPointF> &points) override;
  virtual void setColor(const QColor &color) override;
  virtual void setParam(const QJsonObject &p) override;
  virtual unsigned long drawingMode() override;
  virtual void updateDrawMode(unsigned long mode) override;

 public:
  virtual int save(QJsonObject &o) override;
  virtual int load(const QJsonObject &o) override;

  void clear();

 public:
  void setPointList(const QList<QPointF> &points,
                    unsigned long type = GL_LINE_LOOP,
                    const QJsonObject &p = QJsonObject());

  void setRect(const QRectF &rect, unsigned long type = GL_LINE_LOOP,
               const QJsonObject &p = QJsonObject());

 protected:
  void setOurGeometry(const QList<QPointF> &points, unsigned long type = 0);
  float getLineWidth();

  QColor getColor(const QJsonObject &p);

 protected:
  QSGGeometryNode *_node;
};

#endif  // HFILLNODE_H
