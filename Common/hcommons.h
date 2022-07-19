#ifndef HCOMMONS_H
#define HCOMMONS_H

#include <QList>
#include <QPoint>
#include <QRect>
namespace HCommon {
QPoint TopLeft(const QPoint& f, const QPoint& s);
QPoint TopRight(const QPoint& f, const QPoint& s);
QPoint BottomLeft(const QPoint& f, const QPoint& s);
QPoint BottomRight(const QPoint& f, const QPoint& s);
QRect BuildRect(const QPoint& f, const QPoint& s);
QList<QPoint> BuildRectList(const QPoint& f, const QPoint& s);
}  // namespace HCommon

#endif  // HCOMMONS_H
