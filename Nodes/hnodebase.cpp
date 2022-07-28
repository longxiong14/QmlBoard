#include "hnodebase.h"

#include <QDebug>
#include <QSGNode>
#define DEBUG qDebug() << __FUNCTION__ << " " << __LINE__ << " "
HNodeBase::HNodeBase() : _select(false), _visible(true) {
  _id = QUuid::createUuid();
}

HNodeBase::~HNodeBase() {}

QList<QPoint> HNodeBase::getPointList() { return {}; }

QUuid HNodeBase::id() { return _id; }

void HNodeBase::setVisible(bool flag) { _visible = flag; }

bool HNodeBase::visible() { return _visible; }

void HNodeBase::timeOut() {}

QJsonObject HNodeBase::param() { return _param; }

void HNodeBase::setParam(const QJsonObject &p) { _param = p; }
