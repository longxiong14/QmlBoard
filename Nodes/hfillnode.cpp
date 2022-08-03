#include "hfillnode.h"

#include <QDebug>
#include <QSGFlatColorMaterial>

#include "../Common/hcommons.h"
#include "../Common/hsgnodecommon.h"
#define DEBUG qDebug() << __FUNCTION__ << " " << __LINE__ << " "
/*
QSGGeometry *geometry =
    new QSGGeometry(QSGGeometry::defaultAttributes_TexturedPoint2D(), int(sz));
auto vertices = geometry -> vertexDataAsTexturedPoint2D();
if (aStyle == "dash") {
  float dis = 0;
  auto rt = 1;
  if (m_trans_node) m_trans_node->matrix().data()[0];
  for (auto i = 0; i < aPointList.size(); ++i) {
    if (i > 0) {
      auto del = aPointList[i] - aPointList[i - 1];
      dis += sqrt(QPointF::dotProduct(del, del));
    }
    vertices[i].set(aPointList[i].x(), aPointList[i].y(), dis * rt / 10, 0);
  }
}
*/
HFillNode::HFillNode() {}

HFillNode::HFillNode(const QList<QPointF> &points, const QColor &color,
                     unsigned long type) {
  setOurGeometry(points, type);
  setColor(color);
}

HFillNode::HFillNode(const QRectF &rect, const QColor &color,
                     unsigned long type) {
  auto list = HCommon::BuildRectList(rect);
  setOurGeometry(list, type);
  setColor(color);
}

QSGNode *HFillNode::get() { return this; }

QSGNode *HFillNode::build(HBoard *) { return this; }

QRectF HFillNode::getBoundRect() {
  QRectF r;
  auto geo = geometry();
  if (!geo) return r;
  auto count = geo->vertexCount();
  auto point_list = static_cast<QSGGeometry::Point2D *>(geo->vertexData());
  float left = float(INT_MAX), right = float(INT_MIN), bottom = float(INT_MIN),
        top = float(INT_MAX);
  for (int i = 0; i < count; i++) {
    auto p = point_list[i];
    left = std::min(p.x, left);
    right = std::max(p.x, right);
    bottom = std::max(p.y, bottom);
    top = std::min(p.y, top);
  }
  r = QRectF(double(left), double(top), double(right - left),
             double(bottom - top));
  return r;
}

QList<QPointF> HFillNode::getPointList() {
  QList<QPointF> list;
  auto geo = geometry();
  if (!geo) return list;
  auto count = geo->vertexCount();
  auto point_list = static_cast<QSGGeometry::Point2D *>(geo->vertexData());
  for (int i = 0; i < count; i++) {
    auto p = point_list[i];
    list.push_back(QPointF(double(p.x), double(p.y)));
  }
  return list;
}

void HFillNode::move(const QPointF &p) {
  auto geo = geometry();
  if (!geo) return;
  auto count = geo->vertexCount();
  auto point_list = static_cast<QSGGeometry::Point2D *>(geo->vertexData());
  for (int i = 0; i < count; i++) {
    QSGGeometry::Point2D pt = point_list[i];
    pt.set(float(double(pt.x) + p.x()), float(double(pt.y) + p.y()));
    point_list[i] = pt;
    geo->vertexDataAsPoint2D()[i] = pt;
  }
  setGeometry(geo);
}

void HFillNode::moveTo(const QPointF &p) {}

void HFillNode::drawPoints(const QList<QPointF> &points) {
  auto ptr = geometry();
  if (ptr)
    setOurGeometry(points, ptr->drawingMode());
  else
    setOurGeometry(points);
}

void HFillNode::setColor(const QColor &color) {
  QSGFlatColorMaterial *material = HSGNodeCommon::buildColor(color);
  setMaterial(material);
  setFlag(QSGNode::OwnsMaterial);
}

// void HFillNode::timeOut() {
//  if (false) {
//    QSGFlatColorMaterial *m = static_cast<QSGFlatColorMaterial *>(material());
//    if (m) {
//      auto color = m->color();
//      auto r = color.red() ^ 0x000000ff;
//      auto g = color.green() ^ 0x000000ff;
//      auto b = color.blue() ^ 0x000000ff;
//      QColor results(r, g, b);
//      setColor(results);
//      _flag = !_flag;
//    }
//  } else {
//    if (!childCount()) {
//      auto list = getPointList();
//      for (int i = 0, j = list.size() - 1; i < list.size(); j = i, i++) {
//        std::swap(list[i], list[j]);
//      }
//      auto g = HSGNodeCommon::buildGeometry(list, GL_LINES);
//      QSGGeometryNode *node = new QSGGeometryNode();
//      node->setGeometry(g);
//      node->setMaterial(HSGNodeCommon::buildColor(Qt::blue));
//      appendChildNode(node);
//    }
//  }
//}

QSGGeometry *HFillNode::buildGeometry(const QList<QPointF> &points,
                                      unsigned long type) {
  return HSGNodeCommon::buildGeometry(points, type);
}

void HFillNode::setOurGeometry(const QList<QPointF> &points,
                               unsigned long type) {
  QSGGeometry *geometry = buildGeometry(points, type);
  setGeometry(geometry);
  setFlag(QSGNode::OwnsGeometry);
}
