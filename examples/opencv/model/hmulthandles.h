#ifndef HMULTHANDLES_H
#define HMULTHANDLES_H

#include "../../Handles/hhandlemove.h"
class HMultHandles : public HHandleMove {
 public:
  HMultHandles();

  virtual void mouseMoveEvent(
      HBoard *board, QMouseEvent *event,
      const QJsonObject &object = QJsonObject()) override;

  virtual void wheelEvent(HBoard *board, QWheelEvent *event) override;
};

#endif  // HMULTHANDLES_H
