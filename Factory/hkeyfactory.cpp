#include "hkeyfactory.h"

#include <QDebug>

#include "../Operators/hboardactionbase.h"
#include "../Operators/hcommandbase.h"
#include "../hboard.h"
HKeyFactoryBase::HKeyFactoryBase() {}

HKeyFactoryBase::~HKeyFactoryBase() {}

void HKeyFactoryBase::clearKeys() { _keys.clear(); }

QSet<int> HKeyFactoryBase::keys() { return _keys; }

HKeyFactory::HKeyFactory() {}

void HKeyFactory::keyPressEvent(HBoard *board, QKeyEvent *event) {
  if (event && board) {
    _keys.insert(event->key());
    switch (event->key()) {
      case Qt::Key_Delete: {
        auto command = board->getCommand();
        if (command) {
          auto action = std::make_shared<HRemoveNodeAction>(board->name(),
                                                            board->selects());
          command->excute(action);
        } else {
          board->removeSelectNode();
        }
      } break;
      case Qt::Key_H:
        if (_keys.contains(Qt::Key_Control)) {
          board->home();
        }
        break;
      case Qt::Key_D:
        if (_keys.contains(Qt::Key_Control)) {
          board->setScale(1.0);
          board->face(board->width() * 0.5, board->height() * 0.5);
        }
        break;
      case Qt::Key_Escape:
        board->clearSelect();
        break;
      case Qt::Key_Z:
        if (_keys.contains(Qt::Key_Control)) {
          auto command = board->getCommand();
          if (command) command->undo();
        }
        break;
      case Qt::Key_Y:
        if (_keys.contains(Qt::Key_Control)) {
          auto command = board->getCommand();
          if (command) command->redo();
        }
        break;
    }
  }
}

void HKeyFactory::keyReleaseEvent(HBoard *, QKeyEvent *event) {
  if (event) _keys.remove(event->key());
}
