#include "hnodebase.h"

#include <QDebug>
#include <QSGNode>
#define DEBUG qDebug() << __FUNCTION__ << " " << __LINE__ << " "
HNodeBase::HNodeBase() : _select(false) { _id = QUuid::createUuid(); }

HNodeBase::~HNodeBase() {}

QList<QPoint> HNodeBase::getPointList() { return {}; }

QUuid HNodeBase::id() { return _id; }

void HNodeBase::changedSelectStatus() { _select = !_select; }

bool HNodeBase::isSelect() { return _select; }

HNodeBase::SELECTTYPE HNodeBase::selectType() { return DISTANCE; }

bool HNodeBase::visible() { return false; }

QJsonObject HNodeBase::param() { return _param; }

void HNodeBase::setParam(const QJsonObject &p) { _param = p; }
