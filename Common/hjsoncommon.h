#ifndef HJSONCOMMON_H
#define HJSONCOMMON_H
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>

#include "../HBoard_global.h"
class HBOARD_EXPORT HJsonCommon {
 public:
  HJsonCommon();

  static int readJson(const QString& path, QJsonDocument& doc);

  static int readJsonObject(const QString& path, QJsonObject& o);

  static int readJsonArray(const QString& path, QJsonArray& a);

  static int writeJson(const QString& path, const QJsonValue& value);
};

#endif  // HJSONCOMMON_H
