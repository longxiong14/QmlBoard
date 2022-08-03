#include "hhandlepaint.h"

#include <QColor>
#include <QRect>

#include "Common/hcommons.h"
#include "Nodes/hfillnode.h"
#include "hboard.h"
#define DEBUG qDebug() << __FUNCTION__ << " " << __LINE__ << " "
HHandleDrawRect::HHandleDrawRect() {}

void HHandleDrawRect::mousePressEvent(HBoard *board, QMouseEvent *event,
                                      const QJsonObject &) {
  HHandleMove::mousePressEvent(board, event);
  if (board && event && leftButtonPress(event)) {
    _point = board->WCS2LCS(event->pos());
    auto node = new HFillNode(QRect(_point, QSize(1, 1)), Qt::GlobalColor::red,
                              GL_LINE_LOOP);
    board->pushNode(node);
    _node = node->id();
  }
}

void HHandleDrawRect::mouseMoveEvent(HBoard *board, QMouseEvent *event,
                                     const QJsonObject &) {
  HHandleMove::mouseMoveEvent(board, event);
  if (board && event && leftButtonPress(event)) {
    auto pos = board->WCS2LCS(event->pos());
    QList<QPoint> points = HCommon::BuildRectList(_point, pos);
    board->drawNodePoint(_node, points);
  } else {
    DEBUG << "board or event or node is null";
  }
}

void HHandleDrawRect::mouseReleaseEvent(HBoard *, QMouseEvent *,
                                        const QJsonObject &) {}

HHandleDrawPoly::HHandleDrawPoly() {}

void HHandleDrawPoly::mousePressEvent(HBoard *board, QMouseEvent *event,
                                      const QJsonObject &) {
  HHandleMove::mousePressEvent(board, event);
  if (board && event && leftButtonPress(event)) {
    auto point = board->WCS2LCS(event->pos());
    auto node = new HFillNode(QRect(point, QSize(1, 1)), Qt::GlobalColor::red,
                              GL_LINE_STRIP);
    _points = {point};
    board->pushNode(node);
    _node = node->id();
  }
}

void HHandleDrawPoly::mouseMoveEvent(HBoard *board, QMouseEvent *event,
                                     const QJsonObject &) {
  HHandleMove::mouseMoveEvent(board, event);
  if (board && event && leftButtonPress(event)) {
    auto point = board->WCS2LCS(event->pos());
    _points.push_back(point);
    board->drawNodePoint(_node, _points);
  }
}

void HHandleDrawPoly::mouseReleaseEvent(HBoard *board, QMouseEvent *event,
                                        const QJsonObject &) {
  HHandleMove::mouseReleaseEvent(board, event);
  if (board && event && !_points.empty() && leftButtonPress(event)) {
    _points.push_back(_points.first());
    board->drawNodePoint(_node, _points);
  }
  _points.clear();
}

HHandleDrawLine::HHandleDrawLine() {}

void HHandleDrawLine::mousePressEvent(HBoard *board, QMouseEvent *event,
                                      const QJsonObject &) {
  HHandleMove::mousePressEvent(board, event);
  if (board && event && leftButtonPress(event)) {
    _point = board->WCS2LCS(event->pos());
    QList<QPoint> list{_point};
    auto node = new HFillNode(list, Qt::GlobalColor::red, GL_LINES);
    board->pushNode(node);
    _node = node->id();
  }
}

void HHandleDrawLine::mouseMoveEvent(HBoard *board, QMouseEvent *event,
                                     const QJsonObject &) {
  HHandleMove::mouseMoveEvent(board, event);
  if (board && event && leftButtonPress(event)) {
    auto point = board->WCS2LCS(event->pos());
    QList<QPoint> list{_point, point};
    board->drawNodePoint(_node, list);
  }
}

void HHandleDrawLine::mouseReleaseEvent(HBoard *, QMouseEvent *,
                                        const QJsonObject &) {}

HHandleDrawCurve::HHandleDrawCurve() {}

void HHandleDrawCurve::mousePressEvent(HBoard *board, QMouseEvent *event,
                                       const QJsonObject &) {
  HHandleMove::mousePressEvent(board, event);
  if (board && event && leftButtonPress(event)) {
    auto point = board->WCS2LCS(event->pos());
    auto node = new HFillNode(QRect(point, QSize(1, 1)), Qt::GlobalColor::red,
                              GL_LINE_STRIP);
    _points = {point};
    board->pushNode(node);
    _node = node->id();
  }
}

void HHandleDrawCurve::mouseMoveEvent(HBoard *board, QMouseEvent *event,
                                      const QJsonObject &) {
  HHandleMove::mouseMoveEvent(board, event);
  if (board && event && leftButtonPress(event)) {
    auto point = board->WCS2LCS(event->pos());
    _points.push_back(point);
    board->drawNodePoint(_node, _points);
  }
}

void HHandleDrawCurve::mouseReleaseEvent(HBoard *, QMouseEvent *,
                                         const QJsonObject &) {
  _points.clear();
}

HHandleDrawFillRect::HHandleDrawFillRect() {}

void HHandleDrawFillRect::mousePressEvent(HBoard *board, QMouseEvent *event,
                                          const QJsonObject &) {
  HHandleMove::mousePressEvent(board, event);
  if (board && event && leftButtonPress(event)) {
    _point = board->WCS2LCS(event->pos());
    DEBUG << _point;
    auto node = new HFillNode(QRect(_point, QSize(1, 1)), Qt::GlobalColor::red,
                              GL_QUADS);
    board->pushNode(node);
    _node = node->id();
  }
}

HHandleDrawFillPoly::HHandleDrawFillPoly() {}

void HHandleDrawFillPoly::mousePressEvent(HBoard *board, QMouseEvent *event,
                                          const QJsonObject &) {
  HHandleMove::mousePressEvent(board, event);
  if (board && event && leftButtonPress(event)) {
    auto point = board->WCS2LCS(event->pos());
    auto node = new HFillNode(QRect(point, QSize(1, 1)), Qt::GlobalColor::red,
                              GL_POLYGON);
    _points = {point};
    board->pushNode(node);
    _node = node->id();
  }
}
