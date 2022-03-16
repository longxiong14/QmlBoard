#ifndef HCOMMONS_H
#define HCOMMONS_H

#include <QPoint>
#include <QRect>
namespace HCommon {
    QPoint TopLeft(const QPoint& f, const QPoint& s);
    QPoint BottomRight(const QPoint& f, const QPoint& s);
    QRect BuildRect(const QPoint& f, const QPoint& s);
}

#endif // HCOMMONS_H
