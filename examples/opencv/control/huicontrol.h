#ifndef HUICONTROL_H
#define HUICONTROL_H
#include <QObject>
#include <opencv2/core.hpp>
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
  int blur(const QJsonObject& size, const QJsonObject& point);

  Q_INVOKABLE
  int translate(const QJsonObject& dlt_point);

  Q_INVOKABLE
  int test();

 protected:
  HCVMatBoard* getSingleSource();
  HCVMatBoard* getDest();

  cv::Size getSize(const QJsonObject& param);
  cv::Point getPoint(const QJsonObject& param);

 signals:
  void sigError(const QString& error);

 public slots:

 protected:
  HUIModel* _model;
};

#endif  // HUICONTROL_H
