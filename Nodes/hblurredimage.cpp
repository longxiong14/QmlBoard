
#include "hblurredimage.h"

#include <QPainter>
HBlurredImageNode::HBlurredImageNode(const QImage &image, const QPointF &point,
                                     double scale)
    : HImageMapNodeDelegate(), _scale(scale) {
  QJsonObject param;
  param.insert("b", 0);
  param.insert("g", 0);
  param.insert("r", 255);
  param.insert("a", 255);
  param.insert("line_width", 1);
  setParam(param);
  _image = image;
  setRect(QRectF(point.x(), point.y(), _image.width() / _scale,
                 _image.height() / _scale));
  setColor(QColor(255, 0, 0, 255));
}

QImage HBlurredImageNode::getImage(const QRectF &roi, double scale) {
  auto s = QRectF(0, 0, roi.width() * scale, roi.height() * scale);

  QImage out(s.toRect().size(), _image.format());
  if (!out.isNull()) {
    QPainter painter(&out);
    auto src = QRectF(roi.x() * _scale, roi.y() * _scale, roi.width() * _scale,
                      roi.height() * _scale);
    painter.drawImage(s, _image, src);
  }
  return out;
}
