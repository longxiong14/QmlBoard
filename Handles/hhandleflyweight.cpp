#include "hhandleflyweight.h"

#include <QDebug>

#include "hhandlearrow.h"
#include "hhandlebase.h"
#include "hhandlemove.h"
#include "hhandlepaint.h"

#define DEBUG qDebug() << __FUNCTION__ << " " << __LINE__ << " "
HHandleFlyWeight *HHandleFlyWeight::getInstance() {
  static HHandleFlyWeight weight;
  return &weight;
}

int HHandleFlyWeight::registHandle(const QString &key,
                                   std::shared_ptr<HHandleBase> handle) {
  if (_handles.contains(key)) {
    return -1;
  }
  _handles.insert(key, handle);
  return 0;
}

QMap<QString, std::shared_ptr<HHandleBase> > HHandleFlyWeight::handles() {
  return _handles;
}

std::shared_ptr<HHandleBase> HHandleFlyWeight::getHandle(const QString &key) {
  if (_handles.contains(key)) {
    return _handles.value(key);
  }
  return nullptr;
}

QJsonObject HHandleFlyWeight::getBoardHandleParam(const QString &board,
                                                  const QString &handle) {
  QJsonObject object;
  if (_handles_params.contains(board) &&
      _handles_params[board].contains(handle)) {
    object = _handles_params[board][handle];
  } else {
    if (_handles.contains(handle)) {
      object = _handles[handle]->getDefaultParam();
    }
  }
  return object;
}

int HHandleFlyWeight::setBoardHandleParam(const QString &board,
                                          const QString &handle,
                                          const QString &key,
                                          const QJsonValue &value) {
  if (key.isEmpty()) return -1;
  if (_handles_params.contains(board)) {
    if (_handles_params[board].contains(handle)) {
      _handles_params[board][handle].insert(key, value);
    } else {
      if (_handles.contains(handle)) {
        auto object = _handles[handle]->getDefaultParam();
        object.insert(key, value);
        _handles_params[board][handle] = object;
      } else {
        QJsonObject object;
        object.insert(key, value);
        _handles_params[board][handle] = object;
      }
    }
  } else {
    if (_handles.contains(handle)) {
      auto object = _handles[handle]->getDefaultParam();
      object.insert(key, value);
      _handles_params[board][handle] = object;
    } else {
      QJsonObject object;
      object.insert(key, value);
      _handles_params[board][handle] = object;
    }
  }
  return 0;
}

HHandleFlyWeight::HHandleFlyWeight() {
  auto func = [=](std::shared_ptr<HHandleBase> ptr) {
    _handles.insert(ptr->getName(), ptr);
  };
  func(std::make_shared<HHandleArrow>());
  func(std::make_shared<HHandleDrawPoly>());
  func(std::make_shared<HHandleDrawRect>());
  func(std::make_shared<HHandleDrawLine>());
  func(std::make_shared<HHandleDrawCurve>());
  func(std::make_shared<HHandleMove>());
  func(std::make_shared<HHandleDrawFillRect>());
  func(std::make_shared<HHandleDrawFillPoly>());
  func(std::make_shared<HHandleDrawCircle>());
  func(std::make_shared<HHandleDrawFillCircle>());
  //  _handles = {{"arrow", std::make_shared<HHandleArrow>()},
  //              {"poly", std::make_shared<HHandleDrawPoly>()},
  //              {"rect", std::make_shared<HHandleDrawRect>()},
  //              {"line", std::make_shared<HHandleDrawLine>()},
  //              {"curve", std::make_shared<HHandleDrawCurve>()},
  //              {"none", std::make_shared<HHandleMove>()},
  //              {"fill rect", std::make_shared<HHandleDrawFillRect>()},
  //              {"fill poly", std::make_shared<HHandleDrawFillPoly>()}};
}
