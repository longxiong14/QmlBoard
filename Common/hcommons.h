#ifndef HCOMMONS_H
#define HCOMMONS_H

#include <QList>
#include <QPointF>
#include <QRectF>

#include "../HBoard_global.h"
class HBOARD_EXPORT HCommon {
 public:
  static QPointF TopLeft(const QPointF& f, const QPointF& s);
  static QPointF TopRight(const QPointF& f, const QPointF& s);
  static QPointF BottomLeft(const QPointF& f, const QPointF& s);
  static QPointF BottomRight(const QPointF& f, const QPointF& s);

  static QRectF BuildRect(const QPointF& f, const QPointF& s);
  static QList<QPointF> BuildRectList(const QPointF& f, const QPointF& s);
  static QList<QPointF> BuildRectList(const QRectF& rect);

  static bool PointInContour(const QPointF& point,
                             const QList<QPointF>& contour);

  static QList<QPointF> BuildRectLinesList(const QRectF& rect);
  static QList<QPointF> BuildPolyLinesList(const QList<QPointF>& list);

  static QList<QPointF> BuildCircle(const QPointF& center, double radius,
                                    int parts);

};  // class HCommon

#endif  // HCOMMONS_H
