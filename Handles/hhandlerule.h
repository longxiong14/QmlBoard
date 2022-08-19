#ifndef HHANDLERULE_H
#define HHANDLERULE_H

#include <QList>
#include <QPointF>
#include <QUuid>

#include "hhandlebase.h"
#include "hhandlemove.h"
class HBOARD_EXPORT HHandleRule : public HHandleMove {
 public:
  HHandleRule();

  virtual void mousePressEvent(
      HBoard *board, QMouseEvent *event,
      const QJsonObject &object = QJsonObject()) override;

  virtual void mouseMoveEvent(
      HBoard *board, QMouseEvent *event,
      const QJsonObject &object = QJsonObject()) override;

  virtual QJsonObject getDefaultParam() override;

 protected:
  QPointF _point;
};

#endif  // HHANDLERULE_H
