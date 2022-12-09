#include "hnodebase.h"

#include <QDebug>
#include <QQuickWindow>
#include <QSGGeometryNode>
#include <QSGNode>

#include "../Common/hcommons.h"
#include "../Common/hjsoncommon.h"
#include "../Common/hsgnodecommon.h"
#include "../hboard.h"
#include "hdragnode.h"
#define STEP 5e3
#define DEBUG qDebug() << __FUNCTION__ << " " << __LINE__ << " "
HNodeBase::HNodeBase()
    : _dash(nullptr),
      _text_node(nullptr),
      _drag_node(nullptr),
      _pixel_size(10),
      _flag(CANSELECT | CANDESTORY | VISIBLE | EABLEHOME),
      _z_order(0) {
  _id = QUuid::createUuid();
}

HNodeBase::~HNodeBase() {
  if (_flag & NODEFLAG::CANDESTORY && _text_node) {
    int count = _text_node->childCount();
    for (int i = 0; i < count; i++) {
      auto n = dynamic_cast<QSGImageNode *>(_text_node->childAtIndex(0));
      if (n) HSGNodeCommon::releaseTextureNode(n);
    }
    delete _text_node;
    _text_node = nullptr;
  }
}

QSGNode *HNodeBase::build(HBoard *) { return nullptr; }

QList<QPointF> HNodeBase::getPointList() { return {}; }

QUuid HNodeBase::id() { return _id; }

void HNodeBase::setId(const QUuid &id) { _id = id; }

void HNodeBase::changedSelectStatus() {
  if (canSelect()) {
    bool f = true;
    if (isSelect()) {
      f = false;
    }
    setFlag(NODEFLAG::SELECTED, f);
    if (isSelect()) {
      //      auto node = get();
      //      if (node) {
      //        _drag_node = buildDragNode();
      //        if (_drag_node) {
      //          node->appendChildNode(_drag_node);
      //        }
      //      }
    } else {
      auto n = get();
      if (n) {
        if (_dash) {
          n->removeChildNode(_dash);
          delete _dash;
          _dash = nullptr;
        }
        //        if (_drag_node) {
        //          n->removeChildNode(_drag_node);
        //          delete _drag_node;
        //          _drag_node = nullptr;
        //        }
      }
    }
  }
}

void HNodeBase::move(const QPointF &p) {
  // move _dash
  for (int i = 0; i < _dash_list.size(); i++) {
    _dash_list[i] += p;
  }
  if (_text_node) {
    int count = _text_node->childCount();
    for (int i = 0; i < count; i++) {
      auto n = dynamic_cast<QSGImageNode *>(_text_node->childAtIndex(0));
      if (n) {
        auto rect = n->rect();
        auto dst = QRectF(rect.topLeft() + p, rect.size());
        n->setRect(dst);
      }
    }
  }
}

void HNodeBase::moveTo(const QPointF &p) {
  // move _dash
  auto rect = getBoundRect();
  auto center = rect.center();
  DEBUG << center;
  auto dlt = -QPointF(center.x() - p.x(), center.y() - p.y());
  move(dlt);
  //  flushMayiLine();
}

bool HNodeBase::isSelect() { return _flag & NODEFLAG::SELECTED; }

unsigned long HNodeBase::drawingMode() { return 0; }

void HNodeBase::updateDrawMode(unsigned long) {}

void HNodeBase::setVisible(bool flag) { setFlag(NODEFLAG::VISIBLE, flag); }

bool HNodeBase::visible() { return NODEFLAG::VISIBLE & _flag; }

void HNodeBase::timeOut() {
  auto node = get();
  int line_width = 2;
  if (node && isSelect()) {
    updateDrawDash(node, _dash, _dash_list, line_width);
    //    if (_dash) {
    //      auto geo = _dash->geometry();
    //      if (geo) {
    //        int count = geo->vertexCount();
    //        if (count == _dash_list.count()) {
    //          auto list = _dash_list;
    //          list.pop_front();
    //          auto geo = HSGNodeCommon::buildGeometry(list, GL_LINES);
    //          geo->setLineWidth(line_width);
    //          _dash->setGeometry(geo);
    //        } else {
    //          auto geo = HSGNodeCommon::buildGeometry(_dash_list, GL_LINES);
    //          geo->setLineWidth(line_width);
    //          _dash->setGeometry(geo);
    //        }
    //      }
    //    } else {
    //      QList<QPointF> list;
    //      switch (nodeType()) {
    //        case IMAGE:
    //          list = HCommon::BuildRectLinesList(getBoundRect());
    //          break;
    //        default:
    //          list = HCommon::BuildPolyLinesList(getPointList());
    //          break;
    //      }
    //      if (list.empty()) return;
    //      if (list.size() > STEP) {
    //        double step = list.size() / STEP;
    //        _dash_list.clear();
    //        for (double i = 0; i < list.size(); i += step) {
    //          _dash_list.push_back(list[int(i)]);
    //        }
    //      } else {
    //        _dash_list = list;
    //      }
    //      _dash = HSGNodeCommon::buildGeometryNode(_dash_list, Qt::blue,
    //      GL_LINES,
    //                                               line_width);
    //      node->appendChildNode(_dash);
    //    }
  }
}

int HNodeBase::setText(const QString &text, const QRectF &position,
                       int pixel_size) {
  _text = text;
  _text_rect = position;
  _pixel_size = pixel_size;
  return 0;
}

void HNodeBase::setZOrder(int z) { _z_order = z; }

int HNodeBase::getZOrder() { return _z_order; }

QString HNodeBase::getText() { return _text; }

QRectF HNodeBase::getTextRect() { return _text_rect; }

int HNodeBase::getPixelSize() { return _pixel_size; }

bool HNodeBase::enableHome() { return _flag & EABLEHOME; }

void HNodeBase::setEnableHome(bool f) { setFlag(EABLEHOME, f); }

void HNodeBase::setDestory(bool flag) {
  setFlag(HNodeBase::NODEFLAG::CANDESTORY, flag);
}

void HNodeBase::setFlag(HNodeBase::NODEFLAG flag, bool open) {
  if (open) {
    _flag = _flag | flag;
  } else {
    _flag = _flag & ~flag;
  }
}

HNodeBase::NODEFLAG HNodeBase::flag() { return HNodeBase::NODEFLAG(_flag); }

bool HNodeBase::canSelect() { return _flag & NODEFLAG::CANSELECT; }

QSGNode *HNodeBase::buildDragNode(HBoard *) { return nullptr; }

QSGNode *HNodeBase::getDragNode() { return _drag_node; }

void HNodeBase::destroyDragNode() {
  if (_drag_node) {
    _drag_node->removeAllChildNodes();
    delete _drag_node;
    _drag_node = nullptr;
  }
}

bool HNodeBase::pointInDragNode(const QPointF &point, HDragNode *&drag,
                                double scale) {
  if (!_drag_node) return false;
  auto count = _drag_node->childCount();
  if (!count) return false;
  for (int i = 0; i < count; i++) {
    auto node = dynamic_cast<HDragNode *>(_drag_node->childAtIndex(i));
    if (node) {
      if (node->pointIn(point, scale)) {
        drag = node;
        return true;
      }
    }
  }
  return false;
}

void HNodeBase::updateIndexPoint(int index, const QPointF &point) {
  //  if (_drag_node) {
  //    auto size = _drag_node->childCount();
  //    if (size > index && index >= 0) {
  //      HDragNode *drag =
  //          dynamic_cast<HDragNode *>(_drag_node->childAtIndex(index));
  //      if (drag && drag->getPointIndex() == index) {
  //        drag->moveTo(point);
  //      }
  //    }
  //  }
  flushMayiLine();
}

void HNodeBase::updateDragNodePoint(HBoard *board) {
  if (_drag_node && board) {
    auto count = _drag_node->childCount();
    auto pts = getPointList();
    for (int i = 0; i < count; i++) {
      auto node = dynamic_cast<HDragNode *>(_drag_node->childAtIndex(i));
      if (node) {
        auto index = node->getPointIndex();
        if (index >= 0 && index < pts.size()) {
          auto point = board->LCS2WCS(pts[i]);
          node->moveTo(point);
        }
      }
    }
  }
}

void HNodeBase::updateMat(HBoard *, const QImage &, const QPointF &) {}

void HNodeBase::updateRoi(HBoard *, const QRectF &) {}

int HNodeBase::save(QJsonObject &d) {
  if (!_text.isEmpty()) {
    d.insert("text", _text);
    QJsonObject rect;
    rect.insert("x", _text_rect.x());
    rect.insert("y", _text_rect.y());
    rect.insert("width", _text_rect.width());
    rect.insert("height", _text_rect.height());
    d.insert("text_rect", rect);
    d.insert("text_pixel_size", _pixel_size);
  }
  d.insert("data", _data);
  d.insert("id", id().toString());
  d.insert("nodeType", nodeType());
  d.insert("param", _param);
  return 0;
}

int HNodeBase::load(const QJsonObject &o) {
  auto text = o.value("text").toString();
  auto size = o.value("text_pixel_size").toInt();
  if (!text.isEmpty()) {
    auto rect = o.value("text_rect").toObject();
    QRectF r =
        QRectF(rect.value("x").toDouble(), rect.value("y").toDouble(),
               rect.value("width").toDouble(), rect.value("height").toDouble());
    setText(text, r, size);
  }
  _id = QUuid::fromString(o.value("id").toString());
  _data = o.value("data").toObject();
  _param = o.value("param").toObject();
  return 0;
}

int HNodeBase::save(const QString &path) {
  QJsonObject o;
  save(o);
  return HJsonCommon::writeJson(path, o);
}

int HNodeBase::load(const QString &path) {
  QJsonObject o;
  if (0 != HJsonCommon::readJsonObject(path, o)) {
    return -1;
  }
  return load(o);
}

QJsonObject HNodeBase::param() { return _param; }

void HNodeBase::setParam(const QJsonObject &p) { _param = p; }

QJsonObject HNodeBase::data() { return _data; }

void HNodeBase::setData(const QJsonObject &d) { _data = d; }

void HNodeBase::insertData(const QString &key, const QJsonValue &value) {
  _data.insert(key, value);
}

void HNodeBase::buildTextNode(HBoard *board) {
  if (_text.isEmpty()) return;
  auto node = get();
  if (!board) return;
  auto tl = getBoundRect().topLeft();
  QRectF rect;
  if (_text_rect.isEmpty()) {
    rect = getBoundRect();
  } else {
    rect = QRectF(tl + _text_rect.topLeft(), _text_rect.size());
  }
  auto func = [=]() {
    auto image_node = board->window()->createImageNode();
    auto image = HSGNodeCommon::createTextImage(
        _text, int(rect.width()), int(rect.height()), Qt::red, _pixel_size);
    auto texture = board->window()->createTextureFromImage(image);
    image_node->setTexture(texture);
    image_node->setRect(rect);
    return image_node;
  };
  if (!_text_node) {
    _text_node = new QSGNode();
    auto image_node = func();
    _text_node->appendChildNode(image_node);
    node->appendChildNode(_text_node);
  } else {
    int count = _text_node->childCount();
    for (int i = 0; i < count; i++) {
      auto n = dynamic_cast<QSGImageNode *>(_text_node->childAtIndex(0));
      if (n) HSGNodeCommon::releaseTextureNode(n);
    }
    _text_node->removeAllChildNodes();
    auto image_node = func();
    _text_node->appendChildNode(image_node);
  }
  board->update();
}

void HNodeBase::flushMayiLine() {
  auto n = get();
  if (n) {
    if (_dash) {
      n->removeChildNode(_dash);
      delete _dash;
      _dash = nullptr;
    }
  }
}

void HNodeBase::updateDrawDash(QSGNode *parent, QSGGeometryNode *&dash,
                               QList<QPointF> &dash_list, int line_width) {
  if (!parent) return;
  if (dash) {
    auto geo = dash->geometry();
    if (geo) {
      int count = geo->vertexCount();
      if (count == dash_list.count()) {
        auto list = dash_list;
        list.pop_front();
        auto geo = HSGNodeCommon::buildGeometry(list, GL_LINES);
        geo->setLineWidth(line_width);
        dash->setGeometry(geo);
      } else {
        auto geo = HSGNodeCommon::buildGeometry(dash_list, GL_LINES);
        geo->setLineWidth(line_width);
        dash->setGeometry(geo);
      }
    }
  } else {
    QList<QPointF> list;
    switch (nodeType()) {
      case IMAGE:
        list = HCommon::BuildRectLinesList(getBoundRect());
        break;
      default:
        list = HCommon::BuildPolyLinesList(getPointList());
        break;
    }
    if (list.empty()) return;
    if (list.size() > STEP) {
      double step = list.size() / STEP;
      dash_list.clear();
      for (double i = 0; i < list.size(); i += step) {
        dash_list.push_back(list[int(i)]);
      }
    } else {
      dash_list = list;
    }
    dash = HSGNodeCommon::buildGeometryNode(dash_list, Qt::blue, GL_LINES,
                                            line_width);
    parent->appendChildNode(dash);
  }
}
