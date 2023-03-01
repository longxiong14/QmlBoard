#ifndef HUICONTROL_H
#define HUICONTROL_H

#include <QObject>
class HUIModel;
class HCVMatBoard;
class HUIControl : public QObject {
  Q_OBJECT
 public:
  explicit HUIControl(QObject* parent = nullptr);

  void setModel(HUIModel* ptr);

  Q_INVOKABLE
  int openImage(const QString& path);

  Q_INVOKABLE
  int blur(const QJsonObject& param);

  Q_INVOKABLE
  int test();

 protected:
  HCVMatBoard* getSingleSource();
  HCVMatBoard* getDest();

 signals:
  void sigError(const QString& error);

 public slots:

 protected:
  HUIModel* _model;
};

#endif  // HUICONTROL_H
