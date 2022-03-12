#ifndef HNODEBASE_H
#define HNODEBASE_H

#include "hboard.h"
#include "HBoard_global.h"
#include <QJsonObject>
#include <QUuid>
class QSGNode;
class HBoard;
class HBOARD_EXPORT HNodeBase
{
public:
    HNodeBase();
    HNodeBase(const HNodeBase& other) = default;
    virtual ~HNodeBase();
    HNodeBase& operator=(const HNodeBase& other) = default;

    virtual QSGNode* get() {return nullptr;}
    virtual QSGNode* build(HBoard*) {return nullptr;}
    virtual QRect getBoundRect() {return  QRect();}
    virtual QUuid id();
    virtual void changedSelectStatus(){ _select = !_select; }
    virtual void move(const QPoint&){}
    virtual void moveTo(const QPoint& ){}
    virtual bool isSelect(){ return _select; }
public:
    QJsonObject param();
    void setParam(const QJsonObject& p);

protected:
    QJsonObject _param;
    QUuid _id;
    bool _select;
};

#endif // HNODEBASE_H
