#ifndef HPLANVECTOR_H
#define HPLANVECTOR_H

#include <QList>
#include <QPoint>
class HPlanVector {
 public:
  explicit HPlanVector();

  int sign(double x);
  bool equal(double x, double y);
  double dis(const QPoint& a, const QPoint& b);  //两点之间的距离
  double cmult(const QPoint& a, const QPoint& b, const QPoint& c);  //叉积
  double pmult(const QPoint& a, const QPoint& b);  //点积 这里的ab表示的是向量
  double pldis(const QPoint& a, const QPoint& b,
               const QPoint& c);  //点a，线段起点b，线段终点c
  double ptmPoly(const QPoint& point,
                 const QList<QPoint>& list);  //点到多边形的最小距离
};

#endif  // HPLANVECTOR_H
