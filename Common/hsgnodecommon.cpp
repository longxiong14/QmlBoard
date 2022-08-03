﻿#include "hsgnodecommon.h"

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

QSGFlatColorMaterial *HSGNodeCommon::buildColor(const QColor &color) {
  QSGFlatColorMaterial *material = new QSGFlatColorMaterial;
  material->setColor(color);
  return material;
}