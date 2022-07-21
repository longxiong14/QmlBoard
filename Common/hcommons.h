#ifndef HCOMMONS_H
#define HCOMMONS_H

#include <QList>
#include <QPoint>
#include <QRect>
struct Line {
  Line(const QPoint& a, const QPoint& b) : _a(a), _b(b) {}

  int maxX() { return std::max(_a.x(), _b.x()); }
  int minX() { return std::min(_a.x(), _b.x()); }
  int maxY() { return std::max(_a.y(), _b.y()); }
  int minY() { return std::min(_a.y(), _b.y()); }

  int operator*(const Line& l) {
    auto x1 = QPoint(_b.x() - _a.x(), _b.y() - _a.y());
    auto x2 = QPoint(l._b.x() - l._a.x(), l._b.y() - l._a.y());
    return x1.y() * x2.x() - x2.y() * x1.x();
  }

  QPoint _a, _b;
};

namespace HCommon {

QPoint TopLeft(const QPoint& f, const QPoint& s);
QPoint TopRight(const QPoint& f, const QPoint& s);
QPoint BottomLeft(const QPoint& f, const QPoint& s);
QPoint BottomRight(const QPoint& f, const QPoint& s);
QRect BuildRect(const QPoint& f, const QPoint& s);
QList<QPoint> BuildRectList(const QPoint& f, const QPoint& s);
bool PointInContour(const QPoint& point, const QList<QPoint>& contour);

}  // namespace HCommon

#endif  // HCOMMONS_H
