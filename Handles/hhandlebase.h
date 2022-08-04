#ifndef HHANDLEBASE_H
#define HHANDLEBASE_H

#include <QJsonObject>
#include <QMouseEvent>
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

  virtual void mousePressEvent(HBoard* board, QMouseEvent* event,
                               const QJsonObject& object = QJsonObject());
  virtual void mouseMoveEvent(HBoard* board, QMouseEvent* event,
                              const QJsonObject& object = QJsonObject());
  virtual void mouseReleaseEvent(HBoard* board, QMouseEvent* event,
                                 const QJsonObject& object = QJsonObject());
  virtual void wheelEvent(HBoard* board, QWheelEvent* event);

 protected:
  bool middleButtonPress(QMouseEvent* event);
  bool leftButtonPress(QMouseEvent* event);
  bool ctrlKeyPress(const QSet<int>& keys);

 protected:
  QUuid _node;
  QSet<Qt::MouseButton> _buttons;
};

#endif  // HHANDLEBASE_H
