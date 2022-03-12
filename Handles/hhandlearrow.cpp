#include "hhandlearrow.h"
#include "hhandlemove.h"
#include "hboard.h"
#include "Nodes/hnodebase.h"
#include <QDebug>
#define DEBUG qDebug() << __FUNCTION__ << " " << __LINE__ << " "
static bool pointInRect(const QPoint& point, const QRect& rect){
    if(rect.x() <= point.x() &&
            rect.y() <= point.y() &&
            rect.bottomRight().x() >= point.x()
            && rect.bottomRight().y() >= point.y()){
        return true;
    }
    return false;
}

HHandleArrow::HHandleArrow() :HHandleMove()
{

}

void HHandleArrow::mousePressEvent(HBoard *board, QMouseEvent *event)
{
    HHandleMove::mousePressEvent(board, event);
}

void HHandleArrow::mouseMoveEvent(HBoard *board, QMouseEvent *event)
{
    if(board && event){
        auto selects = board->selects();
        DEBUG << selects;
        if(!selects.empty()){
            auto nodes = board->nodes();
            auto pos = board->WCS2LCS(event->pos());
            for(const auto & s : selects){
                auto node = nodes[s];
                if(nodes.contains(s)){
                    if(pointInRect(pos, node->getBoundRect())){
                        auto dlt = pos - _last_point - board->WCS2LCS(QPoint());
                        board->moveNode(node->id(), dlt);
                    }else{
                        DEBUG << pos << " not in " << node->getBoundRect();
                    }
                }else{
                    DEBUG << nodes.keys() << " " << s;
                }
            }
            _last_point = pos - board->WCS2LCS(QPoint(0,0));
        }else{
            HHandleMove::mouseMoveEvent(board, event);
        }
    }else{
        HHandleMove::mouseMoveEvent(board, event);
    }
}

void HHandleArrow::mouseReleaseEvent(HBoard *board, QMouseEvent *event)
{
    if(board && event){
        auto pos = board->WCS2LCS(event->pos());
        auto nodes = board->nodes();
        board->clearSelect();
        for(const auto& n : nodes){
            auto r = n->getBoundRect();
            if(pointInRect(pos,r)){
                board->changeSelectStatus(n->id());
                DEBUG << n->id() << r;
            }
        }
    }
    HHandleMove::mouseReleaseEvent(board, event);
}

void HHandleArrow::wheelEvent(HBoard *board, QWheelEvent *event)
{
    HHandleMove::wheelEvent(board, event);
}
