﻿#include "hhandlebase.h"

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

void HHandleBase::hoverEnterEvent(HBoard *, QHoverEvent *,
                                  const QJsonObject &) {}

void HHandleBase::hoverMoveEvent(HBoard *, QHoverEvent *, const QJsonObject &) {
}

void HHandleBase::hoverLeaveEvent(HBoard *, QHoverEvent *,
                                  const QJsonObject &) {}

void HHandleBase::wheelEvent(HBoard *, QWheelEvent *) {}

void HHandleBase::boardLeaveOffThisHandle(HBoard *) { _buttons.clear(); }

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

bool HHandleBase::rightButtonPress(QMouseEvent *event) {
  bool flag = (Qt::MouseButton::RightButton ==
               (Qt::MouseButton::RightButton & event->buttons()));
  return flag;
}

bool HHandleBase::isButtonPress(QMouseEvent *event, Qt::MouseButton button) {
  return button == event->button();
}

bool HHandleBase::ctrlKeyPress(const QSet<int> &keys) {
  return keys.contains(Qt::Key::Key_Control);
}
