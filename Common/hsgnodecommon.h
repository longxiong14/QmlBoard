#ifndef HSGNODECOMMON_H
#define HSGNODECOMMON_H

#include <QList>
#include <QPoint>
#include <QSGFlatColorMaterial>
#include <QSGGeometry>
namespace HSGNodeCommon {
QSGGeometry *buildGeometry(const QList<QPoint> &points, unsigned long type);
QSGFlatColorMaterial *buildColor(const QColor &color);

}  // namespace HSGNodeCommon

#endif  // HSGNODECOMMON_H
