#include "hqdebug.h"

QDebug operator<<(QDebug d, const std::string& str) {
  d << QString::fromStdString(str);
  return d;
}

QDebug operator<<(QDebug d, const cv::Rect& rect) {
  d << QString("cv::Rect [%1, %2, %3 x %4]")
           .arg(rect.x)
           .arg(rect.y)
           .arg(rect.width)
           .arg(rect.height);
  return d;
}

QDebug operator<<(QDebug d, const cv::Mat& mat) {
  d << QString("cv::Mat size(%1 x %2) channels=%3 type=%4]")
           .arg(mat.cols)
           .arg(mat.rows)
           .arg(mat.channels())
           .arg(mat.type());
  return d;
}

QDebug operator<<(QDebug d, const cv::Point& point) {
  d << QString("cv::Point(%1, %2)").arg(point.x).arg(point.y);
  return d;
}

QDebug operator<<(QDebug d, const cv::Scalar& scalar) {
  d << QString("cv::Scalar(%1, %2, %3, %4)")
           .arg(scalar[0])
           .arg(scalar[1])
           .arg(scalar[2])
           .arg(scalar[3]);
  return d;
}

QDebug operator<<(QDebug d, const cv::Size& point) {
  d << QString("cv::Size(%1, %2)").arg(point.width).arg(point.height);
  return d;
}
