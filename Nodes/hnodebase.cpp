#include "hnodebase.h"
#include <QSGNode>
#include <QDebug>
#define DEBUG qDebug() << __FUNCTION__ << " " << __LINE__ << " "
HNodeBase::HNodeBase():_select(false)
{
    _id = QUuid::createUuid();
    DEBUG << "build node " << _id;
}

HNodeBase::~HNodeBase()
{

}

QUuid HNodeBase::id()
{
    return _id;
}

QJsonObject HNodeBase::param()
{
    return _param;
}

void HNodeBase::setParam(const QJsonObject &p)
{
    _param = p;
}
