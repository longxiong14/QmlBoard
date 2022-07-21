#include "hcommons.h"

#include <QDebug>

#include "../Nodes/hfillnode.h"
#include "../hboard.h"
#include "../hboardmanager.h"
#define DEBUG qDebug() << __FUNCTION__ << " " << __LINE__ << " "
#define DCODE true

QUuid drawLine(const QPoint &f, const QPoint &s) {
  auto board = HBoardManager::getInstance()->getBoard("test_board");
  auto list = QList<QPoint>({f, s});
  auto node = new HFillNode(list, QColor(255, 255, 0, 255), GL_LINES);
  board->pushNode(node, false);
  return node->id();
}

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

bool HCommon::PointInContour(const QPoint &vtPoint,
                             const QList<QPoint> &vecPoints) {
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
          drawLine(vecPoints[i], vecPoints[j]);
        }
      }
      //进行下一线段判断
      j = i;
    }

    return bResult;
  }
}
