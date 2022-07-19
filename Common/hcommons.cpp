#include "hcommons.h"

QPoint HCommon::TopLeft(const QPoint &f, const QPoint &s) {
  auto x = std::min(f.x(), s.x());
  auto y = std::min(f.y(), s.y());
  return QPoint(x, y);
}

QPoint HCommon::BottomRight(const QPoint &f, const QPoint &s) {
  auto x = std::max(f.x(), s.x());
  auto y = std::max(f.y(), s.y());
  return QPoint(x, y);
}

QRect HCommon::BuildRect(const QPoint &f, const QPoint &s) {
  return QRect(TopLeft(f, s), BottomRight(f, s));
}

QPoint HCommon::TopRight(const QPoint &f, const QPoint &s) {
  auto x = std::min(f.x(), s.x());
  auto y = std::max(f.y(), s.y());
  return QPoint(x, y);
}

QPoint HCommon::BottomLeft(const QPoint &f, const QPoint &s) {
  auto x = std::min(f.x(), s.x());
  auto y = std::max(f.y(), s.y());
  return QPoint(x, y);
}

QList<QPoint> HCommon::BuildRectList(const QPoint &f, const QPoint &s) {
  auto tl = TopLeft(f, s);
  auto br = BottomRight(f, s);
  QRect rect(tl, br);
  QList<QPoint> list{{rect.left(), rect.top()},
                     {rect.right(), rect.top()},
                     {rect.right(), rect.bottom()},
                     {rect.left(), rect.bottom()}};
  return list;
}
