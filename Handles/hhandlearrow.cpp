#include "hhandlearrow.h"

#include <QDebug>
#include <QMatrix>
#include <QSGNode>

#include "../Common/hcommons.h"
#include "../Common/hplanvector.h"
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
      auto node = std::make_shared<HFillNode>(QRectF(0, 0, 1, 1));
      board->pushNode(node);
      _select_node = node->id();
      _select_start_point = pos;
    }
  }
}

void HHandleArrow::mouseMoveEvent(HBoard *board, QMouseEvent *event,
                                  const QJsonObject &) {
  if (board && event) {
    if (middleButtonPress(event)) {
      HHandleMove::mouseMoveEvent(board, event);
    } else if (leftButtonPress(event)) {
      if (_can_move) _moved = true;
      auto selects = board->selects();
      if (!selects.empty()) {
        auto nodes = board->visibleNodes();
        auto pos = board->WCS2LCS(event->pos());
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
    if (isButtonPress(event) && !middleButtonPress(event) && !_moved) {
      if (!ctrlKeyPress(board->keys())) {
        board->clearSelect();
      }
      auto nodes = board->visibleNodes();
      double scale = board->getScale();
      for (const auto &n : nodes.values()) {
        if (canSelect(n.get(), pos, scale)) {
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
        DEBUG << k << n->isSelect();
        auto bound = nodes.value(k)->getBoundRect();
        if (HCommon::PointInRect(bound.topLeft(), r) &&
            HCommon::PointInRect(bound.bottomRight(), r)) {
          board->changeSelectStatus(k);
        }
      }
      board->removeNode(_select_node);
      _select_node = "";
    }
  }
  HHandleMove::mouseReleaseEvent(board, event);
}

void HHandleArrow::wheelEvent(HBoard *board, QWheelEvent *event) {
  HHandleMove::wheelEvent(board, event);
}

void HHandleArrow::setDistance(int dis) { _distance = dis; }

int HHandleArrow::getDistance() { return _distance; }

bool HHandleArrow::canSelect(HNodeBase *node, const QPointF &pos,
                             double scale) {
  auto type = node->nodeType();
  auto points = node->getPointList();
  HPlanVector vec;
  switch (type) {
    case HNodeBase::IMAGE:
      if (HCommon::PointInContour(pos, points)) {
        return true;
      }
      break;
    case HNodeBase::SHAPE: {
      auto min = vec.ptmPoly(pos, points);
      if (std::fabs(min) < (_distance / scale)) {
        return true;
      }
    } break;
  }
  return false;
}
