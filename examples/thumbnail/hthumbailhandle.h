#ifndef HTHUMBAILHANDLE_H
#define HTHUMBAILHANDLE_H

#include "../../Handles/hhandlemove.h"
class HThumbailHandle : public HHandleMove {
 public:
  HThumbailHandle();

  virtual void mousePressEvent(
      HBoard* board, QMouseEvent* event,
      const QJsonObject& object = QJsonObject()) override;
  virtual void mouseMoveEvent(
      HBoard* board, QMouseEvent* event,
      const QJsonObject& object = QJsonObject()) override;
  virtual void mouseReleaseEvent(
      HBoard* board, QMouseEvent* event,
      const QJsonObject& object = QJsonObject()) override;

 private:
  QString _dst_board;
  double _src_scale;
};

#endif  // HTHUMBAILHANDLE_H
