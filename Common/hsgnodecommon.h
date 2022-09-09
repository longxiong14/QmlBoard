#ifndef HSGNODECOMMON_H
#define HSGNODECOMMON_H
#include <QImage>
#include <QList>
#include <QPointF>
#include <QSGFlatColorMaterial>
#include <QSGGeometry>
#include <QSGImageNode>
#include <QUuid>

#include "../HBoard_global.h"

class HBOARD_EXPORT HSGNodeCommon {
 public:
  static QSGGeometry *buildGeometry(const QList<QPointF> &points,
                                    unsigned long type);

  //  static QSGGeometry *updateGeometry(QSGGeometry *geo,
  //                                     const QList<QPointF> &list,
  //                                     unsigned long type);

  static QSGFlatColorMaterial *buildColor(const QColor &color);

  static QImage createTextImage(const QString &str, int w, int h,
                                const QColor &color = Qt::red,
                                int pixel_size = 10);

  static void releaseTextureNode(QSGImageNode *image);

  static int imageCopyTo(const QImage &src, const QRect &src_rect, QImage &dst,
                         const QRect &dst_rect);

  static QSGGeometryNode *buildGeometryNode(const QList<QPointF> &points,
                                            const QColor &color,
                                            unsigned long type);

};  // class  HSGNodeCommon

#endif  // HSGNODECOMMON_H
