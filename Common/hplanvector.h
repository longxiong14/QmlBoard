#ifndef HPLANVECTOR_H
#define HPLANVECTOR_H

#include <QList>
#include <QPointF>

#include "../HBoard_global.h"
class HBOARD_EXPORT HPlanVector {
 public:
  explicit HPlanVector();

  int sign(double x);
  bool equal(double x, double y);
  double dis(const QPointF& a, const QPointF& b);  //两点之间的距离
  double cmult(const QPointF& a, const QPointF& b, const QPointF& c);  //叉积
  double pmult(const QPointF& a, const QPointF& b);  //点积 这里的ab表示的是向量
  double pldis(const QPointF& a, const QPointF& b,
               const QPointF& c);  //点a，线段起点b，线段终点c
  double ptmPoly(const QPointF& point,
                 const QList<QPointF>& list);  //点到多边形的最小距离
  double area(const QList<QPointF>& list);
};

#endif  // HPLANVECTOR_H
