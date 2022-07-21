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

HHandleFlyWeight::HHandleFlyWeight() {
  _handles = {{"arrow", std::make_shared<HHandleArrow>()},
              {"ploy", std::make_shared<HHandleDrawPoly>()},
              {"rect", std::make_shared<HHandleDrawRect>()},
              {"line", std::make_shared<HHandleDrawLine>()},
              {"curve", std::make_shared<HHandleDrawCurve>()},
              {"none", std::make_shared<HHandleMove>()},
              {"fill rect", std::make_shared<HHandleDrawFillRect>()},
              {"fill poly", std::make_shared<HHandleDrawFillPoly>()}};
}
