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

HPushNodeAction::~HPushNodeAction() {}

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

HRemoveNodeAction::HRemoveNodeAction(const QString &name, const QUuid &id)
    : _board_name(name), _id(id), _node(nullptr) {}

HRemoveNodeAction::~HRemoveNodeAction() {}

int HRemoveNodeAction::excute() {
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
  _node = node;
  board->removeNode(_id);
  return 0;
}

int HRemoveNodeAction::undo() {
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

HHandeUpdatePointsAction::HHandeUpdatePointsAction(const QString &name,
                                                   const QUuid &id, int size,
                                                   const QList<QPointF> &points)
    : _board_name(name), _id(id), _size(size), _points(points) {}

HHandeUpdatePointsAction::~HHandeUpdatePointsAction() {}

int HHandeUpdatePointsAction::excute() {  //
  return 0;
}

int HHandeUpdatePointsAction::undo() {  //
  return 0;
}
