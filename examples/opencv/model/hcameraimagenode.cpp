#include "hcameraimagenode.h"

#include <QDebug>
#include <QPainter>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#include "hcvmatnode.h"
HCVMatBlurredImage::HCVMatBlurredImage(const cv::Mat &mat, const QPointF &point,
                                       double scale)
    : _scale(scale), _mat(mat) {
  QJsonObject param;
  param.insert("b", 0);
  param.insert("g", 0);
  param.insert("r", 255);
  param.insert("a", 255);
  param.insert("line_width", 1);
  setParam(param);
  setRect(QRectF(point.x(), point.y(), mat.cols / _scale, mat.rows / _scale));
  setFlag(HNodeBase::NODEFLAG::CANSELECT, false);
  setColor(QColor(255, 0, 0, 255));
}

QImage HCVMatBlurredImage::getImage(const QRectF &r, double scale) {
  QImage q;
  auto roi = QRectF(r.x() * _scale, r.y() * _scale, r.width() * _scale,
                    r.height() * _scale);
  auto src_rect =
      cv::Rect(int(roi.x()), int(roi.y()), int((int(roi.width() + 1))),
               int((int(roi.height()) + 1)));
  src_rect &= cv::Rect(0, 0, _mat.cols, _mat.rows);
  auto d = _mat(src_rect);
  scale = scale / _scale;
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

double HCVMatBlurredImage::getScale() { return _scale; }

cv::Mat HCVMatBlurredImage::getMat() { return _mat; }
