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
private:
};

#endif // HRECTNODE_H
