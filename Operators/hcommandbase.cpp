#include "hcommandbase.h"

#include <QDebug>

#include "hboardactionbase.h"
#define DEBUG qDebug() << __FUNCTION__ << __LINE__
HCommandBase::HCommandBase() {}

HCommandBase::~HCommandBase() {}

HCommand::HCommand() {}

int HCommand::pushAction(std::shared_ptr<HBoardActionBase> ptr) {
  if (ptr) {
    clearUndos();
    _excutes.push(ptr);
  }
  return 0;
}

int HCommand::excute(std::shared_ptr<HBoardActionBase> ptr) {
  if (ptr) {
    //    DEBUG << "excute";
    clearUndos();
    ptr->excute();
    _excutes.push(ptr);
  }
  return 0;
}

int HCommand::undo() {
  if (_excutes.empty()) {
    //    DEBUG << "excute actions is empty";
    return -1;
  }
  auto ptr = _excutes.top();
  _excutes.pop();
  if (ptr) {
    ptr->undo();
    _undos.push(ptr);
  }
  return 0;
}

int HCommand::redo() {
  if (_undos.empty()) {
    //    DEBUG << "undo actions is empty";
    return -1;
  }
  auto ptr = _undos.top();
  _undos.pop();
  if (ptr) {
    ptr->excute();
    _excutes.push(ptr);
  }
  return 0;
}

void HCommand::clearExcute() {
  while (!_excutes.empty()) {
    _excutes.pop();
  }
}

void HCommand::clearUndos() {
  while (!_undos.empty()) {
    _undos.pop();
  }
}
