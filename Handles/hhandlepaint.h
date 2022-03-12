#ifndef HHANDLEPAINT_H
#define HHANDLEPAINT_H

#include "hhandlebase.h"

class HBOARD_EXPORT HHandleDrawRect : public HHandleBase
{
public:
    HHandleDrawRect();

    virtual void mousePressEvent(HBoard* board, QMouseEvent *event) override;
    virtual void mouseMoveEvent(HBoard* board, QMouseEvent *event) override;
    virtual void mouseReleaseEvent(HBoard* board, QMouseEvent *event) override;
    virtual void wheelEvent(HBoard* board, QWheelEvent *event) override;
public:

};

#endif // HHANDLEPAINT_H
