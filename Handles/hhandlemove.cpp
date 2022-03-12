#include "hhandlemove.h"

#include <QSGTransformNode>
#include <QMouseEvent>

#include "hboard.h"

#define DEBUG qDebug() << __FUNCTION__ << " " << __LINE__ << " "
HHandleMove::HHandleMove(): _scale(0.05)
{

}

void HHandleMove::mousePressEvent(HBoard *board, QMouseEvent *event)
{
    if(board && event) {
        _last_point = board->WCS2LCS(event->pos()) - board->WCS2LCS(QPoint(0,0));
        _last_trans = board->transform();
    }
}

void HHandleMove::mouseMoveEvent(HBoard *board, QMouseEvent *event)
{
    if(board && event){
        QPointF ds = board->WCS2LCS(event->pos())
                - board->WCS2LCS(QPoint(0,0));
        QTransform trans;
        trans.translate(ds.x() - _last_point.x(), ds.y() - _last_point.y());
        board->pushTransform(trans * _last_trans);
    }
}

void HHandleMove::mouseReleaseEvent(HBoard *board, QMouseEvent *event)
{
    if(board && event){
        _last_point = QPoint();
        _last_trans = QTransform();
    }
}

void HHandleMove::wheelEvent(HBoard *board, QWheelEvent *event)
{
    if(board && event){
        auto dlt = event->delta();
        auto pos = board->WCS2LCS(event->pos());
        auto trans = board->transformNode()->matrix().toTransform();
        double scale = 1.0;
        if(dlt < 0) {
            scale -= _scale;
        }else{
            scale += _scale;
        }
        trans.translate(pos.x(), pos.y());
        trans.scale(scale, scale);
        trans.translate(-pos.x(), -pos.y());
        board->pushTransform(trans);
    }
}


