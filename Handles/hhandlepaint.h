#ifndef HHANDLEPAINT_H
#define HHANDLEPAINT_H

#include <QList>
#include <QPoint>
#include <QUuid>

#include "hhandlebase.h"
class HBOARD_EXPORT HHandleDrawRect : public HHandleBase {
 public:
  HHandleDrawRect();

  virtual void mousePressEvent(HBoard* board, QMouseEvent* event) override;
  virtual void mouseMoveEvent(HBoard* board, QMouseEvent* event) override;
  virtual void mouseReleaseEvent(HBoard* board, QMouseEvent* event) override;
  virtual void wheelEvent(HBoard* board, QWheelEvent* event) override;

 protected:
  QPoint _point;
};

class HBOARD_EXPORT HHandleDrawPoly : public HHandleBase {
 public:
  HHandleDrawPoly();

  virtual void mousePressEvent(HBoard* board, QMouseEvent* event) override;
  virtual void mouseMoveEvent(HBoard* board, QMouseEvent* event) override;
  virtual void mouseReleaseEvent(HBoard* board, QMouseEvent* event) override;
  virtual void wheelEvent(HBoard* board, QWheelEvent* event) override;

 protected:
  QList<QPoint> _points;
};

class HBOARD_EXPORT HHandleDrawLine : public HHandleBase {
 public:
  HHandleDrawLine();

  virtual void mousePressEvent(HBoard* board, QMouseEvent* event) override;
  virtual void mouseMoveEvent(HBoard* board, QMouseEvent* event) override;
  virtual void mouseReleaseEvent(HBoard* board, QMouseEvent* event) override;
  virtual void wheelEvent(HBoard* board, QWheelEvent* event) override;

 protected:
  QPoint _point;
};

class HBOARD_EXPORT HHandleDrawCurve : public HHandleBase {
 public:
  HHandleDrawCurve();

  virtual void mousePressEvent(HBoard* board, QMouseEvent* event) override;
  virtual void mouseMoveEvent(HBoard* board, QMouseEvent* event) override;
  virtual void mouseReleaseEvent(HBoard* board, QMouseEvent* event) override;
  virtual void wheelEvent(HBoard* board, QWheelEvent* event) override;

 protected:
  QList<QPoint> _points;
};

#endif  // HHANDLEPAINT_H
