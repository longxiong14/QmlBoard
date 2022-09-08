#include "hboarduicontrol.h"

#include <QDebug>

#include "Common/hcommons.h"
#include "Common/hplanvector.h"
#include "Handles/hhandleflyweight.h"
#include "Nodes/hfillnode.h"
#include "Nodes/himagenode.h"
#include "Nodes/hnodebase.h"
#include "Nodes/hshapenodes.h"
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
  auto weight = HHandleFlyWeight::getInstance();
  if (!weight) {
    DEBUG << "HHandleFlyWeight::getInstance() nullptr";
    return -1;
  }
  auto h = weight->getHandle(handle);
  if (!h) {
    DEBUG << "hasn't this handle " << handle;
    return -1;
  }
  ptr->setHandle(h.get());
  return 0;
}

int HBoardUIControl::openBoardPicture(const QString &board,
                                      const QString &path) {
  //
  auto ptr = HBoardManager::getInstance()->getBoard(board);
  if (!ptr) {
    DEBUG << "hasn't this board " << board;
    return -1;
  }
  auto node = std::make_shared<HImageNode>(path);
  node->setText(node->id().toString());
  ptr->pushNode(node);
  ptr->home();
  return 0;
}

int HBoardUIControl::setBoardHandleParam(const QString &board,
                                         const QString &handle,
                                         const QString &key,
                                         const QJsonValue &value) {
  DEBUG << board << " " << handle << " " << key << " " << value;
  auto weight = HHandleFlyWeight::getInstance();
  if (!weight) {
    DEBUG << "HHandleFlyWeight::getInstance() nullptr";
    return -1;
  }
  return weight->setBoardHandleParam(board, handle, key, value);
}

QJsonObject HBoardUIControl::getBoardHandleParam(const QString &board,
                                                 const QString &handle) {
  QJsonObject o;
  auto ptr = HBoardManager::getInstance()->getBoard(board);
  if (!ptr) {
    DEBUG << "hasn't this board " << board;
    return o;
  }
  auto weight = HHandleFlyWeight::getInstance();
  if (!weight) {
    DEBUG << "HHandleFlyWeight::getInstance() nullptr";
    return o;
  }
  o = weight->getBoardHandleParam(board, handle);
  DEBUG << o;
  return o;
}

int HBoardUIControl::setBoardNodeParam(const QString &board, const QString &key,
                                       const QJsonValue &value) {
  auto ptr = HBoardManager::getInstance()->getBoard(board);
  if (!ptr) {
    DEBUG << "hasn't this board " << board;
    return -1;
  }
  ptr->changeSelectParam(key, value);
  return 0;
}

int HBoardUIControl::saveBoard(const QString &board, const QString &path) {
  auto ptr = HBoardManager::getInstance()->getBoard(board);
  if (!ptr) {
    DEBUG << "hasn't this board " << board;
    return -1;
  }
  return ptr->save(path);
}

int HBoardUIControl::loadBoard(const QString &board, const QString &path) {
  auto ptr = HBoardManager::getInstance()->getBoard(board);
  if (!ptr) {
    DEBUG << "hasn't this board " << board;
    return -1;
  }
  return ptr->load(path);
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

QJsonArray HBoardUIControl::paramToUIItems(const QJsonObject &object) {
  QJsonArray array;
  for (const QString &key : object.keys()) {
    QJsonObject item;
    item.insert("key", key);
    item.insert("value", object.value(key));
    // translate name to chinese
    if (_translate_map.contains(key)) {
      item.insert("name", _translate_map.value(key));
    } else {
      item.insert("name", "");
    }
    array.push_back(item);
  }
  return array;
}

void HBoardUIControl::test() {
  auto board = HBoardManager::getInstance()->getBoard("main_board");
  if (board) {
    std::shared_ptr<HShapeXNode> node =
        std::make_shared<HShapeXNode>(QPointF(), 100, QJsonObject());
    board->pushNode(node);
  }
}

void HBoardUIControl::setTranslateMap(const QJsonObject &object) {
  _translate_map = object;
}
