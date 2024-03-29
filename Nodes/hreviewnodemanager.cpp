﻿#include "hreviewnodemanager.h"

#include <QDebug>

#include "../hboard.h"
#include "../hboardmanager.h"
#include "himagemapnode.h"
#include "himagenode.h"
#define DEBUG qDebug() << __FUNCTION__ << " " << __LINE__ << " "
HReviewNodeManager::HReviewNodeManager(const QString &name)
    : _board_name(name) {}

HReviewNodeManager::HReviewNodeManager(const HReviewNodeManager &other) {
  _board_name = other._board_name;
}

HReviewNodeManager &HReviewNodeManager::operator=(
    const HReviewNodeManager &other) {
  _board_name = other._board_name;
  return *this;
}

HReviewNodeManager::~HReviewNodeManager() {
  auto instance = HBoardManager::getInstance();
  if (!instance) {
    DEBUG << "board manager instace is nullptr";
    return;
  }
  auto board = instance->getBoard(_board_name);
  if (!board) {
    DEBUG << "hasn't this board ";
    return;
  }
  for (const auto &id : _nodes) {
    board->removeNode(id);
  }
}

int HReviewNodeManager::pushNode(const QString &key,
                                 std::shared_ptr<HNodeBase> node) {
  if (_nodes.contains(key)) {
    DEBUG << "had this node" << key;
    return -1;
  }
  if (!node) {
    DEBUG << "node is nullptr";
    return -1;
  }
  auto instance = HBoardManager::getInstance();
  if (!instance) {
    DEBUG << "board manager instace is nullptr";
    return -1;
  }
  auto board = instance->getBoard(_board_name);
  if (!board) {
    DEBUG << "hasn't this board ";
    return -1;
  }
  board->pushNode(node);
  _nodes.insert(key, node->id());
  return 0;
}

int HReviewNodeManager::pushNode(const QString &key, const QImage &mat,
                                 const QPointF &start_point) {
  if (_nodes.contains(key)) {
    DEBUG << "had this node " << key;
    return -1;
  }
  if (mat.isNull()) {
    DEBUG << "mat is empty";
    return -1;
  }
  std::shared_ptr<HImageNode> node =
      std::make_shared<HImageNode>(mat, start_point);
  return pushNode(key, node);
}

int HReviewNodeManager::pushMapImage(const QString &key, const QImage &image,
                                     const QPointF &start_point) {
  if (_nodes.contains(key)) {
    DEBUG << "had this node " << key;
    return -1;
  }
  if (image.isNull()) {
    DEBUG << "mat is empty";
    return -1;
  }
  std::shared_ptr<HImageMapNode> node =
      std::make_shared<HImageMapNode>(image, start_point);
  return pushNode(key, node);
}

int HReviewNodeManager::removeNode(const QString &key) {
  if (!_nodes.contains(key)) {
    return -1;
  }
  auto id = _nodes.value(key);
  _nodes.remove(key);
  auto instance = HBoardManager::getInstance();
  if (!instance) {
    DEBUG << "board manager instace is nullptr";
    return -1;
  }
  auto board = instance->getBoard(_board_name);
  if (!board) {
    DEBUG << "hasn't this board ";
    return -1;
  }
  board->removeNode(id);
  return 0;
}

void HReviewNodeManager::clearButUnremoveNodes() { _nodes.clear(); }

bool HReviewNodeManager::hasReviewNode() { return _nodes.empty(); }

QHash<QString, QUuid> HReviewNodeManager::nodes() { return _nodes; }
