#ifndef HENUMMODEL_H
#define HENUMMODEL_H

#include <QJsonArray>
#include <QJsonObject>
#include <QObject>
#include <opencv2/core.hpp>

#include "../hqdebug.h"
class HEnumModel : public QObject {
  Q_OBJECT
  HPROPERTY(QJsonObject, borderTypes, borderTypes, setBorderTypes,
            borderTypesChanged)
  HPROPERTY(QJsonArray, borderTypesList, borderTypesList, setBorderTypesList,
            borderTypesListChanged)
 public:
  HDEFINETION(QJsonObject, borderTypes, borderTypes, setBorderTypes,
              borderTypesChanged)
  HDEFINETION(QJsonArray, borderTypesList, borderTypesList, setBorderTypesList,
              borderTypesListChanged)
 signals:
  void borderTypesChanged();
  void borderTypesListChanged();

 public slots:

 public:
  explicit HEnumModel(QObject *parent = nullptr);
};

#endif  // HENUMMODEL_H
