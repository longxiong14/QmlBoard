#include "hmultshapenode.h"

#include <QDebug>
#include <QJsonArray>

#include "../Common/hcommons.h"
#include "../Common/hjsoncommon.h"
#include "../Common/hsgnodecommon.h"
#define DEBUG qDebug() << __FUNCTION__ << __LINE__
HMultShapeNode::HMultShapeNode() : _node(nullptr), _dash_node(nullptr) {}

HMultShapeNode::HMultShapeNode(const QList<QList<QPointF>> &contours,
                               unsigned long type, const QJsonObject &p)
    : _node(nullptr), _dash_node(nullptr) {
  _param = p;
  buildNode(contours, type);
}

QSGNode *HMultShapeNode::get() { return _node; }

QSGNode *HMultShapeNode::build(HBoard *) { return _node; }

QRectF HMultShapeNode::getBoundRect() {
  if (!_node) return {};
  QRectF r;
  float left = float(INT_MAX), right = float(INT_MIN), bottom = float(INT_MIN),
        top = float(INT_MAX);
  for (int i = 0; i < _node->childCount(); i++) {
    auto node = _node->childAtIndex(i);
    auto geonode = dynamic_cast<QSGGeometryNode *>(node);
    if (!geonode) continue;
    auto geo = geonode->geometry();
    if (!geo) return r;
    auto count = geo->vertexCount();
    auto point_list = static_cast<QSGGeometry::Point2D *>(geo->vertexData());

    for (int i = 0; i < count; i++) {
      auto p = point_list[i];
      left = std::min(p.x, left);
      right = std::max(p.x, right);
      bottom = std::max(p.y, bottom);
      top = std::min(p.y, top);
    }
  }
  r = QRectF(double(left), double(top), double(right - left),
             double(bottom - top));
  return r;
}

QList<QPointF> HMultShapeNode::getPointList() {
  auto list = HCommon::BuildRectList(getBoundRect());
  return list;
}

void HMultShapeNode::move(const QPointF &p) {
  if (!_node) return;
  for (int i = 0; i < _node->childCount(); i++) {
    auto node = _node->childAtIndex(i);
    auto geonode = dynamic_cast<QSGGeometryNode *>(node);
    if (!geonode) continue;
    auto geo = geonode->geometry();
    if (!geo) continue;
    auto count = geo->vertexCount();
    auto point_list = static_cast<QSGGeometry::Point2D *>(geo->vertexData());
    for (int i = 0; i < count; i++) {
      QSGGeometry::Point2D pt = point_list[i];
      float x = float(double(pt.x) + p.x()), y = float(double(pt.y) + p.y());
      pt.set(x, y);
      point_list[i] = pt;
      geo->vertexDataAsPoint2D()[i] = pt;
    }
    geonode->setGeometry(geo);
  }
}

void HMultShapeNode::setColor(const QColor &color) {
  for (int i = 0; i < _node->childCount(); i++) {
    auto node = _node->childAtIndex(i);
    auto geonode = dynamic_cast<QSGGeometryNode *>(node);
    if (!geonode) continue;
    QSGFlatColorMaterial *material = HSGNodeCommon::buildColor(color);
    geonode->setMaterial(material);
    geonode->setFlag(QSGNode::OwnsMaterial);
  }
}

void HMultShapeNode::setParam(const QJsonObject &p) {
  auto pr = p.value("r").toInt(), pg = p.value("g").toInt(),
       pb = p.value("b").toInt(), pa = p.value("a").toInt(),
       pline_width = p.value("line_width").toInt();

  auto r = _param.value("r").toInt(), g = _param.value("g").toInt(),
       b = _param.value("b").toInt(), a = _param.value("a").toInt(),
       line_width = _param.value("line_width").toInt();

  if (line_width != pline_width) {
    for (int i = 0; i < _node->childCount(); i++) {
      auto node = _node->childAtIndex(i);
      auto geonode = dynamic_cast<QSGGeometryNode *>(node);
      if (!geonode) continue;
      auto geo = geonode->geometry();
      if (!geo) continue;
      geo->setLineWidth(pline_width);
    }
  }
  if (pr != r || pg != g || pb != b || pa != a) {
    DEBUG << "set color " << getColor(p);
    setColor(getColor(p));
  }
  HNodeBase::setParam(p);
}

HNodeBase::NODETYPE HMultShapeNode::nodeType() { return NODETYPE::MULTSHAPE; }

void HMultShapeNode::updateDrawMode(unsigned long mode) {
  if (!_node) return;
  for (int i = 0; i < _node->childCount(); i++) {
    auto node = _node->childAtIndex(i);
    auto geonode = dynamic_cast<QSGGeometryNode *>(node);
    if (!geonode) continue;
    auto geo = geonode->geometry();
    if (!geo) continue;
    geo->setDrawingMode(mode);
  }
}

void HMultShapeNode::changedSelectStatus() {
  if (canSelect()) {
    bool f = true;
    if (isSelect()) {
      f = false;
    }
    setFlag(NODEFLAG::SELECTED, f);
    if (!isSelect()) {
      if (_node) {
        if (_dash_node) {
          _node->removeChildNode(_dash_node);
          delete _dash_node;
          _dash_node = nullptr;
        }
      }
    }
  }
}

void HMultShapeNode::timeOut() {
  DEBUG << "mult shape node timeOut";
  if (!_node) return;
  if (_dash_node) {
    for (int i = 0; i < _node->childCount(); i++) {
      auto node = _node->childAtIndex(i);
      auto dash_base = _dash_node->childAtIndex(i);
      if (!node || !dash_base) continue;
      auto geonode = dynamic_cast<QSGGeometryNode *>(node);
      auto dash = dynamic_cast<QSGGeometryNode *>(dash_base);
      if (!geonode || !dash) continue;
      auto list = getGeometryPoints(geonode);
      updateDrawDash(_dash_node, dash, list, 2);
    }
  } else {
    _dash_node = new QSGNode();
    for (int i = 0; i < _node->childCount(); i++) {
      auto node = _node->childAtIndex(i);
      auto geonode = dynamic_cast<QSGGeometryNode *>(node);
      if (!geonode) continue;
      auto list = getGeometryPoints(geonode);
      QSGGeometryNode *dash = nullptr;
      updateDrawDash(_dash_node, dash, list, 2);
    }
    _node->appendChildNode(_dash_node);
  }
}

int HMultShapeNode::save(QJsonObject &o) {
  if (!_node) return -1;
  QJsonArray l;
  unsigned long type = GL_LINE_LOOP;
  for (int i = 0; i < _node->childCount(); i++) {
    auto node = _node->childAtIndex(i);
    auto geonode = dynamic_cast<QSGGeometryNode *>(node);
    if (!geonode) continue;
    auto list = getGeometryPoints(geonode);
    if (geonode->geometry()) {
      type = geonode->geometry()->drawingMode();
    }
    QJsonArray points;
    for (const auto &point : list) {
      QJsonObject p;
      p.insert("x", double(int(point.x() * 100) / 100.0));
      p.insert("y", double(int(point.y() * 100) / 100.0));
      points.push_back(p);
    }
    l.push_back(points);
  }
  o.insert("drawingMode", int(type));
  o.insert("points", l);
  return HNodeBase::save(o);
}

int HMultShapeNode::load(const QJsonObject &o) {
  HNodeBase::load(o);
  auto type = o.value("drawingMode").toInt();
  auto points = o.value("points").toArray();
  QList<QList<QPointF>> lists;
  for (int i = 0; i < points.size(); i++) {
    auto pts = points[i].toArray();
    QList<QPointF> list;
    for (int j = 0; j < pts.size(); j++) {
      auto point = pts[j].toObject();
      list.push_back(
          QPointF(point.value("x").toDouble(), point.value("y").toDouble()));
    }
    lists.push_back(list);
  }
  buildNode(lists, static_cast<unsigned long>(type));
  return 0;
}

QColor HMultShapeNode::getColor(const QJsonObject &p) {
  if (p.empty()) {
    return Qt::red;
  }
  auto color = QColor(p.value("r").toInt(), p.value("g").toInt(),
                      p.value("b").toInt(), p.value("a").toInt());
  return color;
}

QList<QPointF> HMultShapeNode::getGeometryPoints(QSGGeometryNode *geonode) {
  if (!geonode) return {};
  auto geo = geonode->geometry();
  if (!geo) return {};
  auto count = geo->vertexCount();
  auto point_list = static_cast<QSGGeometry::Point2D *>(geo->vertexData());
  QList<QPointF> list;
  for (int i = 0; i < count; i++) {
    QSGGeometry::Point2D pt = point_list[i];
    list.push_front(QPointF(double(pt.x), double(pt.y)));
  }
  return list;
}

void HMultShapeNode::buildNode(const QList<QList<QPointF>> &lists,
                               unsigned long type) {
  _node = new QSGNode();
  QColor color = getColor(_param);
  for (const auto &list : lists) {
    auto node = new QSGGeometryNode();
    auto geo = HSGNodeCommon::buildGeometry(list, type);
    QSGFlatColorMaterial *material = HSGNodeCommon::buildColor(color);
    node->setMaterial(material);
    node->setGeometry(geo);
    node->setFlag(QSGNode::OwnsMaterial);
    node->setFlag(QSGNode::OwnsGeometry);
    _node->appendChildNode(node);
  }
}
