#include "hrectnode.h"
#include <QSGSimpleRectNode>
#include <QSGRectangleNode>
#include <QRectF>
#include <QSGRenderNode>
#include <QDebug>
#define DEBUG qDebug() << __FUNCTION__ << " " << __LINE__ << " "
HRectNode::HRectNode(const QRect &rect, const QColor &color)
{
    setRect(rect);
    setColor(color);
}

QSGNode *HRectNode::get()
{
    return this;
}

QSGNode *HRectNode::build(HBoard *)
{
    return this;
}

QRect HRectNode::getBoundRect()
{
    auto r = rect();
    auto rect = QRect(static_cast<int>(r.x()),
                      static_cast<int>(r.y()),
                      static_cast<int>(r.width()),
                      static_cast<int>(r.height()));
    return rect ;
}

void HRectNode::move(const QPoint &p)
{
    auto tl = rect().topLeft() + p;
    DEBUG << tl;
    setRect(QRectF(tl, rect().size()));
}

void HRectNode::moveTo(const QPoint &p)
{
    auto r = rect();
    r.setX(p.x());
    r.setY(p.y());
    setRect(r);
    DEBUG << rect();
}

void HRectNode::changedSelectStatus()
{
    DEBUG << id();
    HNodeBase::changedSelectStatus();
    if(_select){
        setColor(Qt::GlobalColor::blue);
    }else{
        setColor(Qt::GlobalColor::red);
    }
}

void HRectNode::drawPoints(const QList<QPoint> & points)
{
    if(2 == points.size()){
        setRect(QRect(points[0], points[1]));
    }
}

