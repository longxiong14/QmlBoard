#ifndef HCVMATBOARD_H
#define HCVMATBOARD_H

#include <opencv2/core.hpp>

#include "../../himagemapboard.h"
class HCVMatNode2;
class HCVMatBoard : public HImageMapBoard {
  Q_OBJECT
  Q_PROPERTY(QString msg READ msg WRITE setMsg NOTIFY msgChanged)
 public:
  HCVMatBoard(QQuickItem* parent = nullptr);

 public:
  int setMatNode(std::shared_ptr<HCVMatNode2> node);
  int setMatNode(cv::Mat mat);

  cv::Mat getMatNode();

  void removeMatNode();

 public:
  void setMsg(const QString& msg);
  QString msg();
 signals:
  void msgChanged();

 protected:
  void setMatMsg(cv::Mat mat);

 protected:
  std::shared_ptr<HCVMatNode2> _mat_node;
  QString _msg;
};

#endif  // HCVMATBOARD_H
