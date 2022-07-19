#include "hhandlepaint.h"

#include <QColor>
#include <QRect>

#include "Common/hcommons.h"
#include "Nodes/hfillnode.h"
#include "hboard.h"
#define DEBUG qDebug() << __FUNCTION__ << " " << __LINE__ << " "
HHandleDrawRect::HHandleDrawRect() : HHandleBase() {}

void HHandleDrawRect::mousePressEvent(HBoard *board, QMouseEvent *event) {
  DEBUG << "mouse press";
  if (board && event) {
    _point = board->WCS2LCS(event->pos());
    auto node = new HFillNode(QRect(_point, QSize(1, 1)), Qt::GlobalColor::red);
    board->pushNode(node);
    _node = node->id();
  }
}

void HHandleDrawRect::mouseMoveEvent(HBoard *board, QMouseEvent *event) {
  if (board && event) {
    auto pos = board->WCS2LCS(event->pos());
    QList<QPoint> points = HCommon::BuildRectList(_point, pos);
    DEBUG << points;
    board->drawNodePoint(_node, points);
  } else {
    DEBUG << "board or event or node is null";
  }
}

void HHandleDrawRect::mouseReleaseEvent(HBoard *, QMouseEvent *) {}

void HHandleDrawRect::wheelEvent(HBoard *, QWheelEvent *) {}

HHandleDrawPoly::HHandleDrawPoly() {}

void HHandleDrawPoly::mousePressEvent(HBoard *board, QMouseEvent *event) {
  DEBUG << "mouse press";
  if (board && event) {
    auto point = board->WCS2LCS(event->pos());
    auto node = new HFillNode(QRect(point, QSize(1, 1)), Qt::GlobalColor::red,
                              GL_LINE_STRIP);
    _points = {point};
    board->pushNode(node);
    _node = node->id();
  }
}

void HHandleDrawPoly::mouseMoveEvent(HBoard *board, QMouseEvent *event) {
  if (board && event) {
    auto point = board->WCS2LCS(event->pos());
    _points.push_back(point);
    board->drawNodePoint(_node, _points);
  }
}

void HHandleDrawPoly::mouseReleaseEvent(HBoard *board, QMouseEvent *event) {
  if (board && event) {
    auto point = board->WCS2LCS(event->pos());
    _points.push_back(point);
    _points.push_back(_points.first());
    board->drawNodePoint(_node, _points);
  }
  _points.clear();
}

void HHandleDrawPoly::wheelEvent(HBoard *, QWheelEvent *) {}
