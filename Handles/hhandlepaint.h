#ifndef HHANDLEPAINT_H
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

class HBOARD_EXPORT HHandleDrawPoly : public HHandleMove {
 public:
  HHandleDrawPoly();

  virtual void mousePressEvent(
      HBoard* board, QMouseEvent* event,
      const QJsonObject& object = QJsonObject()) override;
  virtual void mouseMoveEvent(
      HBoard* board, QMouseEvent* event,
      const QJsonObject& object = QJsonObject()) override;
  virtual void mouseReleaseEvent(
      HBoard* board, QMouseEvent* event,
      const QJsonObject& object = QJsonObject()) override;

  virtual QJsonObject getDefaultParam() override;

 protected:
  QList<QPointF> _points;
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
  virtual void mouseMoveEvent(
      HBoard* board, QMouseEvent* event,
      const QJsonObject& object = QJsonObject()) override;
  virtual void mouseReleaseEvent(
      HBoard* board, QMouseEvent* event,
      const QJsonObject& object = QJsonObject()) override;

  virtual QJsonObject getDefaultParam() override;

 protected:
  QList<QPointF> _points;
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

  virtual void mousePressEvent(
      HBoard* board, QMouseEvent* event,
      const QJsonObject& object = QJsonObject()) override;
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

  virtual void hoverLeaveEvent(
      HBoard* board, QHoverEvent* event,
      const QJsonObject& object = QJsonObject()) override;

  virtual QJsonObject getDefaultParam() override;

 protected:
  virtual void updateCirclePosition(HBoard* board, const QPointF& center,
                                    const QJsonObject& object);
};

class HBOARD_EXPORT HHandleDrawFillCircle : public HHandleDrawCircle {
 public:
  HHandleDrawFillCircle();

 protected:
  virtual void updateCirclePosition(HBoard* board, const QPointF& center,
                                    const QJsonObject& object);
};

class HBOARD_EXPORT HHandleDrawWideLine : public HHandleMove {
 public:
  HHandleDrawWideLine();

  virtual void mousePressEvent(
      HBoard* board, QMouseEvent* event,
      const QJsonObject& object = QJsonObject()) override;

  virtual void mouseMoveEvent(
      HBoard* board, QMouseEvent* event,
      const QJsonObject& object = QJsonObject()) override;

 protected:
  QList<QPointF> _points;
};

#endif  // HHANDLEPAINT_H
