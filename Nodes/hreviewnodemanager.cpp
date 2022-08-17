﻿#include "hreviewnodemanager.h"

#include <QDebug>

#include "../hboard.h"
#include "../hboardmanager.h"
#include "hcvmatnode.h"
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

int HReviewNodeManager::pushNode(const QString &key, const cv::Mat &mat,
                                 const QPointF &start_point) {
  std::shared_ptr<HCVMatNode> node =
      std::make_shared<HCVMatNode>(mat, start_point);
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

bool HReviewNodeManager::hasReviewNode() { return _nodes.empty(); }

QHash<QString, QUuid> HReviewNodeManager::nodes() { return _nodes; }