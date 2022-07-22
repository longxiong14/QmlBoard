#ifndef HCOMMONS_H
#define HCOMMONS_H

#include <QList>
#include <QPoint>
#include <QRect>

namespace HCommon {

QPoint TopLeft(const QPoint& f, const QPoint& s);
QPoint TopRight(const QPoint& f, const QPoint& s);
QPoint BottomLeft(const QPoint& f, const QPoint& s);
QPoint BottomRight(const QPoint& f, const QPoint& s);

QRect BuildRect(const QPoint& f, const QPoint& s);
QList<QPoint> BuildRectList(const QPoint& f, const QPoint& s);

bool PointInContour(const QPoint& point, const QList<QPoint>& contour);

// const double esp = 1e-6;
// class dot_line_dis : public QObject {
//  Q_OBJECT
// public:
//  explicit dot_line_dis(QObject* parent = nullptr);

//  int sign(double x);
//  double dis(QPointF a, QPointF b);               //两点之间的距离
//  double cmult(QPointF a, QPointF b, QPointF c);  //叉积
//  double pmult(QPointF a, QPointF b);  //点积 这里的ab表示的是向量
//  double pldis(QPointF a, QPointF b, QPointF c);  //点a，线段起点b，线段终点c
//};

}  // namespace HCommon

#endif  // HCOMMONS_H
