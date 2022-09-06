#include "hsgnodecommon.h"

#include <QPainter>
QSGGeometry *HSGNodeCommon::buildGeometry(const QList<QPointF> &points,
                                          unsigned long type) {
  QSGGeometry *geometry =
      new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), points.size());
  geometry->setDrawingMode(type);
  for (int i = 0; i < points.size(); i++) {
    geometry->vertexDataAsPoint2D()[i].set(float(points[i].x()),
                                           float(points[i].y()));
  }
  return geometry;
}

// QSGGeometry *HSGNodeCommon::updateGeometry(QSGGeometry *geo,
//                                           const QList<QPointF> &list,
//                                           unsigned long type) {
//  if (geo) {
//    for (int i = 0; i < list.size(); i++) {
//      geo->vertexDataAsPoint2D()[i].set(float(list[i].x()),
//      float(list[i].y()));
//    }
//    return geo;
//  } else {
//    return buildGeometry(list, type);
//  }
//}

QSGFlatColorMaterial *HSGNodeCommon::buildColor(const QColor &color) {
  QSGFlatColorMaterial *material = new QSGFlatColorMaterial;
  material->setColor(color);
  return material;
}

QImage HSGNodeCommon::createTextImage(const QString &str, int w, int h,
                                      const QColor &color, int pixel_size) {
  QImage text = QImage(w, h, QImage::Format_ARGB32);
  QPainter painter(&text);
  painter.setCompositionMode(QPainter::CompositionMode_DestinationIn);
  painter.fillRect(0, 0, w, h, QColor(0, 0, 0, 0));
  painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
  auto font = painter.font();
  font.setPointSizeF(pixel_size);
  painter.setFont(font);
  painter.setPen(color);
  painter.drawText(QRectF(0, 0, w, h), str);
  return text;
}

void HSGNodeCommon::releaseTextureNode(QSGImageNode *image) {
  if (image) {
    auto texture = image->texture();
    if (texture) {
      delete image;
      delete texture;
      image = nullptr;
      texture = nullptr;
    }
  }
}

int HSGNodeCommon::imageCopyTo(const QImage &src, const QRect &src_rect,
                               QImage &dst, const QRect &dst_rect) {
  if (src.format() != dst.format()) {
    return -1;
  }
  //  auto r = src_rect & src.rect() & dst.rect() & dst_rect;
  //  auto s_r = src_rect & src.rect();
  //  auto d_r = dst_rect & dst.rect();
  //  auto r = s_r & d_r;
  //  if (r.width() * r.height() <= 0) return -1;

  //  auto s = src.copy(r);
  QPainter painter(&dst);
  painter.drawImage(dst_rect, src.copy(src_rect));
  return 0;
}
