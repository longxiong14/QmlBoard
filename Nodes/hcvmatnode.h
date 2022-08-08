#ifndef HCVMATNODE_H
#define HCVMATNODE_H
#include <QImage>
#include <opencv2/core.hpp>

#include "../HBoard_global.h"
#include "hnodebase.h"
class HBOARD_EXPORT HCVMatNode : public HNodeBase {
 public:
  HCVMatNode();
  HCVMatNode(const QString &path, const QPointF &start_point = QPointF());
  HCVMatNode(const cv::Mat &mat, const QPointF &start_point = QPointF());

  virtual QSGNode *build(HBoard *) override;
  virtual QSGNode *get() override;
  virtual QRectF getBoundRect() override;
  virtual QList<QPointF> getPointList() override;
  virtual void move(const QPointF &) override;
  virtual NODETYPE nodeType() override;

 public:
  virtual int save(QJsonObject &o) override;
  virtual int load(const QJsonObject &o) override;
  virtual int save(const QString &path) override;
  virtual int load(const QString &path) override;

 public:
  void setSplitSize(const cv::Size &size);
  cv::Size getSplitSize();

  void setMat(const cv::Mat &mat);
  cv::Mat getMat();

 public:
  QImage CVMat2Qimage(const cv::Mat &mat);
  QSGNode *BuildQImageNode(const QImage &image, HBoard *board,
                           const QRectF &rect);

 protected:
  cv::Mat _mat;
  cv::Size _split_size;
  QSGNode *_node;
  QRectF _bound_rect;
  QPointF _start_point;
};

#endif  // HCVMATNODE_H
