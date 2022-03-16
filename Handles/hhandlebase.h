#ifndef HHANDLEBASE_H
#define HHANDLEBASE_H

#include "../HBoard_global.h"
class QMouseEvent;
class HBoard;
class QWheelEvent;
class HBOARD_EXPORT HHandleBase
{
public:
    HHandleBase();
    virtual ~HHandleBase();
    HHandleBase(const HHandleBase& other) = default;
    HHandleBase& operator=(const HHandleBase& other) = default;

    virtual void mousePressEvent(HBoard* board, QMouseEvent *event) ;
    virtual void mouseMoveEvent(HBoard* board, QMouseEvent *event) ;
    virtual void mouseReleaseEvent(HBoard* board, QMouseEvent *event) ;
    virtual void wheelEvent(HBoard* board, QWheelEvent *event) ;
};

#endif // HHANDLEBASE_H
