#include "hhandlebase.h"

#include <QMouseEvent>

#include "hboard.h"
#define DEBUG qDebug() << __FUNCTION__ << " " << __LINE__ << " "
HHandleBase::HHandleBase() : _name("base") {}

HHandleBase::~HHandleBase() {}

void HHandleBase::mousePressEvent(HBoard *, QMouseEvent *event,
                                  const QJsonObject &) {
  _buttons.insert(event->button());
}

void HHandleBase::mouseMoveEvent(HBoard *, QMouseEvent *, const QJsonObject &) {
}

void HHandleBase::mouseReleaseEvent(HBoard *, QMouseEvent *e,
                                    const QJsonObject &) {
  _buttons.remove(e->button());
}

void HHandleBase::wheelEvent(HBoard *, QWheelEvent *) {}

QJsonObject HHandleBase::getDefaultParam() { return QJsonObject(); }

QString HHandleBase::getName() { return _name; }

bool HHandleBase::middleButtonPress(QMouseEvent *) {
  return _buttons.contains(Qt::MouseButton::MidButton);
}

bool HHandleBase::leftButtonPress(QMouseEvent *event) {
  bool flag = (Qt::MouseButton::LeftButton ==
               (Qt::MouseButton::LeftButton & event->buttons()));
  return flag;
}

bool HHandleBase::ctrlKeyPress(const QSet<int> &keys) {
  return keys.contains(Qt::Key::Key_Control);
}
