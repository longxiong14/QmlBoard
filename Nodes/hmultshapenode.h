#ifndef HMULTSHAPENODE_H
#define HMULTSHAPENODE_H

#include <QSGGeometryNode>

#include "../HBoard_global.h"
#include "hnodebase.h"
class HBOARD_EXPORT HMultShapeNode : public HNodeBase {
 public:
  HMultShapeNode();
  HMultShapeNode(const QList<QList<QPointF>> &contours,
                 unsigned long type = GL_LINE_LOOP,
                 const QJsonObject &p = QJsonObject());

 public:
  virtual QSGNode *get() override;
  virtual QSGNode *build(HBoard *) override;
  virtual QRectF getBoundRect() override;
  virtual QList<QPointF> getPointList() override;
  virtual void move(const QPointF &p) override;
  virtual void setColor(const QColor &color) override;
  virtual void setParam(const QJsonObject &p) override;

  virtual NODETYPE nodeType() override;
  virtual void updateDrawMode(unsigned long mode) override;

  virtual void changedSelectStatus() override;
  virtual void timeOut() override;

 public:
  virtual int save(QJsonObject &o) override;
  virtual int load(const QJsonObject &o) override;

 public:
  QList<QList<QPointF>> getContours();

 protected:
  QColor getColor(const QJsonObject &p);
  QList<QPointF> getGeometryPoints(QSGGeometryNode *node);
  void buildNode(const QList<QList<QPointF>> &list, unsigned long type);

 protected:
  QSGNode *_node;
  QSGNode *_dash_node;
};

#endif  // HMULTSHAPENODE_H
