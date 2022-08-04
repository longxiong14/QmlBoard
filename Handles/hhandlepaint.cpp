#include "hhandlepaint.h"

#include <QColor>
#include <QRectF>

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
    auto node = new HFillNode(QRectF(_point, QSize(1, 1)), Qt::GlobalColor::red,
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
    QList<QPointF> points = HCommon::BuildRectList(_point, pos);
    board->drawNodePoint(_node, points);
  }
}

HHandleDrawPoly::HHandleDrawPoly() {}

void HHandleDrawPoly::mousePressEvent(HBoard *board, QMouseEvent *event,
                                      const QJsonObject &) {
  HHandleMove::mousePressEvent(board, event);
  if (board && event && leftButtonPress(event)) {
    auto point = board->WCS2LCS(event->pos());
    auto node = new HFillNode(QRectF(point, QSize(1, 1)), Qt::GlobalColor::red,
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
  if (board && event && !_points.empty()) {
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
    QList<QPointF> list{_point};
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
    QList<QPointF> list{_point, point};
    board->drawNodePoint(_node, list);
  }
}

HHandleDrawCurve::HHandleDrawCurve() {}

void HHandleDrawCurve::mousePressEvent(HBoard *board, QMouseEvent *event,
                                       const QJsonObject &) {
  HHandleMove::mousePressEvent(board, event);
  if (board && event && leftButtonPress(event)) {
    auto point = board->WCS2LCS(event->pos());
    auto node = new HFillNode(QRectF(point, QSize(1, 1)), Qt::GlobalColor::red,
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

void HHandleDrawCurve::mouseReleaseEvent(HBoard *b, QMouseEvent *e,
                                         const QJsonObject &o) {
  _points.clear();
  HHandleMove::mouseReleaseEvent(b, e, o);
}

HHandleDrawFillRect::HHandleDrawFillRect() {}

void HHandleDrawFillRect::mousePressEvent(HBoard *board, QMouseEvent *event,
                                          const QJsonObject &) {
  HHandleMove::mousePressEvent(board, event);
  if (board && event && leftButtonPress(event)) {
    _point = board->WCS2LCS(event->pos());
    auto node = new HFillNode(QRectF(_point, QSize(1, 1)), Qt::GlobalColor::red,
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
    auto node = new HFillNode(QRectF(point, QSize(1, 1)), Qt::GlobalColor::red,
                              GL_POLYGON);
    _points = {point};
    board->pushNode(node);
    _node = node->id();
  }
}
