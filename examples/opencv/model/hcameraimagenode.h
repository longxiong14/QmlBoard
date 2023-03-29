#ifndef HCAMERAIMAGENODE_H
#define HCAMERAIMAGENODE_H

#include <opencv2/core.hpp>

#include "../../Nodes/himagemapnode.h"

class HCVMatBlurredImage : public HImageMapNodeDelegate {
 public:
  HCVMatBlurredImage(const cv::Mat& mat, const QPointF& point, double scale);

  virtual QImage getImage(const QRectF& roi, double scale) override;

  double getScale();
  cv::Mat getMat();

 protected:
  double _scale;
  cv::Mat _mat;
};

#endif  // HCAMERAIMAGENODE_H
