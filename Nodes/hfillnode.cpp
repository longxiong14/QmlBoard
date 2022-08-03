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
HFillNode::HFillNode() : _flag(false) {}

HFillNode::HFillNode(const QList<QPoint> &points, const QColor &color,
                     unsigned long type)
    : _flag(false) {
  setOurGeometry(points, type);
  setColor(color);
}

HFillNode::HFillNode(const QRect &rect, const QColor &color, unsigned long type)
    : _flag(false) {
  auto list = HCommon::BuildRectList(rect);
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
  if (!_select && _flag) {
    timeOut();
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
  QSGFlatColorMaterial *material = HSGNodeCommon::buildColor(color);
  setMaterial(material);
  setFlag(QSGNode::OwnsMaterial);
}

void HFillNode::timeOut() {
  QSGFlatColorMaterial *m = static_cast<QSGFlatColorMaterial *>(material());
  if (m) {
    auto color = m->color();
    auto r = color.red() ^ 0x000000ff;
    auto g = color.green() ^ 0x000000ff;
    auto b = color.blue() ^ 0x000000ff;
    QColor results(r, g, b);
    setColor(results);
    _flag = !_flag;
  }
}

QSGGeometry *HFillNode::buildGeometry(const QList<QPoint> &points,
                                      unsigned long type) {
  return HSGNodeCommon::buildGeometry(points, type);
}

void HFillNode::setOurGeometry(const QList<QPoint> &points,
                               unsigned long type) {
  QSGGeometry *geometry = buildGeometry(points, type);
  setGeometry(geometry);
  setFlag(QSGNode::OwnsGeometry);
}
