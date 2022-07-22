#ifndef HNODEBASE_H
#define HNODEBASE_H

#include <QColor>
#include <QJsonObject>
#include <QRect>
#include <QUuid>

#include "../HBoard_global.h"
class QSGNode;
class HBoard;
class HBOARD_EXPORT HNodeBase {
 public:
  typedef enum {
    DISTANCE,    //距离判断（按照点与轮廓的最近距离判断）
    INAREA       //点在封闭形状内的判断
  } SELECTTYPE;  //点选的判断方式

 public:
  HNodeBase();
  HNodeBase(const HNodeBase& other) = default;
  virtual ~HNodeBase();
  HNodeBase& operator=(const HNodeBase& other) = default;

  virtual QSGNode* get() { return nullptr; }
  virtual QSGNode* build(HBoard*) { return nullptr; }
  virtual QRect getBoundRect() { return QRect(); }
  virtual QList<QPoint> getPointList();
  virtual QUuid id();
  virtual void changedSelectStatus() { _select = !_select; }
  virtual void move(const QPoint&) {}    // move delta
  virtual void moveTo(const QPoint&) {}  // move to point
  virtual bool isSelect() { return _select; }
  virtual void drawPoints(const QList<QPoint>&) {}
  virtual void setColor(const QColor&) {}
  virtual SELECTTYPE selectType() { return DISTANCE; }

 public:
  QJsonObject param();
  void setParam(const QJsonObject& p);

 protected:
  QJsonObject _param;
  QUuid _id;
  bool _select;
};

#endif  // HNODEBASE_H
