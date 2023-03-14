#ifndef HCVMATBOARD_H
#define HCVMATBOARD_H

#include <opencv2/core.hpp>

#include "../../himagemapboard.h"
class HCVMatNode2;
class HCVMatBoard : public HImageMapBoard {
  Q_OBJECT
  Q_PROPERTY(QString msg READ msg WRITE setMsg NOTIFY msgChanged)
  Q_PROPERTY(bool mask READ mask WRITE setMask NOTIFY maskChanged)
 public:
  HCVMatBoard(QQuickItem* parent = nullptr);

 public:
  void init();

  int setMatNode(std::shared_ptr<HCVMatNode2> node);
  int setMatNode(cv::Mat mat);

  cv::Mat getMatNode();

  void removeMatNode();

 public:
  void setMsg(const QString& msg);
  QString msg();
  void setMask(bool checked);
  bool mask();
 signals:
  void msgChanged();
  void maskChanged();

 protected:
  void setMatMsg(cv::Mat mat);
  void connections();

 public slots:
  void slotMask();

 protected:
  std::shared_ptr<HCVMatNode2> _mat_node;
  QString _msg;
  bool _mask;
  QUuid _mask_node;
};

#endif  // HCVMATBOARD_H
