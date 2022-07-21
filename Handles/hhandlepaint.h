#ifndef HHANDLEPAINT_H
#define HHANDLEPAINT_H

#include <QList>
#include <QPoint>
#include <QUuid>

#include "hhandlebase.h"
#include "hhandlemove.h"
class HBOARD_EXPORT HHandleDrawRect : public HHandleMove {
 public:
  HHandleDrawRect();

  virtual void mousePressEvent(HBoard* board, QMouseEvent* event) override;
  virtual void mouseMoveEvent(HBoard* board, QMouseEvent* event) override;
  virtual void mouseReleaseEvent(HBoard* board, QMouseEvent* event) override;

 protected:
  QPoint _point;
};

class HBOARD_EXPORT HHandleDrawPoly : public HHandleMove {
 public:
  HHandleDrawPoly();

  virtual void mousePressEvent(HBoard* board, QMouseEvent* event) override;
  virtual void mouseMoveEvent(HBoard* board, QMouseEvent* event) override;
  virtual void mouseReleaseEvent(HBoard* board, QMouseEvent* event) override;

 protected:
  QList<QPoint> _points;
};

class HBOARD_EXPORT HHandleDrawLine : public HHandleMove {
 public:
  HHandleDrawLine();

  virtual void mousePressEvent(HBoard* board, QMouseEvent* event) override;
  virtual void mouseMoveEvent(HBoard* board, QMouseEvent* event) override;
  virtual void mouseReleaseEvent(HBoard* board, QMouseEvent* event) override;

 protected:
  QPoint _point;
};

class HBOARD_EXPORT HHandleDrawCurve : public HHandleMove {
 public:
  HHandleDrawCurve();

  virtual void mousePressEvent(HBoard* board, QMouseEvent* event) override;
  virtual void mouseMoveEvent(HBoard* board, QMouseEvent* event) override;
  virtual void mouseReleaseEvent(HBoard* board, QMouseEvent* event) override;

 protected:
  QList<QPoint> _points;
};

class HBOARD_EXPORT HHandleDrawFillRect : public HHandleDrawRect {
 public:
  HHandleDrawFillRect();

  virtual void mousePressEvent(HBoard* board, QMouseEvent* event) override;
};

class HBOARD_EXPORT HHandleDrawFillPoly : public HHandleDrawPoly {
 public:
  HHandleDrawFillPoly();

  virtual void mousePressEvent(HBoard* board, QMouseEvent* event) override;
};

#endif  // HHANDLEPAINT_H
