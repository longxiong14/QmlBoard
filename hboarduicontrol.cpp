#include "hboarduicontrol.h"

#include <QDebug>

#include "Common/hcommons.h"
#include "Common/hplanvector.h"
#include "Handles/hhandleflyweight.h"
#include "Nodes/hfillnode.h"
#include "Nodes/himagenode.h"
#include "Nodes/hnodebase.h"
#include "hboard.h"
#include "hboardmanager.h"
#define DEBUG qDebug() << __FUNCTION__ << " " << __LINE__ << " "
HBoardUIControl::HBoardUIControl(QObject *parent) : QObject(parent) {}

int HBoardUIControl::setBoardHandle(const QString &board,
                                    const QString &handle) {
  //
  auto ptr = HBoardManager::getInstance()->getBoard(board);
  if (!ptr) {
    DEBUG << "hasn't this board " << board;
    return -1;
  }
  auto h = HHandleFlyWeight::getInstance()->getHandle(handle);
  DEBUG << h.get();
  if (!h) {
    DEBUG << "hasn't this handle " << handle;
    return -1;
  }
  ptr->setHandle(h.get());
  return 0;
}

QJsonArray HBoardUIControl::handleList() {
  QJsonArray list;
  auto h = HHandleFlyWeight::getInstance();
  auto handle = h->handles();
  for (const auto &k : handle.keys()) {
    list.push_back(k);
  }
  return list;
}

void HBoardUIControl::test() {
  auto board = HBoardManager::getInstance()->getBoard("test_board");
  auto nodes = board->nodes();
  HPlanVector vec;
  for (const auto &node : nodes.values()) {
    auto points = node->getPointList();
  }
}
