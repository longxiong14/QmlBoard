#include "hhandlepaint.h"

#include <QColor>
#include <QRectF>

#include "Common/hcommons.h"
#include "Nodes/hfillnode.h"
#include "hboard.h"
#define DEBUG qDebug() << __FUNCTION__ << " " << __LINE__ << " "

QJsonObject defaultParam() {
  QJsonObject object;
  object.insert("line_width", 1);
  object.insert("b", 0);
  object.insert("g", 0);
  object.insert("r", 255);
  object.insert("a", 255);
  return object;
}

HHandleDrawRect::HHandleDrawRect() { _name = "rect"; }

void HHandleDrawRect::mousePressEvent(HBoard *board, QMouseEvent *event,
                                      const QJsonObject &o) {
  HHandleMove::mousePressEvent(board, event);
  if (board && event && leftButtonPress(event)) {
    _point = board->WCS2LCS(event->pos());
    auto node = std::make_shared<HFillNode>(QRectF(_point, QSize(1, 1)),
                                            GL_LINE_LOOP, o);
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

QJsonObject HHandleDrawRect::getDefaultParam() { return defaultParam(); }

HHandleDrawPoly::HHandleDrawPoly() { _name = "poly"; }

void HHandleDrawPoly::mousePressEvent(HBoard *board, QMouseEvent *event,
                                      const QJsonObject &o) {
  HHandleMove::mousePressEvent(board, event);
  if (board && event && leftButtonPress(event)) {
    auto point = board->WCS2LCS(event->pos());
    auto node = std::make_shared<HFillNode>(QRectF(point, QSize(1, 1)),
                                            GL_LINE_STRIP, o);
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

QJsonObject HHandleDrawPoly::getDefaultParam() { return defaultParam(); }

HHandleDrawLine::HHandleDrawLine() { _name = "line"; }

void HHandleDrawLine::mousePressEvent(HBoard *board, QMouseEvent *event,
                                      const QJsonObject &o) {
  HHandleMove::mousePressEvent(board, event);
  if (board && event && leftButtonPress(event)) {
    _point = board->WCS2LCS(event->pos());
    QList<QPointF> list{_point};
    auto node = std::make_shared<HFillNode>(list, GL_LINES, o);
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

QJsonObject HHandleDrawLine::getDefaultParam() { return defaultParam(); }

HHandleDrawCurve::HHandleDrawCurve() { _name = "curve"; }

void HHandleDrawCurve::mousePressEvent(HBoard *board, QMouseEvent *event,
                                       const QJsonObject &o) {
  HHandleMove::mousePressEvent(board, event);
  if (board && event && leftButtonPress(event)) {
    auto point = board->WCS2LCS(event->pos());
    auto node = std::make_shared<HFillNode>(QRectF(point, QSize(1, 1)),
                                            GL_LINE_STRIP, o);
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

QJsonObject HHandleDrawCurve::getDefaultParam() { return defaultParam(); }

HHandleDrawFillRect::HHandleDrawFillRect() { _name = "fill rect"; }

void HHandleDrawFillRect::mousePressEvent(HBoard *board, QMouseEvent *event,
                                          const QJsonObject &o) {
  HHandleMove::mousePressEvent(board, event);
  if (board && event && leftButtonPress(event)) {
    _point = board->WCS2LCS(event->pos());
    auto node =
        std::make_shared<HFillNode>(QRectF(_point, QSize(1, 1)), GL_QUADS, o);
    board->pushNode(node);
    _node = node->id();
  }
}

HHandleDrawFillPoly::HHandleDrawFillPoly() { _name = "fill poly"; }

void HHandleDrawFillPoly::mousePressEvent(HBoard *board, QMouseEvent *event,
                                          const QJsonObject &o) {
  HHandleMove::mousePressEvent(board, event);
  if (board && event && leftButtonPress(event)) {
    auto point = board->WCS2LCS(event->pos());
    auto node =
        std::make_shared<HFillNode>(QRectF(point, QSize(1, 1)), GL_POLYGON, o);
    _points = {point};
    board->pushNode(node);
    _node = node->id();
  }
}
