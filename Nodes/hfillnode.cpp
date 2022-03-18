#include "hfillnode.h"
#include <QSGFlatColorMaterial>

HFillNode::HFillNode(const QList<QPoint> &points, const QColor &color, bool fill)
{
    setOurGeometry(points, fill);
    setColor(color);
}

HFillNode::HFillNode(const QRect &rect, const QColor &color, bool fill)
{
    QList<QPoint> list{{rect.left(), rect.top()}, {rect.right(), rect.top()},
                       {rect.right(), rect.bottom()},{rect.left(), rect.bottom()}};
    setOurGeometry(list, fill);
    setColor(color);
}

QSGNode *HFillNode::get()
{
    return this;
}

QSGNode *HFillNode::build(HBoard *)
{
    return this;
}

QRect HFillNode::getBoundRect()
{
    QRect r;
    auto geo = geometry();
    if(!geo) return r;
    auto count = geo->vertexCount();
    auto point_list = static_cast<QSGGeometry::Point2D*>(geo->vertexData());
    int left = INT_MAX,
            right = INT_MIN,
            bottom = INT_MIN,
            top = INT_MAX;
    for(int i = 0; i < count; i++){
        auto p = point_list[i];
        left = std::min(static_cast<int>(p.x), left);
        right = std::max(static_cast<int>(p.x), right);
        bottom = std::max(static_cast<int>(p.y), bottom);
        top = std::min(static_cast<int>(p.y), top);
    }
    r = QRect(left,top,right-left,bottom-top);
    return  r;
}

void HFillNode::move(const QPoint &p)
{
    auto geo = geometry();
    if(!geo) return;
    auto count = geo->vertexCount();
    auto point_list = static_cast<QSGGeometry::Point2D*>(geo->vertexData());
    for(int i = 0; i < count; i++){
        QSGGeometry::Point2D pt = point_list[i];
        pt.set(pt.x + p.x(), pt.y + p.y());
        point_list[i] = pt;
        geo->vertexDataAsPoint2D()[i] = pt;
    }
    setGeometry(geo);
}

void HFillNode::moveTo(const QPoint &p)
{

}

void HFillNode::changedSelectStatus()
{
    HNodeBase::changedSelectStatus();
    if(_select){
        setColor(Qt::GlobalColor::red);
    }
}

void HFillNode::drawPoints(const QList<QPoint> &points)
{

}

void HFillNode::setColor(const QColor &color)
{
    QSGFlatColorMaterial *material = new QSGFlatColorMaterial;
    material->setColor(color);
    setMaterial(material);
    setFlag(QSGNode::OwnsMaterial);
}

QSGGeometry *HFillNode::buildGeometry(const QList<QPoint> &points, bool fill)
{
    QSGGeometry *geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), points.size());
    if(fill)
        geometry->setDrawingMode(GL_POLYGON);
    else
        geometry->setDrawingMode(GL_LINE_LOOP);
    for(int i = 0; i < points.size(); i++){
        geometry->vertexDataAsPoint2D()[i].set(points[i].x(), points[i].y());
    }
    return geometry;
}

void HFillNode::setOurGeometry(const QList<QPoint> &points, bool fill)
{
    QSGGeometry *geometry = buildGeometry(points, fill);
    setGeometry(geometry);
    setFlag(QSGNode::OwnsGeometry);
}


