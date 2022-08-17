#ifndef HNODEBASE_H
#define HNODEBASE_H

#include <QColor>
#include <QJsonObject>
#include <QRectF>
#include <QUuid>

#include "../HBoard_global.h"
#include "../Storage/hstoragebase.h"
class QSGNode;
class HBoard;
class QSGGeometryNode;
class HBOARD_EXPORT HNodeBase : public HStorageBase<QJsonObject> {
 public:
  typedef enum {
    SHAPE = 1001,  //距离判断（按照点与轮廓的最近距离判断）
    IMAGE          //点在封闭形状内的判断
  } NODETYPE;      // node type

 public:
  HNodeBase();
  HNodeBase(const HNodeBase &other) = default;
  virtual ~HNodeBase();
  HNodeBase &operator=(const HNodeBase &other) = default;

  virtual QSGNode *get() { return nullptr; }
  virtual QSGNode *build(HBoard *) { return nullptr; }
  virtual QRectF getBoundRect() { return QRectF(); }
  virtual QList<QPointF> getPointList();
  virtual QUuid id();
  virtual void setId(const QUuid &id);
  virtual void changedSelectStatus();
  virtual void move(const QPointF &);
  virtual void moveTo(const QPointF &) {}  // move to point
  virtual bool isSelect() { return _select; }
  virtual void drawPoints(const QList<QPointF> &) {}
  virtual void setColor(const QColor &) {}
  virtual NODETYPE nodeType() { return SHAPE; }
  virtual unsigned long drawingMode();
  virtual void setVisible(bool flag);
  virtual bool visible();
  virtual void timeOut();
  virtual int setText(const QString &text, const QRectF &position,
                      HBoard *board);

  virtual bool enableHome();
  virtual void setEnableHome(bool f);
  virtual void setDestory(bool flag);

 public:
  QJsonObject param();
  virtual void setParam(const QJsonObject &p);

 protected:
  QJsonObject _param;
  QUuid _id;
  bool _select;
  bool _visible;
  QList<QPointF> _dash_list;
  QSGGeometryNode *_dash;
  bool _enable_home;
  QSGNode *_text_node;
  QString _text;
  bool _destory;
};

#endif  // HNODEBASE_H
