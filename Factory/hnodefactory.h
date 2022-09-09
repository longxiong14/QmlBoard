#ifndef HNODEFACTORY_H
#define HNODEFACTORY_H
#include <QJsonObject>
#include <memory>

#include "../HBoard_global.h"
class HNodeBase;
class HBOARD_EXPORT HNodeFactory {
 public:
  HNodeFactory();

  std::shared_ptr<HNodeBase> create(const QJsonObject& param);
};

#endif  // HNODEFACTORY_H
