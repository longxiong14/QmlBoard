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

QSGGeometry *HSGNodeCommon::updateGeometry(QSGGeometry *geo,
                                           const QList<QPointF> &list,
                                           unsigned long type) {
  if (geo) {
    for (int i = 0; i < list.size(); i++) {
      geo->vertexDataAsPoint2D()[i].set(float(list[i].x()), float(list[i].y()));
    }
    return geo;
  } else {
    return buildGeometry(list, type);
  }
}

QSGFlatColorMaterial *HSGNodeCommon::buildColor(const QColor &color) {
  QSGFlatColorMaterial *material = new QSGFlatColorMaterial;
  material->setColor(color);
  return material;
}

QImage HSGNodeCommon::createTextImage(const QString &str, int w, int h) {
  QImage text = QImage(w, h, QImage::Format_ARGB32);
  QPainter painter(&text);
  painter.setCompositionMode(QPainter::CompositionMode_DestinationIn);
  painter.fillRect(0, 0, w, h, QColor(0, 0, 0, 0));
  painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
  auto font = painter.font();
  font.setPointSizeF(10);
  painter.setFont(font);
  painter.setPen(Qt::red);
  painter.drawText(QRectF(0, 0, w, h), str);
  return text;
}
