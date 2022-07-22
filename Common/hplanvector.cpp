#include "hplanvector.h"

#include <math.h>

const double esp = 1e-6;

HPlanVector::HPlanVector() {}

int HPlanVector::sign(double x) {
  if (fabs(x) < esp) return 0;
  return x > 0 ? 1 : -1;
}

bool HPlanVector::equal(double x, double y) { return fabs(x - y) < esp; }

double HPlanVector::dis(const QPoint& a, const QPoint& b) {
  return std::sqrt(pow(a.x() - b.x(), 2) + pow(a.y() - b.y(), 2));
}

double HPlanVector::cmult(const QPoint& a, const QPoint& b,
                          const QPoint& c)  ///叉积
{
  return (b.x() - a.x()) * (c.y() - a.y()) - (c.x() - a.x()) * (b.y() - a.y());
}

double HPlanVector::pmult(const QPoint& a, const QPoint& b) {
  return a.x() * b.x() + a.y() * b.y();
}

double HPlanVector::pldis(const QPoint& a, const QPoint& b, const QPoint& c) {
  QPoint s1, s2, s3;
  s1.setX(c.x() - b.x());
  s1.setY(c.y() - b.y());
  s2.setX(a.x() - b.x());
  s2.setY(a.y() - b.y());
  s3.setX(a.x() - c.x());
  s3.setY(a.y() - c.y());
  if (equal(b.x(), c.x()) && equal(b.y(), c.y())) return dis(a, b);

  if (sign(pmult(s1, s2)) < 0) {  //两向量成钝角
    return dis(a, b);
  } else if (sign(pmult(s1, s3)) > 0) {
    return dis(a, c);
  } else {  //该处为c在线段上方
    //两向量组成的三角形的面积为此两向的叉积的二倍，所以知道面积和底边就可以求出高
    return fabs(cmult(b, a, c)) / dis(b, c);
  }
}

double HPlanVector::ptmPoly(const QPoint& point, const QList<QPoint>& list) {
  double distance = INT_MAX;
  for (int i = 0, j = list.size() - 1; i < list.size(); j = i, i++) {
    distance = std::min(distance, pldis(point, list[j], list[i]));
  }
  return distance;
}
