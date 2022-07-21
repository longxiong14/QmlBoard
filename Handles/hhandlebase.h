#ifndef HHANDLEBASE_H
#define HHANDLEBASE_H

#include <QUuid>

#include "../HBoard_global.h"
class QMouseEvent;
class HBoard;
class QWheelEvent;
class HBOARD_EXPORT HHandleBase {
 public:
  HHandleBase();
  virtual ~HHandleBase();
  HHandleBase(const HHandleBase& other) = default;
  HHandleBase& operator=(const HHandleBase& other) = default;

  virtual void mousePressEvent(HBoard* board, QMouseEvent* event);
  virtual void mouseMoveEvent(HBoard* board, QMouseEvent* event);
  virtual void mouseReleaseEvent(HBoard* board, QMouseEvent* event);
  virtual void wheelEvent(HBoard* board, QWheelEvent* event);

 protected:
  bool middleButtonPress(QMouseEvent* event);
  bool leftButtonPress(QMouseEvent* event);
  bool ctrlKeyPress(const QSet<int>& keys);

 protected:
  QUuid _node;
};

#endif  // HHANDLEBASE_H
