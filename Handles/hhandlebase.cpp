#include "hhandlebase.h"

#include <QMouseEvent>

#include "hboard.h"
HHandleBase::HHandleBase() {}

HHandleBase::~HHandleBase() {}

void HHandleBase::mousePressEvent(HBoard *, QMouseEvent *) {}

void HHandleBase::mouseMoveEvent(HBoard *, QMouseEvent *) {}

void HHandleBase::mouseReleaseEvent(HBoard *, QMouseEvent *) {}

void HHandleBase::wheelEvent(HBoard *, QWheelEvent *) {}

bool HHandleBase::middleButtonPress(QMouseEvent *event) {
  return (Qt::MouseButton::MiddleButton ==
          (Qt::MouseButton::MiddleButton & event->buttons()));
}
