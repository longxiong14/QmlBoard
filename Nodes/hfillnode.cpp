#include "hfillnode.h"

#include <QDebug>
#include <QSGFlatColorMaterial>

#include "../Common/hcommons.h"
#define DEBUG qDebug() << __FUNCTION__ << " " << __LINE__ << " "
HFillNode::HFillNode(const QList<QPoint> &points, const QColor &color,
                     unsigned long type) {
  setOurGeometry(points, type);
  setColor(color);
}

HFillNode::HFillNode(const QRect &rect, const QColor &color,
                     unsigned long type) {
  QList<QPoint> list =
      HCommon::BuildRectList(rect.topLeft(), rect.bottomRight());
  DEBUG << type;
  setOurGeometry(list, type);
  setColor(color);
}

QSGNode *HFillNode::get() { return this; }

QSGNode *HFillNode::build(HBoard *) { return this; }

QRect HFillNode::getBoundRect() {
  QRect r;
  auto geo = geometry();
  if (!geo) return r;
  auto count = geo->vertexCount();
  auto point_list = static_cast<QSGGeometry::Point2D *>(geo->vertexData());
  int left = INT_MAX, right = INT_MIN, bottom = INT_MIN, top = INT_MAX;
  for (int i = 0; i < count; i++) {
    auto p = point_list[i];
    left = std::min(static_cast<int>(p.x), left);
    right = std::max(static_cast<int>(p.x), right);
    bottom = std::max(static_cast<int>(p.y), bottom);
    top = std::min(static_cast<int>(p.y), top);
  }
  r = QRect(left, top, right - left, bottom - top);
  return r;
}

QList<QPoint> HFillNode::getPointList() {
  QList<QPoint> list;
  auto geo = geometry();
  if (!geo) return list;
  auto count = geo->vertexCount();
  auto point_list = static_cast<QSGGeometry::Point2D *>(geo->vertexData());
  for (int i = 0; i < count; i++) {
    auto p = point_list[i];
    list.push_back(QPoint(static_cast<int>(p.x), static_cast<int>(p.y)));
  }
  return list;
}

void HFillNode::move(const QPoint &p) {
  auto geo = geometry();
  if (!geo) return;
  auto count = geo->vertexCount();
  auto point_list = static_cast<QSGGeometry::Point2D *>(geo->vertexData());
  for (int i = 0; i < count; i++) {
    QSGGeometry::Point2D pt = point_list[i];
    pt.set(pt.x + p.x(), pt.y + p.y());
    point_list[i] = pt;
    geo->vertexDataAsPoint2D()[i] = pt;
  }
  setGeometry(geo);
}

void HFillNode::moveTo(const QPoint &p) {}

void HFillNode::changedSelectStatus() {
  HNodeBase::changedSelectStatus();
  if (_select) {
    setColor(Qt::GlobalColor::blue);
  } else {
    setColor(Qt::GlobalColor::red);
  }
}

void HFillNode::drawPoints(const QList<QPoint> &points) {
  auto ptr = geometry();
  if (ptr)
    setOurGeometry(points, ptr->drawingMode());
  else
    setOurGeometry(points);
}

void HFillNode::setColor(const QColor &color) {
  QSGFlatColorMaterial *material = new QSGFlatColorMaterial;
  material->setColor(color);
  setMaterial(material);
  setFlag(QSGNode::OwnsMaterial);
}

QSGGeometry *HFillNode::buildGeometry(const QList<QPoint> &points,
                                      unsigned long type) {
  QSGGeometry *geometry =
      new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), points.size());
  GL_LINE_LOOP;
  geometry->setDrawingMode(static_cast<unsigned long>(type));
  for (int i = 0; i < points.size(); i++) {
    geometry->vertexDataAsPoint2D()[i].set(points[i].x(), points[i].y());
  }
  return geometry;
}

void HFillNode::setOurGeometry(const QList<QPoint> &points,
                               unsigned long type) {
  QSGGeometry *geometry = buildGeometry(points, type);
  setGeometry(geometry);
  setFlag(QSGNode::OwnsGeometry);
}
