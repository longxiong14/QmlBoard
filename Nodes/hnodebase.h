#ifndef HNODEBASE_H
#define HNODEBASE_H

#include <QColor>
#include <QJsonObject>
#include <QRectF>
#include <QUuid>

#include "../HBoard_global.h"
class QSGNode;
class HBoard;
class QSGGeometryNode;
class HBOARD_EXPORT HNodeBase {
 public:
  typedef enum {
    POLY,      //距离判断（按照点与轮廓的最近距离判断）
    RECTANGLE  //点在封闭形状内的判断
  } NODETYPE;  //点选的判断方式

 public:
  HNodeBase();
  HNodeBase(const HNodeBase& other) = default;
  virtual ~HNodeBase();
  HNodeBase& operator=(const HNodeBase& other) = default;

  virtual QSGNode* get() { return nullptr; }
  virtual QSGNode* build(HBoard*) { return nullptr; }
  virtual QRectF getBoundRect() { return QRectF(); }
  virtual QList<QPointF> getPointList();
  virtual QUuid id();
  virtual void changedSelectStatus();
  virtual void move(const QPointF&);
  virtual void moveTo(const QPointF&) {}  // move to point
  virtual bool isSelect() { return _select; }
  virtual void drawPoints(const QList<QPointF>&) {}
  virtual void setColor(const QColor&) {}
  virtual NODETYPE nodeType() { return POLY; }
  virtual void setVisible(bool flag);
  virtual bool visible();
  virtual void timeOut();

 public:
  QJsonObject param();
  void setParam(const QJsonObject& p);

 protected:
  QJsonObject _param;
  QUuid _id;
  bool _select;
  bool _visible;
  QList<QPointF> _dash_list;
  QSGGeometryNode* _dash;
};

#endif  // HNODEBASE_H
