#ifndef HSGNODECOMMON_H
#define HSGNODECOMMON_H
#include <QImage>
#include <QList>
#include <QPointF>
#include <QSGFlatColorMaterial>
#include <QSGGeometry>
#include <QSGImageNode>

#include "../HBoard_global.h"
class HBOARD_EXPORT HSGNodeCommon {
 public:
  static QSGGeometry *buildGeometry(const QList<QPointF> &points,
                                    unsigned long type);

  //  static QSGGeometry *updateGeometry(QSGGeometry *geo,
  //                                     const QList<QPointF> &list,
  //                                     unsigned long type);

  static QSGFlatColorMaterial *buildColor(const QColor &color);

  static QImage createTextImage(const QString &str, int w, int h);

  static void releaseTextureNode(QSGImageNode *image);

};  // class  HSGNodeCommon

#endif  // HSGNODECOMMON_H
