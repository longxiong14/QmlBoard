#ifndef HHANDLEMOVE_H
#define HHANDLEMOVE_H
#include <QPoint>
#include <QTransform>
#include "hhandlebase.h"
class HBoard;
class HBOARD_EXPORT HHandleMove : public HHandleBase
{
public:
    HHandleMove();

    virtual void mousePressEvent(HBoard* board, QMouseEvent *event) override;
    virtual void mouseMoveEvent(HBoard* board, QMouseEvent *event) override;
    virtual void mouseReleaseEvent(HBoard* board, QMouseEvent *event) override;
    virtual void wheelEvent(HBoard *board, QWheelEvent *event) override;
private:
    QPoint _last_point;
    QTransform _last_trans;
    double _scale;
};

#endif // HHANDLEMOVE_H
