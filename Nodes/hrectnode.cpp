#include "hrectnode.h"
#include <QSGSimpleRectNode>
#include <QSGRectangleNode>
#include <QRectF>
#include "hselectstyle.h"
#include <QSGRenderNode>
HRectNode::HRectNode(const QRect &rect, const QColor &color)
{
    setRect(rect);
    setColor(color);
    auto rander = new HSelectStyle();
    appendChildNode(rander);
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
