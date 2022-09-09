#ifndef HNODEFACTORY_H
#define HNODEFACTORY_H
#include <QJsonObject>
#include <memory>
class HNodeBase;
class HNodeFactory {
 public:
  HNodeFactory();

  std::shared_ptr<HNodeBase> create(const QJsonObject& param);
};

#endif  // HNODEFACTORY_H
