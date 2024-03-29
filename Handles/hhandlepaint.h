﻿#ifndef HHANDLEPAINT_H
#define HHANDLEPAINT_H

#include <QList>
#include <QPointF>
#include <QUuid>

#include "hhandlebase.h"
#include "hhandlemove.h"

class HBOARD_EXPORT HHandleDrawRect : public HHandleMove {
 public:
  HHandleDrawRect();

  virtual void mousePressEvent(
      HBoard* board, QMouseEvent* event,
      const QJsonObject& object = QJsonObject()) override;
  virtual void mouseMoveEvent(
      HBoard* board, QMouseEvent* event,
      const QJsonObject& object = QJsonObject()) override;

  virtual QJsonObject getDefaultParam() override;

 protected:
  QPointF _point;
};

class HBOARD_EXPORT HHandleDrawLine : public HHandleMove {
 public:
  HHandleDrawLine();

  virtual void mousePressEvent(
      HBoard* board, QMouseEvent* event,
      const QJsonObject& object = QJsonObject()) override;
  virtual void mouseMoveEvent(
      HBoard* board, QMouseEvent* event,
      const QJsonObject& object = QJsonObject()) override;

  virtual QJsonObject getDefaultParam() override;

 protected:
  QPointF _point;
};

class HBOARD_EXPORT HHandleDrawCurve : public HHandleMove {
 public:
  HHandleDrawCurve();

  virtual void mousePressEvent(
      HBoard* board, QMouseEvent* event,
      const QJsonObject& object = QJsonObject()) override;

  virtual void hoverMoveEvent(
      HBoard* board, QHoverEvent* event,
      const QJsonObject& object = QJsonObject()) override;

  virtual void mouseReleaseEvent(
      HBoard* board, QMouseEvent* event,
      const QJsonObject& object = QJsonObject()) override;

  virtual void boardLeaveOffThisHandle(HBoard* board) override;

  virtual QJsonObject getDefaultParam() override;

 protected:
  void leave(HBoard* board);

 protected:
  int _size;
};

class HBOARD_EXPORT HHandleDrawPoly : public HHandleMove {
 public:
  HHandleDrawPoly();

  virtual void mousePressEvent(
      HBoard* board, QMouseEvent* event,
      const QJsonObject& object = QJsonObject()) override;

  virtual void hoverMoveEvent(
      HBoard* board, QHoverEvent* event,
      const QJsonObject& object = QJsonObject()) override;

  virtual void mouseReleaseEvent(
      HBoard* board, QMouseEvent* event,
      const QJsonObject& object = QJsonObject()) override;

  virtual void boardLeaveOffThisHandle(HBoard* board) override;

  virtual QJsonObject getDefaultParam() override;

 protected:
  int _size;
};

class HBOARD_EXPORT HHandleDrawFillRect : public HHandleDrawRect {
 public:
  HHandleDrawFillRect();

  virtual void mousePressEvent(
      HBoard* board, QMouseEvent* event,
      const QJsonObject& object = QJsonObject()) override;
};

class HBOARD_EXPORT HHandleDrawFillPoly : public HHandleDrawPoly {
 public:
  HHandleDrawFillPoly();

  virtual void boardLeaveOffThisHandle(HBoard* board) override;
};

class HBOARD_EXPORT HHandleDrawCircle : public HHandleMove {
 public:
  HHandleDrawCircle();

  virtual void hoverMoveEvent(
      HBoard* board, QHoverEvent* event,
      const QJsonObject& object = QJsonObject()) override;

  virtual void mouseMoveEvent(
      HBoard* board, QMouseEvent* event,
      const QJsonObject& object = QJsonObject()) override;

  virtual void mousePressEvent(
      HBoard* board, QMouseEvent* event,
      const QJsonObject& object = QJsonObject()) override;

  virtual void boardLeaveOffThisHandle(HBoard* board) override;

  virtual void hoverLeaveEvent(
      HBoard* board, QHoverEvent* event,
      const QJsonObject& object = QJsonObject()) override;

  virtual QJsonObject getDefaultParam() override;

 protected:
  virtual void updateCirclePosition(HBoard* board, const QPointF& center,
                                    const QJsonObject& object);

 protected:
  QUuid _circle_node;
};

class HBOARD_EXPORT HHandleDrawFillCircle : public HHandleDrawCircle {
 public:
  HHandleDrawFillCircle();

  virtual void mousePressEvent(
      HBoard* board, QMouseEvent* event,
      const QJsonObject& object = QJsonObject()) override;

 protected:
  virtual void updateCirclePosition(HBoard* board, const QPointF& center,
                                    const QJsonObject& object) override;
};

class HBOARD_EXPORT HHandleDrawEllipse : public HHandleMove {
 public:
  HHandleDrawEllipse();

  virtual void mousePressEvent(
      HBoard* board, QMouseEvent* event,
      const QJsonObject& object = QJsonObject()) override;

  virtual void mouseMoveEvent(
      HBoard* board, QMouseEvent* event,
      const QJsonObject& object = QJsonObject()) override;

  virtual QJsonObject getDefaultParam() override;

 protected:
  QPointF _point;
};

class HBOARD_EXPORT HHandleDrawFillEllipse : public HHandleMove {
 public:
  HHandleDrawFillEllipse();

  virtual void mousePressEvent(
      HBoard* board, QMouseEvent* event,
      const QJsonObject& object = QJsonObject()) override;

  virtual void mouseMoveEvent(
      HBoard* board, QMouseEvent* event,
      const QJsonObject& object = QJsonObject()) override;

  virtual QJsonObject getDefaultParam() override;

 protected:
  QPointF _point;
};

// class HBOARD_EXPORT HHandleDrawWideLine : public HHandleMove {
// public:
//  HHandleDrawWideLine();

//  virtual void mousePressEvent(
//      HBoard* board, QMouseEvent* event,
//      const QJsonObject& object = QJsonObject()) override;

//  virtual void mouseMoveEvent(
//      HBoard* board, QMouseEvent* event,
//      const QJsonObject& object = QJsonObject()) override;

// protected:
//  QList<QPointF> _points;
//};

#endif  // HHANDLEPAINT_H
