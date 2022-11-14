#include "maincontrol.h"

#include "../../Nodes/himagemapnode.h"
#include "../../hboard.h"
#include "../../hboardmanager.h"
MainControl::MainControl(QObject *parent) : QObject(parent) {}

int MainControl::open(const QString &path) {
  auto thumbail = HBoardManager::getInstance()->getBoard("thunail_board");
  auto max = HBoardManager::getInstance()->getBoard("max_board");
  if (!thumbail || !max) {
    return -1;
  }
  auto image = QImage(path);

  {
    auto node = std::make_shared<HImageMapNode>(
        image.scaled(int(image.width() * 0.1), int(image.height() * 0.1)));
    node->setFlag(HNodeBase::NODEFLAG::CANSELECT, false);
    thumbail->pushNode(node);
    thumbail->home();
  }
  {
    auto node = std::make_shared<HImageMapNode>(image);
    node->setFlag(HNodeBase::NODEFLAG::CANSELECT, false);
    max->pushNode(node);
  }

  return 0;
}
