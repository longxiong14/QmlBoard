#ifndef HSGNODECOMMON_H
#define HSGNODECOMMON_H

#include <QList>
#include <QPointF>
#include <QSGFlatColorMaterial>
#include <QSGGeometry>
namespace HSGNodeCommon {
QSGGeometry *buildGeometry(const QList<QPointF> &points, unsigned long type);
QSGFlatColorMaterial *buildColor(const QColor &color);

}  // namespace HSGNodeCommon

#endif  // HSGNODECOMMON_H
