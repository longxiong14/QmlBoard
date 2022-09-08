﻿#ifndef HHANDLEARROW_H
#define HHANDLEARROW_H
#include <memory>

#include "hhandlemove.h"
class HNodeBase;
class HDragNode;
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
  virtual void hoverEnterEvent(
      HBoard* board, QHoverEvent* event,
      const QJsonObject& object = QJsonObject()) override;
  virtual void hoverMoveEvent(
      HBoard* board, QHoverEvent* event,
      const QJsonObject& object = QJsonObject()) override;

 public:
  void setDistance(int dis);
  int getDistance();

 protected:
  virtual bool canSelect(HNodeBase* node, const QPointF& pos, double scale);

 protected:
  bool _can_move;
  bool _moved;
  int _distance;
  QUuid _select_node;
  QPointF _point;
  QPointF _select_start_point;
  HDragNode* _drag_node;
};

#endif  // HHANDLEARROW_H
