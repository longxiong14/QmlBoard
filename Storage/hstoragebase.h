#ifndef HSTORAGEBASE_H
#define HSTORAGEBASE_H

#include <QJsonObject>
#include <QString>

#include "../HBoard_global.h"
template <class Data>
class HBOARD_EXPORT HStorageBase {
 public:
  HStorageBase() {}
  HStorageBase(const HStorageBase& other) = default;
  HStorageBase& operator=(const HStorageBase&) = default;
  virtual ~HStorageBase() = default;

  virtual int load(const QString& path) = 0;
  virtual int save(const QString& path) = 0;
  virtual int load(const Data& o) = 0;
  virtual int save(Data& d) = 0;
};

// class HBOARD_EXPORT HStorageJsonObject : public HStorageBase<QJsonObject>,
//                                         public QJsonObject {
// public:
//  HStorageJsonObject();

//  virtual int save(QJsonObject& d) override;
//  virtual int load(const QJsonObject& o) override;
//  virtual int save(const QString& path) override;
//  virtual int load(const QString& path) override;
//};

#endif  // HSTORAGEBASE_H
