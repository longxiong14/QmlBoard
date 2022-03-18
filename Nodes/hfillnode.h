#ifndef HFILLNODE_H
#define HFILLNODE_H

#include "hnodebase.h"
#include <QSGGeometryNode>
class HBOARD_EXPORT HFillNode
        : public HNodeBase,
        public QSGGeometryNode
{
public:
    HFillNode(const QList<QPoint>& points, const QColor& color, bool fill = false);
    HFillNode(const QRect& rect, const QColor& color, bool fill = false);

    virtual QSGNode* get()override;
    virtual QSGNode * build(HBoard *) override;
    virtual QRect getBoundRect() override;
    virtual void move(const QPoint &p) override;
    virtual void moveTo(const QPoint &p) override;
    virtual void changedSelectStatus() override;
    virtual void drawPoints(const QList<QPoint> &points) override;
    virtual void setColor(const QColor& color) override;
protected:
    QSGGeometry* buildGeometry(const QList<QPoint>& points, bool fill = false);
    void setOurGeometry(const QList<QPoint>& points, bool fill = false);
};

//class HBOARD_EXPORT HPolyNode
//        : public HNodeBase,
//        public QSGGeometryNode
//{
//public:
//    HPolyNode(const QList<QPoint>& points, const QColor& color);
//    virtual QSGNode* get()override;
//    virtual QSGNode * build(HBoard *) override;
//    virtual QRect getBoundRect() override;
//    virtual void move(const QPoint &p) override;
//    virtual void moveTo(const QPoint &p) override;
//    virtual void changedSelectStatus() override;
//    virtual void drawPoints(const QList<QPoint> &points) override;
//};

#endif // HFILLNODE_H
