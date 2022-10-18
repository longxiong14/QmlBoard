#ifndef HSHAPENODES_H
#define HSHAPENODES_H

#include <QJsonObject>
#include <QRectF>

#include "../HBoard_global.h"
#include "hfillnode.h"

class HBOARD_EXPORT HShapeLineNode : public HFillNode {
 public:
  HShapeLineNode();
  HShapeLineNode(const QPointF &begin, const QPointF &end,
                 const QJsonObject &param);

  virtual void move(const QPointF &p) override;

  virtual HNodeBase::NODETYPE nodeType() override;

  virtual QSGNode *buildDragNode(HBoard *board) override;

  virtual void updateIndexPoint(int index, const QPointF &point) override;

  //  virtual void updateDragNodePoint(HBoard *board) override;
};

class HBOARD_EXPORT HShapeRectNode : public HFillNode {
 public:
  struct dragNodeMsg {
    QPointF _point;
    Qt::CursorShape _cursor;
  };

 public:
  HShapeRectNode();
  HShapeRectNode(const QRectF &rect, const QJsonObject &param);

  virtual QSGNode *buildDragNode(HBoard *board) override;

  virtual void move(const QPointF &p) override;

  virtual HNodeBase::NODETYPE nodeType() override;

  virtual void updateIndexPoint(int index, const QPointF &point) override;

  virtual int save(QJsonObject &o) override;

  virtual int load(const QJsonObject &o) override;

  virtual void updateDragNodePoint(HBoard *board) override;

 public:
  static void updateDragNodes(QSGNode *drag_node, const QRectF &rect);

  static void createRectDragNode(QSGNode *node, const QRectF &rect,
                                 const QUuid &id, double size);

  static int updateRectDragNode(int index, const QRectF &rect,
                                const QPointF &point, QRectF &out_rect);

  static QMap<int, dragNodeMsg> getRectDragNodeMap(const QRectF &rect);
};

class HBOARD_EXPORT HShapeCurveNode : public HFillNode {
 public:
  HShapeCurveNode();
  HShapeCurveNode(const QList<QPointF> &list, const QJsonObject &param);

  virtual HNodeBase::NODETYPE nodeType() override;

  virtual void move(const QPointF &p) override;

  virtual QSGNode *buildDragNode(HBoard *board) override;

  virtual void updateIndexPoint(int index, const QPointF &point) override;

  //  virtual void updateDragNodePoint(HBoard *board) override;
};

class HBOARD_EXPORT HShapePolyNode : public HFillNode {
 public:
  HShapePolyNode();
  HShapePolyNode(const QList<QPointF> &list, const QJsonObject &param);

  virtual void move(const QPointF &p) override;

  virtual HNodeBase::NODETYPE nodeType() override;

  virtual QSGNode *buildDragNode(HBoard *board) override;

  virtual void updateIndexPoint(int index, const QPointF &point) override;

  //  virtual void updateDragNodePoint(HBoard *board) override;
};

class HBOARD_EXPORT HShapeFillRectNode : public HFillNode {
 public:
  HShapeFillRectNode();
  HShapeFillRectNode(const QRectF &rect, const QJsonObject &param);

  virtual void move(const QPointF &p) override;

  virtual HNodeBase::NODETYPE nodeType() override;

  virtual QSGNode *buildDragNode(HBoard *board) override;

  virtual void updateIndexPoint(int index, const QPointF &point) override;

  virtual int save(QJsonObject &o) override;

  virtual int load(const QJsonObject &o) override;

  virtual void updateDragNodePoint(HBoard *board) override;
};

class HBOARD_EXPORT HShapeCircleNode : public HFillNode {
 public:
  HShapeCircleNode();

  HShapeCircleNode(const QPointF &center, const QJsonObject &param);

  virtual HNodeBase::NODETYPE nodeType() override;

  virtual void setParam(const QJsonObject &p) override;

  virtual int save(QJsonObject &o) override;

  virtual int load(const QJsonObject &o) override;
};

class HBOARD_EXPORT HShapeFillCircleNode : public HFillNode {
 public:
  HShapeFillCircleNode();

  virtual HNodeBase::NODETYPE nodeType() override;

  HShapeFillCircleNode(const QPointF &center, const QJsonObject &param);

  virtual void setParam(const QJsonObject &p) override;

  virtual int save(QJsonObject &o) override;

  virtual int load(const QJsonObject &o) override;
};

class HBOARD_EXPORT HShapeCrossNode : public HFillNode {
 public:
  HShapeCrossNode();

  HShapeCrossNode(const QPointF &center, double size, const QJsonObject &param);

  virtual HNodeBase::NODETYPE nodeType() override;

  virtual int save(QJsonObject &o) override;

  virtual int load(const QJsonObject &o) override;

 protected:
  double _size;
};

class HBOARD_EXPORT HShapeXNode : public HFillNode {
 public:
  HShapeXNode();

  HShapeXNode(const QPointF &center, double size, const QJsonObject &param);

  virtual HNodeBase::NODETYPE nodeType() override;

  virtual int save(QJsonObject &o) override;

  virtual int load(const QJsonObject &o) override;

 protected:
  double _size;
};

class HBOARD_EXPORT HShapeEllipseNode : public HFillNode {
 public:
  HShapeEllipseNode();

  HShapeEllipseNode(const QRectF &rect,
                    const QJsonObject &param = QJsonObject());

  virtual QSGNode *buildDragNode(HBoard *board) override;

  virtual void move(const QPointF &p) override;

  virtual void updateIndexPoint(int index, const QPointF &point) override;

  virtual HNodeBase::NODETYPE nodeType() override;

  virtual void updateDragNodePoint(HBoard *board) override;

  virtual int save(QJsonObject &o) override;

  virtual int load(const QJsonObject &o) override;

 protected:
  double _size;
};

class HBOARD_EXPORT HShapeFillEllipseNode : public HShapeEllipseNode {
 public:
  HShapeFillEllipseNode();

  HShapeFillEllipseNode(const QRectF &rect,
                        const QJsonObject &param = QJsonObject());

  virtual HNodeBase::NODETYPE nodeType() override;

  virtual int load(const QJsonObject &o) override;
};

#endif  // HSHAPENODES_H
