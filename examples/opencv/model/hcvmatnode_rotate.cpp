#include "hcvmatnode_rotate.h"

#include <QDebug>
#include <QPainter>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#include "../../Common/hcommons.h"
#include "hcvmatnode2.h"
#define DEBUG qDebug() << __FUNCTION__ << __LINE__
HCVMatRotateNode::HCVMatRotateNode(const QString &path)
    : HImageMapNodeDelegate() {
  _mat = cv::imread(path.toLocal8Bit().toStdString(), cv::IMREAD_UNCHANGED);

  auto m = cv::getRotationMatrix2D(cv::Point(), -30, 1.0);
  cv::warpAffine(_mat, _mat, m, cv::Size(3000, 3000));

  auto rect = QRect(QPoint(), QSize(_mat.cols, _mat.rows));
  setPointList(HCommon::RotateRect(rect, 30, QPointF()));
  setColor(Qt::red);
}

HCVMatRotateNode::~HCVMatRotateNode() {}

QImage HCVMatRotateNode::getImage(const QRectF &roi, double scale) {
  QImage q;

  auto src_rect =
      cv::Rect(int(roi.x()), int(roi.y()), int((int(roi.width() + 1))),
               int((int(roi.height()) + 1)));
  src_rect &= cv::Rect(0, 0, _mat.cols, _mat.rows);
  auto d = _mat(src_rect);
  if (d.cols * scale > 1 && d.rows * scale > 1) {
    cv::resize(d, d, cv::Size(), scale, scale, cv::INTER_NEAREST);

    auto dst_roi = QRectF((roi.x() - int(roi.x())) * scale,
                          (roi.y() - int(roi.y())) * scale, roi.width() * scale,
                          roi.height() * scale);
    cv::Rect int_rect(int(dst_roi.x()), int(dst_roi.y()), int(dst_roi.width()),
                      int(dst_roi.height()));

    auto dst_rect = int_rect & cv::Rect(0, 0, d.cols, d.rows);
    auto out = d(dst_rect);
    q = HCVMatNode::CVMat2Qimage(out);
  }
  return q;
}
