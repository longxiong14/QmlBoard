#include "hjsoncommon.h"

#include <QDebug>
#include <QFile>
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#define DEBUG qDebug() << __FUNCTION__ << " " << __LINE__ << " "
HJsonCommon::HJsonCommon() {}

int HJsonCommon::readJson(const QString &path, QJsonDocument &doc) {
  if (!QFile::exists(path)) {
    DEBUG << " fileName = " << path << " is not exist will return";
    return -1;
  }
  QByteArray value;
  QFile file(path);
  if (file.open(QFile::ReadOnly)) {
    value = file.readAll();
  } else {
    DEBUG << "open file read only error : " << path;
    file.close();
    return -1;
  }
  file.close();
  QJsonParseError jsonError;
  doc = QJsonDocument::fromJson(value, &jsonError);
  if (jsonError.error != QJsonParseError::NoError) {
    DEBUG << "file convert to Json error : " << path;
    return -1;
  }
  return 0;
}

int HJsonCommon::readJsonObject(const QString &path, QJsonObject &o) {
  QJsonDocument doc;
  if (0 != readJson(path, doc)) {
    DEBUG << "read json document error : " << path;
    return -1;
  }
  if (!doc.isObject()) {
    DEBUG << "read json document isn't object " << path;
    return -1;
  }
  o = doc.object();
  return 0;
}

int HJsonCommon::readJsonArray(const QString &path, QJsonArray &a) {
  QJsonDocument doc;
  if (0 != readJson(path, doc)) {
    DEBUG << "read json document error : " << path;
    return -1;
  }
  if (!doc.isObject()) {
    DEBUG << "read json document isn't array " << path;
    return -1;
  }
  a = doc.array();
  return 0;
}

int HJsonCommon::writeJson(const QString &path, const QJsonValue &value) {
  if (path.isNull() || path.isEmpty()) {
    DEBUG << " file path is empty";
    return -1;
  }
  if (!fs::exists(fs::path(path.toStdString()).parent_path())) {
    fs::create_directories(fs::path(path.toStdString()).parent_path());
  }
  QString tmpFile = path + ".temp";
  QFile file(tmpFile);
  if (!file.open(QIODevice::OpenModeFlag::WriteOnly)) {
    DEBUG << "create file by write only error " << path;
    file.close();
    return -1;
  }

  QJsonDocument doc;
  if (value.isArray()) {
    doc.setArray(value.toArray());
  } else if (value.isObject()) {
    doc.setObject(value.toObject());
  } else {
    DEBUG << "input json value is not json array or json object";
    return -1;
  }
  QByteArray array = doc.toJson(QJsonDocument::JsonFormat::Indented);
  if (array.size() != file.write(array)) {
    file.close();
    return -1;
  }
  file.close();

  if (fs::exists(path.toStdString())) {
    fs::remove(path.toStdString());
  }
  if (fs::exists(tmpFile.toStdString()) && !fs::exists(path.toStdString())) {
    fs::rename(tmpFile.toStdString(), path.toStdString());
    return 0;
  }
  return -1;
}
