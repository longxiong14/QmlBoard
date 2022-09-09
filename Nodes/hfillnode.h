#ifndef HFILLNODE_H
#define HFILLNODE_H

#include <QSGGeometryNode>

#include "../Storage/hstoragebase.h"
#include "hnodebase.h"
class HBOARD_EXPORT HFillNode : public HNodeBase /*, public QSGGeometryNode*/ {
 public:
  HFillNode();
  HFillNode(const QList<QPointF> &points, unsigned long type = GL_LINE_LOOP,
            const QJsonObject &p = QJsonObject());
  HFillNode(const QRectF &rect, unsigned long type = GL_LINE_LOOP,
            const QJsonObject &p = QJsonObject());
  //  virtual ~HFillNode() override;
  //  HFillNode(const HFillNode &o) = delete;

 public:
  virtual QSGNode *get() override;
  virtual QSGNode *build(HBoard *) override;
  virtual QRectF getBoundRect() override;
  virtual QList<QPointF> getPointList() override;
  virtual void move(const QPointF &p) override;
  virtual void moveTo(const QPointF &p) override;
  virtual void drawPoints(const QList<QPointF> &points) override;
  virtual void setColor(const QColor &color) override;
  virtual void setParam(const QJsonObject &p) override;
  virtual unsigned long drawingMode() override;
  virtual void updateDrawMode(unsigned long mode) override;

 public:
  virtual int save(QJsonObject &o) override;
  virtual int load(const QJsonObject &o) override;
  virtual int save(const QString &path) override;
  virtual int load(const QString &path) override;

  void clear();

 protected:
  void setOurGeometry(const QList<QPointF> &points, unsigned long type = false);
  float getLineWidth();

  QColor getColor(const QJsonObject &p);
  QSGGeometryNode *_node;

 protected:
  unsigned long _drawMode;
};

#endif  // HFILLNODE_H
