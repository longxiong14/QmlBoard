#ifndef HHANDLEPAINT_H
#define HHANDLEPAINT_H

#include "hhandlebase.h"
#include <QPoint>
#include <QUuid>
class HBOARD_EXPORT HHandleDrawRect : public HHandleBase
{
public:
    HHandleDrawRect();

    virtual void mousePressEvent(HBoard* board, QMouseEvent *event) override;
    virtual void mouseMoveEvent(HBoard* board, QMouseEvent *event) override;
    virtual void mouseReleaseEvent(HBoard* board, QMouseEvent *event) override;
    virtual void wheelEvent(HBoard* board, QWheelEvent *event) override;
protected:
    QPoint _point;
    QUuid _node;
};

class HBOARD_EXPORT HHandleDrawPoly : public HHandleBase
{
public:
    HHandleDrawPoly();

    virtual void mousePressEvent(HBoard* board, QMouseEvent *event) override;
    virtual void mouseMoveEvent(HBoard* board, QMouseEvent *event) override;
    virtual void mouseReleaseEvent(HBoard* board, QMouseEvent *event) override;
    virtual void wheelEvent(HBoard* board, QWheelEvent *event) override;
protected:
    QPoint _point;
    QUuid _node;
};

#endif // HHANDLEPAINT_H
