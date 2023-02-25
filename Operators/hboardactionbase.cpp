#include "hboardactionbase.h"

#include <QDebug>

#include "../Nodes/hnodebase.h"
#include "../hboard.h"
#include "../hboardmanager.h"
#define DEBUG qDebug() << __FUNCTION__ << __LINE__
HBoardActionBase::HBoardActionBase() {}

HBoardActionBase::~HBoardActionBase() {}

HPushNodeAction::HPushNodeAction(const QString &name,
                                 std::shared_ptr<HNodeBase> node)
    : _board_name(name), _node(node) {}

int HPushNodeAction::excute() {
  if (!_node) {
    DEBUG << "node is nullptr";
    return -1;
  }
  auto board = HBoardManager::getInstance()->getBoard(_board_name);
  if (!board) {
    DEBUG << "get board" << _board_name << "error";
    return -1;
  }
  board->pushNode(_node);
  return 0;
}

int HPushNodeAction::undo() {
  if (!_node) {
    DEBUG << "node is nullptr";
    return -1;
  }
  auto board = HBoardManager::getInstance()->getBoard(_board_name);
  if (!board) {
    DEBUG << "get board" << _board_name << "error";
    return -1;
  }
  board->removeNode(_node->id());
  return 0;
}

HRemoveNodeAction::HRemoveNodeAction(const QString &name, const QSet<QUuid> &id)
    : _board_name(name) {
  for (const auto &n : id) {
    _nodes.insert(n, nullptr);
  }
}

int HRemoveNodeAction::excute() {
  auto board = HBoardManager::getInstance()->getBoard(_board_name);
  if (!board) {
    DEBUG << "get board" << _board_name << "error";
    return -1;
  }
  for (const auto &id : _nodes.keys()) {
    auto node = board->getNodeById(id);
    if (!node) {
      DEBUG << "get node" << id << "from board" << _board_name << "error";
      return -1;
    }
    _nodes[id] = node;
    board->removeNode(id);
  }
  return 0;
}

int HRemoveNodeAction::undo() {
  auto board = HBoardManager::getInstance()->getBoard(_board_name);
  if (!board) {
    DEBUG << "get board" << _board_name << "error";
    return -1;
  }
  for (const auto &node : _nodes) board->pushNode(node);
  return 0;
}

HUpdateNodePointsAction::HUpdateNodePointsAction(const QString &name,
                                                 const QUuid &node_id,
                                                 const QList<QPointF> &list)
    : _board_name(name), _id(node_id), _list(list) {}

int HUpdateNodePointsAction::excute() { return update(); }

int HUpdateNodePointsAction::undo() { return update(); }

int HUpdateNodePointsAction::update() {
  auto board = HBoardManager::getInstance()->getBoard(_board_name);
  if (!board) {
    DEBUG << "get board" << _board_name << "error";
    return -1;
  }
  auto node = board->getNodeById(_id);
  if (!node) {
    DEBUG << "get node" << _id << "from board" << _board_name << "error";
    return -1;
  }
  auto list = node->getPointList();
  board->drawNodePoint(_id, _list);
  _list = list;
  return 0;
}

HMoveNodeAction::HMoveNodeAction(const QString &board, const QSet<QUuid> &nodes,
                                 const QPointF &dlt, bool flag)
    : _board_name(board), _nodes(nodes), _dlt(dlt), _flag(flag) {}

int HMoveNodeAction::excute() {
  if (!_flag) {
    _flag = true;
    return 0;
  }
  auto board = HBoardManager::getInstance()->getBoard(_board_name);
  if (!board) {
    DEBUG << "get board" << _board_name << "error";
    return -1;
  }
  for (const auto &_id : _nodes) {
    board->moveNode(_id, _dlt);
  }
  return 0;
}

int HMoveNodeAction::undo() {
  auto board = HBoardManager::getInstance()->getBoard(_board_name);
  if (!board) {
    DEBUG << "get board" << _board_name << "error";
    return -1;
  }
  for (const auto &_id : _nodes) {
    board->moveNode(_id, -_dlt);
  }
  return 0;
}

HMoveDragNodeAction::HMoveDragNodeAction(const QString &board,
                                         const QUuid &node, int index,
                                         const QPointF &begin,
                                         const QPointF &end, bool flag)
    : _board_name(board),
      _node(node),
      _index(index),
      _begin(begin),
      _end(end),
      _flag(flag) {}

int HMoveDragNodeAction::excute() {
  if (!_flag) {
    _flag = true;
    return 0;
  }
  auto board = HBoardManager::getInstance()->getBoard(_board_name);
  if (!board) {
    DEBUG << "get board" << _board_name << "error";
    return -1;
  }
  board->updateNodeIndexPoint(_node, _index, _end);
  return 0;
}

int HMoveDragNodeAction::undo() {
  auto board = HBoardManager::getInstance()->getBoard(_board_name);
  if (!board) {
    DEBUG << "get board" << _board_name << "error";
    return -1;
  }
  board->updateNodeIndexPoint(_node, _index, _begin);
  return 0;
}
