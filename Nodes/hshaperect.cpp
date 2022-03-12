#include "hshaperect.h"
#include <QSGGeometryNode>
#include <QSGFlatColorMaterial>
#include <QDebug>
#include <algorithm>
#define DEBUG qDebug() << __FUNCTION__ << " " << __LINE__ << " "
HShapeRect::HShapeRect(const QRect &rect, const QColor &color, int line_width):
    _rect(rect)
{
    QSGGeometry *geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 4);
    geometry->setDrawingMode(GL_LINE_LOOP);
    geometry->vertexDataAsPoint2D()[0].set(_rect.left(), _rect.top());
    geometry->vertexDataAsPoint2D()[1].set(_rect.right(), _rect.top());
    geometry->vertexDataAsPoint2D()[2].set(_rect.right(), _rect.bottom());
    geometry->vertexDataAsPoint2D()[3].set(_rect.left(), _rect.bottom());
    geometry->setLineWidth(line_width);

    QSGFlatColorMaterial *material = new QSGFlatColorMaterial;
    material->setColor(color);
    setGeometry(geometry);
    setFlag(QSGNode::OwnsGeometry);
    setMaterial(material);
    setFlag(QSGNode::OwnsMaterial);
}

QSGNode *HShapeRect::build(HBoard *)
{
    return this;
}

QRect HShapeRect::getBoundRect()
{
    auto geo = geometry();
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
    QRect r = QRect(left,top,right-left,bottom-top);
    return  r;
}

void HShapeRect::changedSelectStatus()
{
    HNodeBase::changedSelectStatus();
    auto mar = static_cast<QSGFlatColorMaterial*>(material());
    if(mar){
        if(_select){
            mar->setColor(QColor(0,255,0));
        }else{
            mar->setColor(QColor(255,0,0));
        }
        setMaterial(mar);
    }
}

void HShapeRect::move(const QPoint & p)
{
    auto geo = geometry();
    if(geo){
        auto r = getBoundRect();
        auto tl = r.topLeft() + p;
        auto rect = QRect(tl, _rect.size());
        geo->vertexDataAsPoint2D()[0].set(rect.left(), rect.top());
        geo->vertexDataAsPoint2D()[1].set(rect.right(), rect.top());
        geo->vertexDataAsPoint2D()[2].set(rect.right(), rect.bottom());
        geo->vertexDataAsPoint2D()[3].set(rect.left(), rect.bottom());
        setGeometry(geo);
    }
}
