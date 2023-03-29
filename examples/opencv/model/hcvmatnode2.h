#ifndef HCVMATNODE2_H
#define HCVMATNODE2_H

#include <opencv2/core.hpp>

#include "../../Nodes/himagemapnode.h"
class HCVMatNode : public HImageMapNodeDelegate {
 public:
  HCVMatNode(const QString &path);
  HCVMatNode(const cv::Mat &mat);
  HCVMatNode(const cv::Mat &mat, const QPointF &s);

  cv::Mat getMat();
  void setMat(const cv::Mat &m);

  virtual ~HCVMatNode() override;

  virtual QImage getImage(const QRectF &roi, double scale) override;

  virtual int save(QJsonObject &) override { return -1; }
  virtual int load(const QJsonObject &) override { return -1; }

  static QImage CVMat2Qimage(cv::Mat mat);

 protected:
  cv::Mat _mat;
};
#endif  // HCVMATNODE2_H
