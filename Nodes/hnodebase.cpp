#include "hnodebase.h"

#include <QDebug>
#include <QSGNode>
#define DEBUG qDebug() << __FUNCTION__ << " " << __LINE__ << " "
HNodeBase::HNodeBase() : _select(false) { _id = QUuid::createUuid(); }

HNodeBase::~HNodeBase() {}

QUuid HNodeBase::id() { return _id; }

QJsonObject HNodeBase::param() { return _param; }

void HNodeBase::setParam(const QJsonObject &p) { _param = p; }
