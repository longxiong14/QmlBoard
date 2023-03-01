#include "hboardmanager.h"

#include <QMutexLocker>

#include "hboard.h"
HBoardManager::HBoardManager() {}

HBoardManager *HBoardManager::getInstance() {
  static HBoardManager manager;
  return &manager;
}

void HBoardManager::pushBoard(HBoard *board) {
  QMutexLocker lock(&_mutex);
  if (!_boards.contains(board->name())) {
    _boards.insert(board->name(), board);
  }
}

HBoard *HBoardManager::getBoard(const QString &name) {
  QMutexLocker lock(&_mutex);
  HBoard *ptr = nullptr;
  if (_boards.contains(name)) {
    ptr = _boards[name];
  }
  return ptr;
}

int HBoardManager::removeBoard(const QString &name) {
  QMutexLocker lock(&_mutex);
  if (_boards.contains(name)) {
    _boards.remove(name);
  }
  return 0;
}

void HBoardManager::changeBoardName(const QString &old, HBoard *board) {
  QMutexLocker lock(&_mutex);
  if (_boards.contains(old)) {
    _boards.remove(old);
  }
  _boards.insert(board->name(), board);
}

QHash<QString, HBoard *> HBoardManager::boards() { return _boards; }
