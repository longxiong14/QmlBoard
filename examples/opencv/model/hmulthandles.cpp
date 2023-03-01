#include "hmulthandles.h"

#include "../../hboard.h"
#include "../../hboardmanager.h"
HMultHandles::HMultHandles() { _name = "mult_handle"; }

void HMultHandles::mouseMoveEvent(HBoard *, QMouseEvent *event,
                                  const QJsonObject &object) {
  auto boards = HBoardManager::getInstance()->boards();
  for (const auto &b : boards.values()) {
    if (b) {
      HHandleMove::mouseMoveEvent(b, event, object);
      b->sigUpdate();
    }
  }
}

void HMultHandles::wheelEvent(HBoard *, QWheelEvent *event) {
  auto boards = HBoardManager::getInstance()->boards();
  for (const auto &b : boards.values()) {
    if (b) {
      HHandleMove::wheelEvent(b, event);
      b->sigUpdate();
    }
  }
}
