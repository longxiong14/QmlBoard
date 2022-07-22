#ifndef HHANDLEARROW_H
#define HHANDLEARROW_H
#include <memory>

#include "hhandlemove.h"
class HNodeBase;
class HBOARD_EXPORT HHandleArrow : public HHandleMove {
 public:
  HHandleArrow();

  virtual void mousePressEvent(HBoard* board, QMouseEvent* event);
  virtual void mouseMoveEvent(HBoard* board, QMouseEvent* event);
  virtual void mouseReleaseEvent(HBoard* board, QMouseEvent* event);
  virtual void wheelEvent(HBoard* board, QWheelEvent* event);

 public:
  void setDistance(int dis);
  int getDistance();

 protected:
  virtual bool canSelect(HNodeBase* node, const QPoint& pos, double scale);

 protected:
  bool _move;
  int _distance;
};

#endif  // HHANDLEARROW_H
