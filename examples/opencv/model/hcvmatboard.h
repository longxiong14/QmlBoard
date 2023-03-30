#ifndef HCVMATBOARD_H
#define HCVMATBOARD_H

#include <QJsonArray>
#include <opencv2/core.hpp>

#include "../../himagemapboard.h"
class HCVMatNode;
class HCVMatBoard : public HImageMapBoard {
  Q_OBJECT
  Q_PROPERTY(QString msg READ msg WRITE setMsg NOTIFY msgChanged)
  Q_PROPERTY(bool mask READ mask WRITE setMask NOTIFY maskChanged)
  Q_PROPERTY(QJsonArray histogram READ histogram WRITE setHistogram NOTIFY
                 histogramChanged)
 public:
  HCVMatBoard(QQuickItem* parent = nullptr);

 public:
  void init();

  int setMatNode(std::shared_ptr<HCVMatNode> node);
  int setMatNode(cv::Mat mat);

  cv::Mat getMatNode();
  cv::Mat getMaskNode();

  void removeMatNode();

 public:
  void setMsg(const QString& msg);
  QString msg();
  void setMask(bool checked);
  bool mask();
  void setHistogram(const QJsonArray& array);
  QJsonArray histogram();

 signals:
  void histogramChanged();
  void msgChanged();
  void maskChanged();

 protected:
  void setMatMsg(cv::Mat mat);
  void connections();

 public slots:
  void slotMask();

 protected:
  std::shared_ptr<HCVMatNode> _mat_node;
  QString _msg;
  bool _mask;
  std::shared_ptr<HCVMatNode> _mask_node;
  QJsonArray _histogram;
};

#endif  // HCVMATBOARD_H
