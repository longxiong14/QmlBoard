#include "hhandlebase.h"

#include <QMouseEvent>

#include "hboard.h"
#define DEBUG qDebug() << __FUNCTION__ << " " << __LINE__ << " "
HHandleBase::HHandleBase() {}

HHandleBase::~HHandleBase() {}

void HHandleBase::mousePressEvent(HBoard *, QMouseEvent *,
                                  const QJsonObject &) {}

void HHandleBase::mouseMoveEvent(HBoard *, QMouseEvent *, const QJsonObject &) {
}

void HHandleBase::mouseReleaseEvent(HBoard *, QMouseEvent *,
                                    const QJsonObject &) {}

void HHandleBase::wheelEvent(HBoard *, QWheelEvent *) {}

bool HHandleBase::middleButtonPress(QMouseEvent *event) {
  bool flag = (Qt::MouseButton::MiddleButton ==
               (Qt::MouseButton::MiddleButton & event->buttons()));
  return flag;
}

bool HHandleBase::leftButtonPress(QMouseEvent *event) {
  bool flag = (Qt::MouseButton::LeftButton ==
               (Qt::MouseButton::LeftButton & event->buttons()));
  return flag;
}

bool HHandleBase::ctrlKeyPress(const QSet<int> &keys) {
  return keys.contains(Qt::Key::Key_Control);
}
