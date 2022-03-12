#include "hhandlebase.h"

#include <QMouseEvent>
#include "hboard.h"
HHandleBase::HHandleBase()
{

}

HHandleBase::~HHandleBase()
{

}

void HHandleBase::mousePressEvent(HBoard *board, QMouseEvent *event)
{
    (void)event;
    (void)board;
}

void HHandleBase::mouseMoveEvent(HBoard* board, QMouseEvent *event)
{
    (void)event;
    (void)board;
}

void HHandleBase::mouseReleaseEvent(HBoard* board, QMouseEvent *event)
{
    (void)event;
    (void)board;
}

void HHandleBase::wheelEvent(HBoard *board, QWheelEvent *event)
{
    (void)event;
    (void)board;
}
