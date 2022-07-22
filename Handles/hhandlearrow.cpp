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

void HHandleArrow::mousePressEvent(HBoard *board, QMouseEvent *event) {
  HHandleMove::mousePressEvent(board, event);
  if (board && event) {
    _move = false;
    auto pos = board->WCS2LCS(event->pos());
    auto nodes = board->nodes();
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

void HHandleArrow::mouseMoveEvent(HBoard *board, QMouseEvent *event) {
  if (board && event) {
    if (middleButtonPress(event)) {
      HHandleMove::mouseMoveEvent(board, event);
    } else {
      auto selects = board->selects();
      DEBUG << selects;
      if (!selects.empty()) {
        auto nodes = board->nodes();
        auto pos = board->WCS2LCS(event->pos());
        for (const auto &s : selects) {
          if (nodes.contains(s) && _move) {
            auto node = nodes[s];
            auto dlt = pos - _last_point - board->WCS2LCS(QPoint());
            board->moveNode(node->id(), dlt);
          } else {
            DEBUG << nodes.keys() << " " << s;
          }
        }
        _last_point = pos - board->WCS2LCS(QPoint(0, 0));
      }
    }
  }
}

void HHandleArrow::mouseReleaseEvent(HBoard *board, QMouseEvent *event) {
  if (board && event) {
    if (middleButtonPress(event)) {
      HHandleMove::mouseReleaseEvent(board, event);
    } else {
      if (!ctrlKeyPress(board->keys())) {
        board->clearSelect();
      }
      auto pos = board->WCS2LCS(event->pos());
      auto nodes = board->nodes();
      double scale = board->getScale();
      for (const auto &n : nodes.values()) {
        if (canSelect(n, pos, scale)) board->changeSelectStatus(n->id());
      }
    }
  }
}

void HHandleArrow::wheelEvent(HBoard *board, QWheelEvent *event) {
  HHandleMove::wheelEvent(board, event);
}

void HHandleArrow::setDistance(int dis) { _distance = dis; }

int HHandleArrow::getDistance() { return _distance; }

bool HHandleArrow::canSelect(HNodeBase *node, const QPoint &pos, double scale) {
  auto type = node->selectType();
  auto points = node->getPointList();
  HPlanVector vec;
  switch (type) {
    case HNodeBase::INAREA:
      if (HCommon::PointInContour(pos, points)) {
        return true;
      }
      break;
    case HNodeBase::DISTANCE: {
      auto min = vec.ptmPoly(pos, points);
      DEBUG << min << " " << scale;
      if (std::fabs(min) < (_distance / scale)) {
        return true;
      }
    } break;
  }
  return false;
}
