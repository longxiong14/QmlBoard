#include "hhandlepaint.h"
#include "hboard.h"
#include <QRect>
#include <QColor>
#include "Nodes/hfillnode.h"
#include "Common/hcommons.h"
#define DEBUG qDebug() << __FUNCTION__ << " " << __LINE__ << " "
HHandleDrawRect::HHandleDrawRect():HHandleBase()
{

}

void HHandleDrawRect::mousePressEvent(HBoard *board, QMouseEvent *event)
{
    DEBUG << "mouse press";
    if(board && event){
        _point = board->WCS2LCS(event->pos());
        auto node = new HFillNode(QRect(_point,QSize(1,1)), Qt::GlobalColor::red);
        board->pushNode(node);
        _node = node->id();
    }
}

void HHandleDrawRect::mouseMoveEvent(HBoard *board, QMouseEvent *event)
{
    if(board && event){
        auto pos = board->WCS2LCS(event->pos());
        QPoint tl = HCommon::TopLeft(pos, _point);
        QPoint br = HCommon::BottomRight(pos,_point);
        QList<QPoint> points{tl,br};
        board->drawNodePoint(_node, points);
    }else{
        DEBUG << "board or event or node is null";
    }
}

void HHandleDrawRect::mouseReleaseEvent(HBoard *board, QMouseEvent *event)
{

}

void HHandleDrawRect::wheelEvent(HBoard *board, QWheelEvent *event)
{

}

HHandleDrawPoly::HHandleDrawPoly()
{

}

void HHandleDrawPoly::mousePressEvent(HBoard *board, QMouseEvent *event)
{

}

void HHandleDrawPoly::mouseMoveEvent(HBoard *board, QMouseEvent *event)
{

}

void HHandleDrawPoly::mouseReleaseEvent(HBoard *board, QMouseEvent *event)
{

}

void HHandleDrawPoly::wheelEvent(HBoard *board, QWheelEvent *event)
{

}
