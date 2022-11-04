#include "maincontrol.h"

#include "../../Nodes/hnodebase.h"
#include "../../hboard.h"
#include "../../hboardmanager.h"
MainControl::MainControl(QObject *parent) : QObject(parent) {}

int MainControl::face(const QString &name, int x, int y) {
  auto board = HBoardManager::getInstance()->getBoard(name);
  if (!board) {
    return -1;
  }
  board->face(x, y);
  return 0;
}

int MainControl::moveTo(const QString &name, int x, int y) {
  auto board = HBoardManager::getInstance()->getBoard(name);
  if (!board) {
    return -1;
  }
  auto sel = board->selects();
  for (const auto &n : sel) {
    board->nodeMoveTo(n, QPointF(x, y));
  }

  return 0;
}
