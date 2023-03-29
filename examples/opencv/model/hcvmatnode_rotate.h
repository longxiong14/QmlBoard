#ifndef HCVMATNODE2_ROTATE_H
#define HCVMATNODE2_ROTATE_H

#include <opencv2/core.hpp>

#include "../../Nodes/himagemapnode.h"
class HCVMatRotateNode : public HImageMapNodeDelegate {
 public:
  HCVMatRotateNode(const QString &path);

  virtual ~HCVMatRotateNode() override;

  virtual QImage getImage(const QRectF &roi, double scale) override;

 protected:
  cv::Mat _mat;
};
#endif  // HCVMATNODE2_H
