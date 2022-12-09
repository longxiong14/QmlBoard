#include "hkeyfactory.h"

#include <QDebug>

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
      case Qt::Key_Delete:
        board->removeSelectNode();
        break;
      case Qt::Key_H:
        if (_keys.contains(Qt::Key_Control)) {
          board->home();
        }
        break;
      case Qt::Key_Escape:
        board->clearSelect();
        break;
    }
  }
}

void HKeyFactory::keyReleaseEvent(HBoard *, QKeyEvent *event) {
  if (event) _keys.remove(event->key());
}
