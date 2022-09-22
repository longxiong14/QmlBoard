﻿#include "hhandlearrow.h"

#include <QCursor>
#include <QDebug>
#include <QMatrix>
#include <QSGNode>

#include "../Common/hcommons.h"
#include "../Common/hplanvector.h"
#include "../Nodes/hdragnode.h"
#include "Nodes/hfillnode.h"
#include "Nodes/hnodebase.h"
#include "hboard.h"
#include "hhandlemove.h"
#define DEBUG qDebug() << __FUNCTION__ << " " << __LINE__ << " "

HHandleArrow::HHandleArrow()
    : HHandleMove(), _can_move(false), _moved(false), _distance(5) {
  _name = "arrow";
}

void HHandleArrow::mousePressEvent(HBoard *board, QMouseEvent *event,
                                   const QJsonObject &) {
  HHandleMove::mousePressEvent(board, event);
  _can_move = false;
  _moved = false;
  if (board && event) {
    if (!_select_node.isNull()) {
      board->removeNode(_select_node);
    }
    auto pos = board->WCS2LCS(event->pos());
    if (leftButtonPress(event)) {
      auto nodes = board->visibleNodes();
      double scale = board->getScale();
      HPlanVector vec;
      for (const auto &n : nodes.values()) {
        if (canSelect(n.get(), pos, scale)) {
          _can_move = true;
          return;
        }
      }
      _point = pos;
    } else if (rightButtonPress(event)) {
      QJsonObject o;
      o.insert("b", 200);
      o.insert("g", 200);
      o.insert("r", 0);
      o.insert("a", 255);
      auto node =
          std::make_shared<HFillNode>(QRectF(0, 0, 1, 1), GL_LINE_LOOP, o);
      board->pushNode(node);
      _select_node = node->id();
      _select_start_point = pos;
    }
  }
}

void HHandleArrow::mouseMoveEvent(HBoard *board, QMouseEvent *event,
                                  const QJsonObject &) {
  if (board && event) {
    auto pos = board->WCS2LCS(event->pos());
    if (middleButtonPress(event)) {
      HHandleMove::mouseMoveEvent(board, event);
    } else if (leftButtonPress(event)) {
      if (_drag_node) {
        board->updateNodeIndexPoint(_drag_node->getParent(),
                                    _drag_node->getPointIndex(), pos);
      } else {
        if (_can_move) _moved = true;
        auto selects = board->selects();
        if (!selects.empty()) {
          auto nodes = board->visibleNodes();
          for (const auto &s : selects) {
            if (nodes.contains(s) && _can_move) {
              auto node = nodes[s];
              auto dlt = pos - _last_point - board->WCS2LCS(QPointF());
              board->moveNode(node->id(), dlt);
            } else {
            }
          }
          _last_point = pos - board->WCS2LCS(QPointF(0, 0));
        }
      }
    } else if (rightButtonPress(event)) {
      auto list = HCommon::BuildRectList(_select_start_point,
                                         board->WCS2LCS(event->pos()));
      board->drawNodePoint(_select_node, list);
    }
  }
}

void HHandleArrow::mouseReleaseEvent(HBoard *board, QMouseEvent *event,
                                     const QJsonObject &) {
  if (board && event) {
    auto pos = board->WCS2LCS(event->pos());
    if (_drag_node) {
      return;
    }
    if (isButtonPress(event) && !middleButtonPress(event) && !_moved) {
      if (!ctrlKeyPress(board->keys())) {
        board->clearSelect();
      }
      auto nodes = board->visibleNodes();
      double scale = board->getScale();
      for (const auto &n : nodes.values()) {
        if (n && n->canSelect() && canSelect(n.get(), pos, scale)) {
          board->changeSelectStatus(n->id());
        }
      }
      board->checkItems();
    }
    auto r = HCommon::BuildRect(_select_start_point, pos);
    if (isButtonPress(event, Qt::MouseButton::RightButton)) {
      auto nodes = board->nodes();
      for (const auto &k : nodes.keys()) {
        auto n = nodes.value(k);
        if (!n || n->isSelect() || n->id() == _select_node) continue;
        auto bound = nodes.value(k)->getBoundRect();
        if (HCommon::PointInRect(bound.topLeft(), r) &&
            HCommon::PointInRect(bound.bottomRight(), r)) {
          board->changeSelectStatus(k);
        }
      }
    }
    board->removeNode(_select_node);
    _select_node = "";
  }
  HHandleMove::mouseReleaseEvent(board, event);
}

void HHandleArrow::wheelEvent(HBoard *board, QWheelEvent *event) {
  HHandleMove::wheelEvent(board, event);
}

void HHandleArrow::hoverEnterEvent(HBoard *board, QHoverEvent *,
                                   const QJsonObject &) {
  if (board) {
    board->setCursor(Qt::CursorShape::ArrowCursor);
  }
}

void HHandleArrow::hoverMoveEvent(HBoard *board, QHoverEvent *e,
                                  const QJsonObject &) {
  if (board && e) {
    auto sels = board->selects();
    auto pos = board->WCS2LCS(e->pos());
    _drag_node = nullptr;
    for (auto s : sels) {
      auto node = board->getNodeById(s);
      if (node->isSelect() && node->pointInDragNode(pos, _drag_node) &&
          _drag_node) {
        board->setCursor(_drag_node->getCursor());
        return;
      }
    }
    board->setCursor(Qt::CursorShape::ArrowCursor);
  }
}

void HHandleArrow::setDistance(int dis) { _distance = dis; }

int HHandleArrow::getDistance() { return _distance; }

bool HHandleArrow::canSelect(HNodeBase *node, const QPointF &pos,
                             double scale) {
  auto type = node->nodeType();
  auto points = node->getPointList();
  auto rect = node->getBoundRect();
  HPlanVector vec;
  switch (type) {
    case HNodeBase::IMAGE:
    case HNodeBase::MAPINAGE:
      if (HCommon::PointInRect(pos, rect)) {
        return true;
      }
      break;
    default: {
      auto min = vec.ptmPoly(pos, points);
      if (std::fabs(min) < (_distance / scale)) {
        return true;
      }
    } break;
  }
  return false;
}
