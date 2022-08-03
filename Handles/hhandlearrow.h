#ifndef HHANDLEARROW_H
#define HHANDLEARROW_H
#include <memory>

#include "hhandlemove.h"
class HNodeBase;
class HBOARD_EXPORT HHandleArrow : public HHandleMove {
 public:
  HHandleArrow();

  virtual void mousePressEvent(
      HBoard* board, QMouseEvent* event,
      const QJsonObject& object = QJsonObject()) override;
  virtual void mouseMoveEvent(
      HBoard* board, QMouseEvent* event,
      const QJsonObject& object = QJsonObject()) override;
  virtual void mouseReleaseEvent(
      HBoard* board, QMouseEvent* event,
      const QJsonObject& object = QJsonObject()) override;
  virtual void wheelEvent(HBoard* board, QWheelEvent* event) override;

 public:
  void setDistance(int dis);
  int getDistance();

 protected:
  virtual bool canSelect(HNodeBase* node, const QPointF& pos, double scale);

 protected:
  bool _move;
  int _distance;
};

#endif  // HHANDLEARROW_H
