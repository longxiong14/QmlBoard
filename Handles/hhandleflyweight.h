#ifndef HHANDLEFLYWEIGHT_H
#define HHANDLEFLYWEIGHT_H
#include <QMap>

#include "../HBoard_global.h"
class HHandleBase;
class HBOARD_EXPORT HHandleFlyWeight {
 public:
  static HHandleFlyWeight* getInstance();

  int registHandle(const QString& key, std::shared_ptr<HHandleBase> handle);

  QMap<QString, std::shared_ptr<HHandleBase>> handles();

  std::shared_ptr<HHandleBase> getHandle(const QString& key);

 protected:
  HHandleFlyWeight();

 protected:
  QMap<QString, std::shared_ptr<HHandleBase>> _handles;
};

#endif  // HHANDLEFLYWEIGHT_H
