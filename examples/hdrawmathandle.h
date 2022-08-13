#ifndef HDRAWMATHANDLE_H
#define HDRAWMATHANDLE_H
#include "../Handles/hhandlebase.h"
#include "../Handles/hhandlemove.h"
class HDrawMatHandle : public HHandleMove {
public:
  HDrawMatHandle();

  virtual void
  mousePressEvent(HBoard *board, QMouseEvent *event,
                  const QJsonObject &object = QJsonObject()) override;
};

#endif // HDRAWMATHANDLE_H
