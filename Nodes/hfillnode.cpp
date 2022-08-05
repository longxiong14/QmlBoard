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
HFillNode::HFillNode(const QList<QPointF> &points, unsigned long type,
                     const QJsonObject &p) {
  _param = p;
  setOurGeometry(points, type);
  setColor(getColor(p));
}

HFillNode::HFillNode(const QRectF &rect, unsigned long type,
                     const QJsonObject &p) {
  _param = p;
  auto list = HCommon::BuildRectList(rect);
  setOurGeometry(list, type);
  setColor(getColor(p));
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
  HNodeBase::move(p);
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

void HFillNode::setParam(const QJsonObject &p) {
  auto pr = p.value("r").toInt(), pg = p.value("g").toInt(),
       pb = p.value("b").toInt(), pa = p.value("a").toInt(),
       pline_width = p.value("line_width").toInt();

  auto r = _param.value("r").toInt(), g = _param.value("g").toInt(),
       b = _param.value("b").toInt(), a = _param.value("a").toInt(),
       line_width = _param.value("line_width").toInt();

  if (line_width != pline_width) {
    if (geometry()) {
      DEBUG << "set line width " << pline_width;
      geometry()->setLineWidth(pline_width);
    }
  }
  if (pr != r || pg != g || pb != b || pa != a) {
    DEBUG << "set color " << getColor(p);
    setColor(getColor(p));
  }
  HNodeBase::setParam(p);
}

QSGGeometry *HFillNode::buildGeometry(const QList<QPointF> &points,
                                      unsigned long type) {
  return HSGNodeCommon::buildGeometry(points, type);
}

void HFillNode::setOurGeometry(const QList<QPointF> &points,
                               unsigned long type) {
  QSGGeometry *geometry = buildGeometry(points, type);
  if (_param.contains("line_width")) {
    geometry->setLineWidth(_param.value("line_width").toInt());
  }
  setGeometry(geometry);
  setFlag(QSGNode::OwnsGeometry);
}

QColor HFillNode::getColor(const QJsonObject &p) {
  if (p.empty()) {
    return Qt::red;
  }
  auto color = QColor(p.value("r").toInt(), p.value("g").toInt(),
                      p.value("b").toInt(), p.value("a").toInt());
  return color;
}
