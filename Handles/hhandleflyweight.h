#ifndef HHANDLEFLYWEIGHT_H
#define HHANDLEFLYWEIGHT_H
#include <QJsonObject>
#include <QMap>

#include "../HBoard_global.h"
class HHandleBase;
class HBOARD_EXPORT HHandleFlyWeight {
 public:
  static HHandleFlyWeight* getInstance();

  template <class T>
  static int registHandle();

  int registHandle(const QString& key, std::shared_ptr<HHandleBase> handle);

  QMap<QString, std::shared_ptr<HHandleBase>> handles();

  std::shared_ptr<HHandleBase> getHandle(const QString& key);

  QJsonObject getBoardHandleParam(const QString& board, const QString& handle);

  int setBoardHandleParam(const QString& board, const QString& handle,
                          const QString& key, const QJsonValue& value);

 protected:
  HHandleFlyWeight();

 protected:
  QMap<QString, std::shared_ptr<HHandleBase>> _handles;
  QMap<QString, QMap<QString, QJsonObject>> _handles_params;
};

template <class T>
int HHandleFlyWeight::registHandle() {
  auto instance = getInstance();
  if (!instance) return -1;
  auto ptr = std::make_shared<T>();
  instance->registHandle(ptr->getName(), ptr);
  return 0;
}

#endif  // HHANDLEFLYWEIGHT_H
