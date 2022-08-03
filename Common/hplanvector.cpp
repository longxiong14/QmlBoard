#include "hplanvector.h"

#include <math.h>

#include <QDebug>
#define DEBUG qDebug() << __FUNCTION__ << " " << __LINE__ << " "
const double esp = 1e-6;

HPlanVector::HPlanVector() {}

int HPlanVector::sign(double x) {
  if (fabs(x) < esp) return 0;
  return x > 0 ? 1 : -1;
}

bool HPlanVector::equal(double x, double y) { return fabs(x - y) < esp; }

double HPlanVector::dis(const QPointF& a, const QPointF& b) {
  return std::sqrt(pow(a.x() - b.x(), 2) + pow(a.y() - b.y(), 2));
}

double HPlanVector::cmult(const QPointF& a, const QPointF& b,
                          const QPointF& c)  //叉积
{
  return (b.x() - a.x()) * (c.y() - a.y()) - (c.x() - a.x()) * (b.y() - a.y());
}

double HPlanVector::pmult(const QPointF& a, const QPointF& b) {
  return a.x() * b.x() + a.y() * b.y();
}

double HPlanVector::pldis(const QPointF& a, const QPointF& b, const QPointF& c) {
  QPointF s1, s2, s3;
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

double HPlanVector::ptmPoly(const QPointF& point, const QList<QPointF>& list) {
  double distance = INT_MAX;
  for (int i = 0, j = list.size() - 1; i < list.size(); j = i, i++) {
    distance = std::min(distance, pldis(point, list[j], list[i]));
  }
  return distance;
}

double HPlanVector::area(const QList<QPointF>& list) {
  //
  double area = 0.0;
  if (list.size() < 3) return 0;
  auto first = list[0];
  for (int j = 1, i = 2; i < list.size(); j = i, i++) {
    area += (cmult(first, list[j], list[i])) / 2.0;
  }
  return area;
}
