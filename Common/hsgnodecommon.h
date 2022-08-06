#ifndef HSGNODECOMMON_H
#define HSGNODECOMMON_H

#include <QList>
#include <QPointF>
#include <QSGFlatColorMaterial>
#include <QSGGeometry>

#include "../HBoard_global.h"
class HBOARD_EXPORT HSGNodeCommon {
 public:
  static QSGGeometry *buildGeometry(const QList<QPointF> &points,
                                    unsigned long type);
  static QSGFlatColorMaterial *buildColor(const QColor &color);

};  // class  HSGNodeCommon

#endif  // HSGNODECOMMON_H
