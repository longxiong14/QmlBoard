#ifndef HRECTNODE_H
#define HRECTNODE_H

#include "hnodebase.h"
#include <QRect>
#include <QSGSimpleRectNode>
class HBOARD_EXPORT HRectNode :
        public HNodeBase,
        public QSGSimpleRectNode
{
public:
    HRectNode(const QRect& rect, const QColor& color);

    virtual QSGNode * build(HBoard *) override;
    virtual QRect getBoundRect() override;
    virtual void move(const QPoint &p) override;
    virtual void moveTo(const QPoint &p) override;
    virtual void changedSelectStatus() override;
private:
    QColor _color;
};

#endif // HRECTNODE_H
