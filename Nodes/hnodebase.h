﻿#ifndef HNODEBASE_H
#define HNODEBASE_H
#include <QColor>
#include <QImage>
#include <QJsonObject>
#include <QRectF>
#include <QUuid>

#include "../HBoard_global.h"
#include "../Storage/hstoragebase.h"
class QSGNode;
class HBoard;
class QSGGeometryNode;
class HDragNode;
class HBOARD_EXPORT HNodeBase : public HStorageBase<QJsonObject> {
 public:
  typedef enum {
    SHAPE = 1001,  //距离判断（按照点与轮廓的最近距离判断）
    IMAGE,         //点在封闭形状内的判断
    SHAPELINE,
    SHAPERECT,
    SHAPECURVE,
    SHAPEPOLY,
    SHAPEFILLRECT,
    SHAPECIRCLE,
    SHAPEFILLCIRCLE,
    SHAPECROSS,
    SHAPEXNODE,
    MAPINAGE,  //点在封闭形状内的判断
    SHAPEELLIPSE,
    SHAPEFILLELLIPSE,
    MULTSHAPE  //点在外接矩形内判断
  } NODETYPE;  // node type

  typedef enum {
    CANSELECT = 1,        //能否选取
    CANDESTORY = 1 << 1,  //是否销毁
    VISIBLE = 1 << 2,     // visible
    SELECTED = 1 << 3,    //是否选中
    EABLEHOME = 1 << 4    // home 时参与计算
  } NODEFLAG;

 public:
  HNodeBase();
  HNodeBase(const HNodeBase &other) = default;
  HNodeBase &operator=(const HNodeBase &other) = default;
  virtual ~HNodeBase() override;

  virtual QSGNode *get() { return nullptr; }
  virtual QSGNode *build(HBoard *);
  virtual QRectF getBoundRect() { return QRectF(); }
  virtual QList<QPointF> getPointList();
  virtual QUuid id();
  virtual void setId(const QUuid &id);
  virtual void changedSelectStatus();
  virtual void move(const QPointF &);
  virtual void moveTo(const QPointF &);  // move to point
  virtual bool isSelect();
  virtual void drawPoints(const QList<QPointF> &) {}
  virtual void setColor(const QColor &) {}
  virtual NODETYPE nodeType() { return SHAPE; }
  virtual unsigned long drawingMode();
  virtual void updateDrawMode(unsigned long mode);
  virtual void setVisible(bool flag);
  virtual bool visible();
  virtual void timeOut();
  virtual int setText(const QString &text, const QRectF &position = QRectF(),
                      int pixel_size = 10);
  void setZOrder(int z);
  int getZOrder();
  QString getText();
  QRectF getTextRect();
  int getPixelSize();

  virtual bool enableHome();
  virtual void setEnableHome(bool f);
  virtual void setDestory(bool flag);

  virtual void setFlag(NODEFLAG flag, bool open);
  virtual NODEFLAG flag();
  bool canSelect();

  // drag nodes
 public:
  virtual QSGNode *buildDragNode(HBoard *board);
  QSGNode *getDragNode();
  void destroyDragNode();
  virtual bool pointInDragNode(const QPointF &point, HDragNode *&drag,
                               double scale);
  virtual void updateIndexPoint(int index, const QPointF &point);
  virtual void updateDragNodePoint(HBoard *board);

 public:  // image node interface
  // start: relative position
  virtual void updateMat(HBoard *board, const QImage &mat,
                         const QPointF &start);

  // roi: relative rectangle
  virtual void updateRoi(HBoard *board, const QRectF &roi);

 public:
  virtual int save(QJsonObject &d) override;
  virtual int load(const QJsonObject &o) override;
  virtual int save(const QString &path) override;
  virtual int load(const QString &path) override;

 public:
  QJsonObject param();
  virtual void setParam(const QJsonObject &p);

  QJsonObject data();
  virtual void setData(const QJsonObject &d);

  void insertData(const QString &key, const QJsonValue &value);

 public:
  void buildTextNode(HBoard *board);

 protected:
  void flushMayiLine();
  void updateDrawDash(QSGNode *parent, QSGGeometryNode *&dash,
                      QList<QPointF> &dash_list, int line_width);

 protected:
  QJsonObject _param;
  QJsonObject _data;
  QUuid _id;
  QList<QPointF> _dash_list;
  QSGGeometryNode *_dash;
  QSGNode *_text_node;
  QSGNode *_drag_node;
  QRectF _text_rect;
  QString _text;
  int _pixel_size;
  int _flag;
  int _z_order;
};

#endif  // HNODEBASE_H
