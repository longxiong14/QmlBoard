#ifndef HCVMATNODE_H
#define HCVMATNODE_H
#include <opencv2/core.hpp>

#include "../HBoard_global.h"
#include "himagenode.h"
class HBOARD_EXPORT HCVMatNode : public HNodeBase {
 public:
  HCVMatNode(const QString& path, const QPoint& start_point = QPoint());
  HCVMatNode(const cv::Mat& mat, const QPoint& start_point = QPoint());

  virtual QSGNode* build(HBoard*) override;
  virtual QSGNode* get() override;
  virtual QRect getBoundRect() override;
  virtual QList<QPoint> getPointList() override;
  virtual void move(const QPoint&) override;
  virtual SELECTTYPE selectType() override;

 public:
  void setSplitSize(const cv::Size& size);
  cv::Size getSplitSize();

  void setMat(const cv::Mat& mat);
  cv::Mat getMat();

 public:
  QImage CVMat2Qimage(const cv::Mat& mat);
  QSGNode* BuildQImageNode(const QImage& image, HBoard* board,
                           const QRect& rect);

 protected:
  cv::Mat _mat;
  cv::Size _split_size;
  QSGNode* _node;
  QRect _bound_rect;
  QPoint _start_point;
};

#endif  // HCVMATNODE_H
