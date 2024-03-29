﻿#ifndef HUICONTROL_H
#define HUICONTROL_H
#include <QObject>
#include <opencv2/core.hpp>

class HUIModel;
class HTranslator;
class HCVMatBoard;

class HUIControl : public QObject {
  Q_OBJECT
 public:
  explicit HUIControl(QObject* parent = nullptr);

  void setModel(HUIModel* ptr);

  HTranslator* getTranslate();

  Q_INVOKABLE
  int openImage(const QString& path);

  Q_INVOKABLE
  int blur(const QJsonObject& size, const QJsonObject& point);

  Q_INVOKABLE
  int translate(const QJsonObject& dlt_point);

  Q_INVOKABLE
  int maskBoard(const QString& name, bool checked);

  Q_INVOKABLE
  int openInrange(const QString& name, const QString& file);

  Q_INVOKABLE
  int test();

 protected:
  HCVMatBoard* getBoardByName(const QString& name);
  HCVMatBoard* getSingleSource();
  HCVMatBoard* getDest();

  cv::Size getSize(const QJsonObject& param);
  cv::Point getPoint(const QJsonObject& param);

 signals:
  void sigError(const QString& error);

 public slots:

 protected:
  HUIModel* _model;
  std::shared_ptr<HTranslator> _translator;
};

#endif  // HUICONTROL_H
