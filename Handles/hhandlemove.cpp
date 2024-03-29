﻿#include "hhandlemove.h"

#include <QCursor>
#include <QMouseEvent>
#include <QPainter>
#include <QPen>
#include <QPixmap>
#include <QSGTransformNode>

#include "hboard.h"

#define DEBUG qDebug() << __FUNCTION__ << " " << __LINE__ << " "
HHandleMove::HHandleMove()
    : _scale(0.05), _cursor_size(200), _cursor_color("green") {
  _name = "none";
}

void HHandleMove::mousePressEvent(HBoard *board, QMouseEvent *event,
                                  const QJsonObject &o) {
  HHandleBase::mousePressEvent(board, event, o);
  if (board && event) {
    _last_point = board->WCS2LCS(event->pos()) - board->WCS2LCS(QPointF(0, 0));
    _last_trans = board->transform();
  }
}

void HHandleMove::mouseMoveEvent(HBoard *board, QMouseEvent *event,
                                 const QJsonObject &o) {
  HHandleBase::mouseMoveEvent(board, event, o);
  if (board && event && middleButtonPress(event)) {
    QPointF ds = board->WCS2LCS(event->pos()) - board->WCS2LCS(QPointF(0, 0));
    QTransform trans;
    trans.translate(ds.x() - _last_point.x(), ds.y() - _last_point.y());
    board->pushTransform(trans * _last_trans);
  }
}

void HHandleMove::mouseReleaseEvent(HBoard *board, QMouseEvent *event,
                                    const QJsonObject &o) {
  HHandleBase::mouseReleaseEvent(board, event, o);
  if (board && event) {
    _last_point = QPointF();
    _last_trans = QTransform();
    if (!_node.isNull()) {
      board->clearSelect();
      board->setSelect(_node);
      _node = "";
    }
  }
}

void HHandleMove::wheelEvent(HBoard *board, QWheelEvent *event) {
  if (board && event) {
    auto keys = board->keys();
    auto s = _scale;
    if (keys.contains(Qt::Key_Control)) {
      s *= 10;
    }
    auto dlt = event->delta();
    auto pos = board->WCS2LCS(event->pos());
    auto trans = board->transformNode()->matrix().toTransform();
    double scale = 1.0;

    if (dlt < 0) {
      scale = 1 / (1 + s);
      //      scale -= s;
    } else {
      scale += s;
    }
    trans.translate(pos.x(), pos.y());
    trans.scale(scale, scale);
    trans.translate(-pos.x(), -pos.y());
    board->pushTransform(trans);
  }
}

void HHandleMove::hoverEnterEvent(HBoard *board, QHoverEvent *,
                                  const QJsonObject &) {
  if (board) {
    QPixmap map(_cursor_size, _cursor_size);
    map.fill(Qt::transparent);
    QPainter painter(&map);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillRect(map.rect(), QColor(0, 0, 0, 0));
    QColor color;
    color.setNamedColor(_cursor_color);
    painter.setPen(color);
    painter.drawLine(0, _cursor_size / 2, _cursor_size, _cursor_size / 2);
    painter.drawLine(_cursor_size / 2, 0, _cursor_size / 2, _cursor_size);
    QCursor cursor(map);
    board->setCursor(cursor);
  }
}

void HHandleMove::setCursorSize(int size) { _cursor_size = size; }

int HHandleMove::cursorSize() { return _cursor_size; }

void HHandleMove::setColor(const QString &color) { _cursor_color = color; }

void HHandleMove::setColor(const QColor &color) {
  _cursor_color = color.name();
}

QColor HHandleMove::getCursorColor() {
  QColor color;
  color.setNamedColor(_cursor_color);
  return color;
}

double HHandleMove::getScale() { return _scale; }

void HHandleMove::setScale(double s) { _scale = s; }
