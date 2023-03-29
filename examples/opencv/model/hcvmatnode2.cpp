#include "hcvmatnode2.h"

#include <QDebug>
#include <QPainter>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#define DEBUG qDebug() << __FUNCTION__ << __LINE__
HCVMatNode::HCVMatNode(const QString &path) : HImageMapNodeDelegate() {
  _mat = cv::imread(path.toLocal8Bit().toStdString(), cv::IMREAD_UNCHANGED);
  setRect(QRect(QPoint(), QSize(_mat.cols, _mat.rows)));
  setFlag(NODEFLAG::CANSELECT, false);
}

HCVMatNode::HCVMatNode(const cv::Mat &mat)
    : HImageMapNodeDelegate(), _mat(mat) {
  setRect(QRect(QPoint(), QSize(_mat.cols, _mat.rows)));
  setFlag(NODEFLAG::CANSELECT, false);
}

HCVMatNode::HCVMatNode(const cv::Mat &mat, const QPointF &s)
    : HImageMapNodeDelegate(), _mat(mat) {
  setRect(QRectF(s, QSizeF(_mat.cols, _mat.rows)));
  setFlag(NODEFLAG::CANSELECT, false);
}

cv::Mat HCVMatNode::getMat() { return _mat; }

void HCVMatNode::setMat(const cv::Mat &m) {
  _mat = m;
  setRect(QRect(QPoint(), QSize(_mat.cols, _mat.rows)));
}

HCVMatNode::~HCVMatNode() {}

// void HCVMatNode2::changedSelectStatus() { setf }

QImage HCVMatNode::CVMat2Qimage(cv::Mat mat) {
  if (mat.type() == CV_8UC1) {
    QImage image(mat.cols, mat.rows, QImage::Format_Grayscale8);
    image.setColorCount(256);
    for (int i = 0; i < 256; i++) {
      image.setColor(i, qRgb(i, i, i));
    }
    uchar *pSrc = mat.data;
    for (int row = 0; row < mat.rows; row++) {
      uchar *pDest = image.scanLine(row);
      memcpy(pDest, pSrc, static_cast<std::size_t>(mat.cols));
      pSrc += mat.step;
    }
    return image;
  } else if (mat.type() == CV_8UC3) {
    const uchar *pSrc = const_cast<const uchar *>(mat.data);
    QImage image(pSrc, mat.cols, mat.rows, int(mat.step),
                 QImage::Format_RGB888);

    return image.rgbSwapped();
  } else if (mat.type() == CV_8UC4) {
    const uchar *pSrc = const_cast<const uchar *>(mat.data);
    QImage image(pSrc, mat.cols, mat.rows, int(mat.step),
                 QImage::Format_ARGB32);
    return image.copy();
  }
  return QImage();
}

QImage HCVMatNode::getImage(const QRectF &roi, double scale) {
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
    q = CVMat2Qimage(out);
  }
  return q;
}
