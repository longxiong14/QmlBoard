#include "hhandlepaint.h"

#include <QColor>
#include <QRectF>

#include "../Nodes/hshapenodes.h"
#include "../Operators/hboardactionbase.h"
#include "../Operators/hcommandbase.h"
#include "Common/hcommons.h"
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

int commandNode(HBoard *board, std::shared_ptr<HNodeBase> node) {
  if (!board) return -1;
  auto command = board->getCommand();
  if (command) {
    auto action = std::make_shared<HPushNodeAction>(board->name(), node);
    command->excute(action);
  } else {
    board->pushNode(node);
  }
  return 0;
}

HHandleDrawRect::HHandleDrawRect() { _name = "rect"; }

void HHandleDrawRect::mousePressEvent(HBoard *board, QMouseEvent *event,
                                      const QJsonObject &o) {
  HHandleMove::mousePressEvent(board, event);
  if (board && event && isButtonPress(event)) {
    _point = board->WCS2LCS(event->pos());
    auto node =
        std::make_shared<HShapeRectNode>(QRectF(_point, QSize(0, 0)), o);
    commandNode(board, node);
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

HHandleDrawLine::HHandleDrawLine() { _name = "line"; }

void HHandleDrawLine::mousePressEvent(HBoard *board, QMouseEvent *event,
                                      const QJsonObject &o) {
  HHandleMove::mousePressEvent(board, event);
  if (board && event && isButtonPress(event)) {
    _point = board->WCS2LCS(event->pos());
    auto node = std::make_shared<HShapeLineNode>(_point, _point, o);
    commandNode(board, node);
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

HHandleDrawCurve::HHandleDrawCurve() : _size(0) { _name = "curve"; }

void HHandleDrawCurve::mousePressEvent(HBoard *board, QMouseEvent *event,
                                       const QJsonObject &o) {
  HHandleMove::mousePressEvent(board, event);
  if (board && event && isButtonPress(event)) {
    auto point = board->WCS2LCS(event->pos());
    if (_node.isNull()) {
      auto node = std::make_shared<HShapeCurveNode>(QList<QPointF>({point}), o);
      _size = 1;
      commandNode(board, node);
      _node = node->id();
    } else {
      auto node = board->getNodeById(_node);
      if (node) {
        auto points = node->getPointList();
        while (points.size() > _size) {
          points.pop_back();
        }
        points.push_back(point);
        _size++;
        board->drawNodePoint(_node, points);
      }
    }
  }
}

void HHandleDrawCurve::hoverMoveEvent(HBoard *board, QHoverEvent *event,
                                      const QJsonObject &) {
  if (board && event) {
    auto point = board->WCS2LCS(event->pos());
    if (!_node.isNull()) {
      auto node = board->getNodeById(_node);
      if (node) {
        auto points = node->getPointList();
        while (points.size() > _size) {
          points.pop_back();
        }
        points.push_back(point);
        board->drawNodePoint(_node, points);
      }
    }
  }
}

void HHandleDrawCurve::mouseReleaseEvent(HBoard *b, QMouseEvent *e,
                                         const QJsonObject &o) {
  if (isButtonPress(e, Qt::MouseButton::RightButton)) {
    leave(b);
    if (b) {
      b->clearSelect();
      b->setSelect(_node);
    }
    HHandleMove::mouseReleaseEvent(b, e, o);
  } else {
    if (b) {
      b->clearSelect();
      b->setSelect(_node);
    }
  }
}

void HHandleDrawCurve::boardLeaveOffThisHandle(HBoard *board) {
  leave(board);
  _node = "";
}

QJsonObject HHandleDrawCurve::getDefaultParam() { return defaultParam(); }

void HHandleDrawCurve::leave(HBoard *board) {
  if (board && !_node.isNull()) {
    auto node = board->getNodeById(_node);
    if (node) {
      auto points = node->getPointList();
      if (!points.empty()) {
        while (points.size() > _size) {
          points.pop_back();
        }
        board->drawNodePoint(_node, points);
      }
    }
  }
  _size = 0;
}

HHandleDrawPoly::HHandleDrawPoly() : _size(0) { _name = "poly"; }

void HHandleDrawPoly::mousePressEvent(HBoard *board, QMouseEvent *event,
                                      const QJsonObject &object) {
  HHandleMove::mousePressEvent(board, event);
  if (board && event && isButtonPress(event)) {
    auto point = board->WCS2LCS(event->pos());
    if (_node.isNull()) {
      auto node =
          std::make_shared<HShapePolyNode>(QList<QPointF>({point}), object);
      commandNode(board, node);
      _size = 1;
      _node = node->id();
    } else {
      auto node = board->getNodeById(_node);
      if (node) {
        auto points = node->getPointList();
        while (points.size() > _size) {
          points.pop_back();
        }
        points.push_back(point);
        _size++;
        board->drawNodePoint(_node, points);
      }
    }
  }
}

void HHandleDrawPoly::hoverMoveEvent(HBoard *board, QHoverEvent *event,
                                     const QJsonObject &) {
  if (board && event) {
    auto point = board->WCS2LCS(event->pos());
    if (!_node.isNull()) {
      auto node = board->getNodeById(_node);
      if (node) {
        auto points = node->getPointList();
        while (points.size() > _size) {
          points.pop_back();
        }
        points.push_back(point);
        board->drawNodePoint(_node, points);
      }
    }
  }
}

void HHandleDrawPoly::mouseReleaseEvent(HBoard *board, QMouseEvent *event,
                                        const QJsonObject &) {
  if (isButtonPress(event, Qt::MouseButton::RightButton)) {
    if (board) {
      board->clearSelect();
      board->setSelect(_node);
    }
    boardLeaveOffThisHandle(board);
    HHandleMove::mouseReleaseEvent(board, event);
  } else {
    if (board) {
      board->clearSelect();
      board->setSelect(_node);
    }
  }
}

void HHandleDrawPoly::boardLeaveOffThisHandle(HBoard *board) {
  if (board && !_node.isNull()) {
    auto node = board->getNodeById(_node);
    if (node) {
      auto points = node->getPointList();
      if (!points.empty()) {
        while (points.size() > _size) {
          points.pop_back();
        }
        points.push_back(points[0]);
        board->drawNodePoint(_node, points);
      }
    }
  }
  _size = 0;
  _node = "";
}

QJsonObject HHandleDrawPoly::getDefaultParam() { return defaultParam(); }

HHandleDrawFillRect::HHandleDrawFillRect() { _name = "fill rect"; }

void HHandleDrawFillRect::mousePressEvent(HBoard *board, QMouseEvent *event,
                                          const QJsonObject &o) {
  HHandleMove::mousePressEvent(board, event);
  if (board && event && isButtonPress(event)) {
    _point = board->WCS2LCS(event->pos());
    auto node =
        std::make_shared<HShapeFillRectNode>(QRectF(_point, QSize(0, 0)), o);
    commandNode(board, node);
    _node = node->id();
  }
}

HHandleDrawFillPoly::HHandleDrawFillPoly() { _name = "fill poly"; }

void HHandleDrawFillPoly::boardLeaveOffThisHandle(HBoard *board) {
  if (board && !_node.isNull()) {
    board->updateNodeDrawMode(_node, GL_POLYGON);
    HHandleDrawPoly::boardLeaveOffThisHandle(board);
  }
}

HHandleDrawCircle::HHandleDrawCircle() { _name = "circle"; }

void HHandleDrawCircle::hoverMoveEvent(HBoard *board, QHoverEvent *event,
                                       const QJsonObject &object) {
  if (board && event) {
    auto center = board->WCS2LCS(event->pos());
    updateCirclePosition(board, center, object);
  }
  HHandleMove::hoverMoveEvent(board, event, object);
}

void HHandleDrawCircle::mouseMoveEvent(HBoard *board, QMouseEvent *event,
                                       const QJsonObject &object) {
  if (board && event) {
    auto center = board->WCS2LCS(event->pos());
    updateCirclePosition(board, center, object);
  }
  HHandleMove::mouseMoveEvent(board, event, object);
}

void HHandleDrawCircle::mousePressEvent(HBoard *board, QMouseEvent *event,
                                        const QJsonObject &object) {
  HHandleMove::mousePressEvent(board, event, object);
  if (board && event && isButtonPress(event)) {
    auto center = board->WCS2LCS(event->pos());
    auto radius = object.value("radius").toDouble();
    QRectF r(QPointF(center.x() - radius, center.y() - radius),
             QSizeF(radius * 2, radius * 2));
    auto node = std::make_shared<HShapeEllipseNode>(r, object);
    commandNode(board, node);
    _node = node->id();
  }
}

void HHandleDrawCircle::boardLeaveOffThisHandle(HBoard *board) {
  if (board) {
    board->removeNode(_circle_node);
    _circle_node = "";
  }
}

void HHandleDrawCircle::hoverLeaveEvent(HBoard *board, QHoverEvent *event,
                                        const QJsonObject &object) {
  if (!_circle_node.isNull() && board) {
    board->removeNode(_circle_node);
    _circle_node = "";
  }
  HHandleMove::hoverLeaveEvent(board, event, object);
}

QJsonObject HHandleDrawCircle::getDefaultParam() {
  QJsonObject object = defaultParam();
  object.insert("radius", 50);
  return object;
}

void HHandleDrawCircle::updateCirclePosition(HBoard *board,
                                             const QPointF &center,
                                             const QJsonObject &object) {
  if (!board) return;
  if (_circle_node.isNull()) {
    auto node = std::make_shared<HShapeCircleNode>(center, object);
    _circle_node = node->id();
    board->pushNode(node);
  } else {
    auto r = object.value("radius").toInt();
    auto radius = (0 == r ? 50 : object.value("radius").toInt());
    auto list = HCommon::BuildCircle(center, radius, 360);
    board->drawNodePoint(_circle_node, list);
  }
}

HHandleDrawFillCircle::HHandleDrawFillCircle() { _name = "fill circle"; }

void HHandleDrawFillCircle::mousePressEvent(HBoard *board, QMouseEvent *event,
                                            const QJsonObject &object) {
  HHandleMove::mousePressEvent(board, event);
  if (board && event && isButtonPress(event)) {
    auto center = board->WCS2LCS(event->pos());

    auto radius = object.value("radius").toDouble();
    QRectF r(QPointF(center.x() - radius, center.y() - radius),
             QSizeF(radius * 2, radius * 2));
    auto node = std::make_shared<HShapeFillEllipseNode>(r, object);
    commandNode(board, node);
    _node = node->id();
  }
}

void HHandleDrawFillCircle::updateCirclePosition(HBoard *board,
                                                 const QPointF &center,
                                                 const QJsonObject &object) {
  if (!board) return;
  if (_circle_node.isNull()) {
    auto node = std::make_shared<HShapeFillCircleNode>(center, object);
    _circle_node = node->id();
    board->pushNode(node);
  } else {
    auto list = HCommon::BuildCircle(center,
                                     0 == object.value("radius").toInt()
                                         ? 50
                                         : object.value("radius").toInt(),
                                     360);
    board->drawNodePoint(_circle_node, list);
  }
}

HHandleDrawEllipse::HHandleDrawEllipse() { _name = "ellipse"; }

void HHandleDrawEllipse::mousePressEvent(HBoard *board, QMouseEvent *event,
                                         const QJsonObject &object) {
  if (board && event && isButtonPress(event, Qt::LeftButton)) {
    auto pos = board->WCS2LCS(event->pos());
    auto node = std::make_shared<HShapeEllipseNode>(QRectF(pos, pos), object);
    commandNode(board, node);
    _node = node->id();
    _point = pos;
  }
  HHandleMove::mousePressEvent(board, event, object);
}

void HHandleDrawEllipse::mouseMoveEvent(HBoard *board, QMouseEvent *event,
                                        const QJsonObject &object) {
  HHandleMove::mouseMoveEvent(board, event, object);
  if (board && event && leftButtonPress(event)) {
    auto pos = board->WCS2LCS(event->pos());
    auto list = HCommon::BuildEllipse(HCommon::BuildRect(_point, pos), 360);
    board->drawNodePoint(_node, list);
  }
}

QJsonObject HHandleDrawEllipse::getDefaultParam() {
  QJsonObject object;
  object.insert("b", 0);
  object.insert("g", 0);
  object.insert("r", 255);
  object.insert("a", 255);
  return object;
}

HHandleDrawFillEllipse::HHandleDrawFillEllipse() { _name = "fill ellipse"; }

void HHandleDrawFillEllipse::mousePressEvent(HBoard *board, QMouseEvent *event,
                                             const QJsonObject &object) {
  if (board && event && isButtonPress(event, Qt::LeftButton)) {
    auto pos = board->WCS2LCS(event->pos());
    auto node =
        std::make_shared<HShapeFillEllipseNode>(QRectF(pos, pos), object);
    commandNode(board, node);
    _node = node->id();
    _point = pos;
  }
  HHandleMove::mousePressEvent(board, event, object);
}

void HHandleDrawFillEllipse::mouseMoveEvent(HBoard *board, QMouseEvent *event,
                                            const QJsonObject &object) {
  HHandleMove::mouseMoveEvent(board, event, object);
  if (board && event && leftButtonPress(event)) {
    auto pos = board->WCS2LCS(event->pos());
    auto list = HCommon::BuildEllipse(HCommon::BuildRect(_point, pos), 360);
    board->drawNodePoint(_node, list);
  }
}

QJsonObject HHandleDrawFillEllipse::getDefaultParam() {
  QJsonObject object;
  object.insert("b", 0);
  object.insert("g", 0);
  object.insert("r", 255);
  object.insert("a", 255);
  return object;
}
