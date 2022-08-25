#include "hcommons.h"

#include <QDebug>

#include "../Nodes/hfillnode.h"
#include "../hboard.h"
#include "../hboardmanager.h"
#define DEBUG qDebug() << __FUNCTION__ << " " << __LINE__ << " "
#define DCODE true
#define D 1e-7
#define PI 3.1415
QPointF HCommon::TopLeft(const QPointF &f, const QPointF &s) {
  auto x = std::min(f.x(), s.x());
  auto y = std::min(f.y(), s.y());
  return QPointF(x, y);
}

QPointF HCommon::BottomRight(const QPointF &f, const QPointF &s) {
  auto x = std::max(f.x(), s.x());
  auto y = std::max(f.y(), s.y());
  return QPointF(x, y);
}

QRectF HCommon::BuildRect(const QPointF &f, const QPointF &s) {
  return QRectF(TopLeft(f, s), BottomRight(f, s));
}

QPointF HCommon::TopRight(const QPointF &f, const QPointF &s) {
  auto x = std::min(f.x(), s.x());
  auto y = std::max(f.y(), s.y());
  return QPointF(x, y);
}

QPointF HCommon::BottomLeft(const QPointF &f, const QPointF &s) {
  auto x = std::min(f.x(), s.x());
  auto y = std::max(f.y(), s.y());
  return QPointF(x, y);
}

QList<QPointF> HCommon::BuildRectList(const QPointF &f, const QPointF &s) {
  auto tl = TopLeft(f, s);
  auto br = BottomRight(f, s);
  QRectF rect(tl, br);
  return BuildRectList(rect);
}

QList<QPointF> HCommon::BuildRectList(const QRectF &rect) {
  QList<QPointF> list{{rect.left(), rect.top()},
                      {rect.right(), rect.top()},
                      {rect.right(), rect.bottom()},
                      {rect.left(), rect.bottom()},
                      {rect.left(), rect.top()}};
  return list;
}

bool HCommon::PointInContour(const QPointF &vtPoint,
                             const QList<QPointF> &vecPoints) {
  if (true) {
    bool bResult =
        false;  //判断结果（true；点落在多边形内；false:点未落在多边形内）
    int nSize = vecPoints.size();
    int j = nSize - 1;  // nSize -1 是多边形的最后一个顶点
    for (int i = 0; i < nSize; i++) {
      //判断点是否在线段的两侧(只取上端点,不取下端点，否则会多出一次判断，出现异常)
      if ((vecPoints[i].y() < vtPoint.y() && vecPoints[j].y() >= vtPoint.y()) ||
          (vecPoints[j].y() < vtPoint.y() && vecPoints[i].y() >= vtPoint.y())) {
        //根据两点式方程计算出过点P且平行于X轴的直线与线段的交点，两点式方程：x
        //= x1 +  (y - y1) * (x2 - x1) / (y2 - y1);
        if (vecPoints[i].x() + (vtPoint.y() - vecPoints[i].y()) *
                                   (vecPoints[j].x() - vecPoints[i].x()) /
                                   (vecPoints[j].y() - vecPoints[i].y()) <
            vtPoint.x()) {
          bResult = !bResult;
        }
      }
      //进行下一线段判断
      j = i;
    }
    return bResult;
  } else {
    bool bResult =
        false;  //判断结果（true；点落在多边形内；false:点未落在多边形内）
    int nSize = vecPoints.size();
    int j = nSize - 1;  // nSize -1 是多边形的最后一个顶点
    for (int i = 0; i < nSize; i++) {
      //判断点是否在线段的两侧
      if ((vecPoints[i].y() < vtPoint.y() && vecPoints[j].y() >= vtPoint.y()) ||
          (vecPoints[j].y() < vtPoint.y() && vecPoints[i].y() >= vtPoint.y())) {
        //根据两点式方程计算出过点P且平行于X轴的直线与线段的交点，两点式方程：x
        //= x1 +  (y - y1) * (x2 - x1) / (y2 - y1);
        double value = vecPoints[i].x() +
                       (vtPoint.y() - vecPoints[i].y()) *
                           (vecPoints[j].x() - vecPoints[i].x()) /
                           (vecPoints[j].y() - vecPoints[i].y()) -
                       vtPoint.x();
        DEBUG << value;
        if (value < 0) {
          bResult = !bResult;
        }
      }
      //进行下一线段判断
      j = i;
    }

    return bResult;
  }
}

bool HCommon::PointInRect(const QPointF &point, const QRectF &rect) {
  auto tl = rect.topLeft();
  auto br = rect.bottomRight();
  if (point.x() >= tl.x() && point.y() >= tl.y() && point.x() <= br.x() &&
      point.y() <= br.y()) {
    return true;
  }
  return false;
}

QList<QPointF> HCommon::BuildRectLinesList(const QRectF &rect) {
  //  DEBUG << rect;
  auto tl = rect.topLeft(), br = rect.bottomRight();
  QList<QPointF> h_line1, h_line2, v_line1, v_line2;
  int x_min = int(tl.x()), x_max = int(br.x() + 1), y_min = int(tl.y()),
      y_max = int(br.y() + 1);
  //  DEBUG << x_max << " " << x_min << " " << y_max << " " << y_min;
  for (int i = x_min, j = x_max; i <= x_max && j >= x_min; i++, j--) {
    h_line1.push_back(QPointF(i, y_min));
    h_line2.push_back(QPointF(j, y_max));
  }

  for (int i = y_min, j = y_max; i <= y_max && j >= y_min; i++, j--) {
    v_line1.push_back(QPointF(x_max, i));
    v_line2.push_back(QPointF(x_min, j));
  }
  QList<QPointF> list;
  list.append(h_line1);
  list.append(v_line1);
  list.append(h_line2);
  list.append(v_line2);
  //  DEBUG << list.size();
  if (!list.empty()) {
    list.push_back(list.first());
  }
  return list;
}

QList<QPointF> HCommon::BuildPolyLinesList(const QList<QPointF> &list) {
  QList<QPointF> out;
  for (int i = 1, j = 0; i < list.size(); j = i, i++) {
    auto p1 = list[j], p2 = list[i];
    if (std::fabs(p1.x() - p2.x()) < D) {
      int num = 1;
      if (p2.y() < p1.y()) {
        num = -1;
      }
      for (int idx = int(p1.y()); (num * idx) <= (num * p2.y()); idx += num) {
        out.push_back(QPointF(p1.x(), idx));
      }
    } else if (std::fabs(p1.y() - p2.y()) < D) {
      int num = 1;
      if (p2.x() < p1.x()) {
        num = -1;
      }
      for (int idx = int(p1.x()); (num * idx) <= (num * p2.x()); idx += num) {
        out.push_back(QPointF(idx, p1.y()));
      }
    } else {
      double k = 1.0 * (p2.y() - p1.y()) / (p2.x() - p1.x());
      double b = p2.y() - k * p2.x();
      auto func = [=](int x) { return 1.0 * x * k + b; };
      auto func2 = [=](int y) { return 1.0 * (y - b) / k; };
      if (std::abs(p2.x() - p1.x()) > std::abs(p2.y() - p1.y())) {
        int num = 1;
        if (p2.x() < p1.x()) {
          num = -1;
        }
        for (int idx = int(p1.x()); (num * idx) < (num * p2.x()); idx += num) {
          out.push_back(QPointF(idx, func(idx)));
        }
      } else {
        int num = 1;
        if (p2.y() < p1.y()) {
          num = -1;
        }
        for (int idx = int(p1.y()); (num * idx) < (num * p2.y()); idx += num) {
          out.push_back(QPointF(func2(idx), idx));
        }
      }
    }
  }

  return out;
}

QList<QPointF> HCommon::BuildCircle(const QPointF &center, double radius,
                                    int parts) {
  QList<QPointF> list;

  double x, z;
  for (int i = 0; i <= parts; i++) {
    x = center.x() + std::sin(i * (PI / 180)) * radius;
    z = center.y() + std::cos(i * (PI / 180)) * radius;
    list.push_back(QPointF(x, z));
  }

  return list;
}

QList<QPointF> HCommon::BuildWideLine(const QList<QPointF> &list,
                                      double line_width) {
  QList<QPointF> out_line, in_line;
  double x, z;
  for (int i = 0, j = list.size() - 1; i < list.size(); j = i, i++) {
    auto center = list[i];
    auto p = list[j];
    double leng = std::pow(
        (std::pow(center.x() - p.x(), 2) + std::pow(center.y() - p.y(), 2)),
        0.5);
    double index = std::acos((center.x() - p.x()) / leng);
    x = center.x() + std::sin(index * (PI / 180)) * line_width;
    z = center.y() + std::cos(index * (PI / 180)) * line_width;
    if (center.x() < p.x()) {
      out_line.push_back(QPointF(x, z));
    } else {
      //      out_line.push_front(QPointF(x, z));
    }
  }
  out_line.append(in_line);
  return out_line;
}

bool HCommon::RectHasOverlap(const QRectF &r1, const QRectF &r2) {
  auto func = [](const QRectF &rect) {
    QList<QPointF> list1{{rect.left(), rect.top()},
                         {rect.right(), rect.top()},
                         {rect.right(), rect.bottom()},
                         {rect.left(), rect.bottom()}};
    return list1;
  };
  auto list1 = func(r1);
  auto list2 = func(r2);
  for (int i = 0; i < list1.size(); i++) {
    auto p1 = list1[i];
    auto p2 = list2[i];
    if (PointInContour(p1, list2) || PointInContour(p2, list1)) {
      return true;
    }
  }
  return false;
}
