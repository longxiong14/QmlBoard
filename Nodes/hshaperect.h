#ifndef HSHAPERECT_H
#define HSHAPERECT_H

#include "hnodebase.h"
#include <QSGGeometryNode>
class QSGGeometryNode;
class HBOARD_EXPORT HShapeRect :
        public HNodeBase,
        public QSGGeometryNode
{
public:
    HShapeRect(const QRect& rect, const QColor& color, int line_width);

    virtual QSGNode* build(HBoard*) override;
    virtual QRect getBoundRect() override;
    virtual void changedSelectStatus() override;
    virtual void move(const QPoint &p) override;
private:
    QRect _rect;
};

#endif // HSHAPERECT_H
