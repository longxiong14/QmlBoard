#ifndef HCOMMONS_H
#define HCOMMONS_H

#include <QList>
#include <QPointF>
#include <QRectF>

namespace HCommon {

QPointF TopLeft(const QPointF& f, const QPointF& s);
QPointF TopRight(const QPointF& f, const QPointF& s);
QPointF BottomLeft(const QPointF& f, const QPointF& s);
QPointF BottomRight(const QPointF& f, const QPointF& s);

QRectF BuildRect(const QPointF& f, const QPointF& s);
QList<QPointF> BuildRectList(const QPointF& f, const QPointF& s);
QList<QPointF> BuildRectList(const QRectF& rect);

bool PointInContour(const QPointF& point, const QList<QPointF>& contour);

QList<QPointF> BuildRectLinesList(const QRectF& rect);
QList<QPointF> BuildPolyLinesList(const QList<QPointF>& list);

}  // namespace HCommon

#endif  // HCOMMONS_H
