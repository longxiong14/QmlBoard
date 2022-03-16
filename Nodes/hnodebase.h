#ifndef HNODEBASE_H
#define HNODEBASE_H

#include "../HBoard_global.h"
#include <QJsonObject>
#include <QUuid>
#include <QRect>
#include <QColor>
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
    virtual void move(const QPoint&){}      //move delta
    virtual void moveTo(const QPoint& ){}   //move to point
    virtual bool isSelect(){ return _select; }
    virtual void drawPoints(const QList<QPoint>& ){}
public:
    QJsonObject param();
    void setParam(const QJsonObject& p);

protected:
    QJsonObject _param;
    QUuid _id;
    bool _select;
};

#endif // HNODEBASE_H
