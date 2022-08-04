#include "hhandlearrow.h"

#include <QDebug>
#include <QMatrix>
#include <QSGNode>

#include "../Common/hcommons.h"
#include "../Common/hplanvector.h"
#include "Nodes/hnodebase.h"
#include "hboard.h"
#include "hhandlemove.h"
#define DEBUG qDebug() << __FUNCTION__ << " " << __LINE__ << " "

HHandleArrow::HHandleArrow() : HHandleMove(), _move(false), _distance(5) {}

void HHandleArrow::mousePressEvent(HBoard *board, QMouseEvent *event,
                                   const QJsonObject &) {
  HHandleMove::mousePressEvent(board, event);
  if (board && event) {
    _move = false;
    auto pos = board->WCS2LCS(event->pos());
    auto nodes = board->visibleNodes();
    double scale = board->getScale();
    HPlanVector vec;
    for (const auto &n : nodes.values()) {
      if (canSelect(n, pos, scale)) {
        _move = true;
        break;
      }
    }
  }
}

void HHandleArrow::mouseMoveEvent(HBoard *board, QMouseEvent *event,
                                  const QJsonObject &) {
  if (board && event) {
    if (middleButtonPress(event)) {
      HHandleMove::mouseMoveEvent(board, event);
    } else {
      auto selects = board->selects();
      if (!selects.empty()) {
        auto nodes = board->visibleNodes();
        auto pos = board->WCS2LCS(event->pos());
        for (const auto &s : selects) {
          if (nodes.contains(s) && _move) {
            auto node = nodes[s];
            auto dlt = pos - _last_point - board->WCS2LCS(QPointF());
            board->moveNode(node->id(), dlt);
          } else {
          }
        }
        _last_point = pos - board->WCS2LCS(QPointF(0, 0));
      }
    }
  }
}

void HHandleArrow::mouseReleaseEvent(HBoard *board, QMouseEvent *event,
                                     const QJsonObject &) {
  if (board && event) {
    if (!middleButtonPress(event)) {
      if (!ctrlKeyPress(board->keys())) {
        board->clearSelect();
      }
      auto pos = board->WCS2LCS(event->pos());
      auto nodes = board->visibleNodes();
      double scale = board->getScale();
      for (const auto &n : nodes.values()) {
        if (canSelect(n, pos, scale)) board->changeSelectStatus(n->id());
      }
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
    case HNodeBase::RECTANGLE:
      if (HCommon::PointInContour(pos, points)) {
        return true;
      }
      break;
    case HNodeBase::POLY: {
      auto min = vec.ptmPoly(pos, points);
      if (std::fabs(min) < (_distance / scale)) {
        return true;
      }
    } break;
  }
  return false;
}
