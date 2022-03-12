#ifndef HHANDLEARROW_H
#define HHANDLEARROW_H
#include <memory>
#include "hhandlemove.h"
class HBOARD_EXPORT HHandleArrow : public HHandleMove
{
public:
    HHandleArrow();

    virtual void mousePressEvent(HBoard* board, QMouseEvent *event) ;
    virtual void mouseMoveEvent(HBoard* board, QMouseEvent *event) ;
    virtual void mouseReleaseEvent(HBoard* board, QMouseEvent *event) ;
    virtual void wheelEvent(HBoard* board, QWheelEvent *event) ;

private:

};

#endif // HHANDLEARROW_H
