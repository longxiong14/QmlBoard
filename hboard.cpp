﻿#include "hboard.h"

#include <QDebug>
#include <QJsonArray>
#include <QPainter>
#include <QQmlEngine>
#include <QQuickWindow>
#include <QSGImageNode>
#include <QSGOpaqueTextureMaterial>
#include <QSGSimpleRectNode>
#include <QSGTexture>
#include <thread>

#include "Common/hcommons.h"
#include "Common/hjsoncommon.h"
#include "Common/hsgnodecommon.h"
#include "Factory/hkeyfactory.h"
#include "Factory/hnodefactory.h"
#include "Handles/hhandlearrow.h"
#include "Handles/hhandlebase.h"
#include "Handles/hhandleflyweight.h"
#include "Handles/hhandlemove.h"
#include "Nodes/hfillnode.h"
#include "Nodes/himagenode.h"
#include "Nodes/hnodebase.h"
#include "Operators/hcommandbase.h"
#include "hboardmanager.h"
#define DEBUG qDebug() << __FUNCTION__ << " " << __LINE__ << " "

HBoard::HBoard(QQuickItem *parent)
    : QQuickItem(parent),
      _trans_node(nullptr),
      _handle(new HHandleArrow()),
      _name(""),
      _rule(nullptr),
      _drag_nodes(nullptr),
      _rule_flag(true),
      _command_base(std::make_shared<HCommand>()) {
  setKeysControlFactory(std::make_shared<HKeyFactory>());
  setFlag(QQuickItem::ItemHasContents, true);
  setClip(true);
  setAcceptHoverEvents(true);
  setSmooth(true);
  setAntialiasing(true);
  setFocus(true);
  setAcceptedMouseButtons(Qt::MouseButton::LeftButton |
                          Qt::MouseButton::RightButton |
                          Qt::MouseButton::MiddleButton);
  _timer.start(200);
  connect(&_timer, &QTimer::timeout, [this]() {
    if (_trans_node) {
      for (auto node : _nodes) {
        if (node && node->isSelect()) {
          pushTask([=]() {
            for (auto node : _nodes) {
              if (node->isSelect() && node->get()) {
                node->timeOut();
              }
            }
          });
          emit sigUpdate();
          break;
        }
      }
    }
  });

  connect(this, &HBoard::sigUpdate, this, &QQuickItem::update);
}

HBoard::~HBoard() {
  for (const auto &node : nodes()) {
    node->setDestory(false);
  }
  clearNode();
  auto instance = HBoardManager::getInstance();
  if (instance) {
    instance->removeBoard(name());
  }
}

void HBoard::home() {
  pushTask([=]() {
    bool flag = false;
    double tl_x = INT_MAX, tl_y = INT_MAX, br_x = INT_MIN, br_y = INT_MIN;
    for (const auto &node : _nodes) {
      //      auto node = _nodes.value(key);
      if (node && node->enableHome()) {
        flag = true;
        auto r = node->getBoundRect();
        tl_x = std::min(r.topLeft().x(), tl_x);
        tl_y = std::min(r.topLeft().y(), tl_y);
        br_x = std::max(r.bottomRight().x(), br_x);
        br_y = std::max(r.bottomRight().y(), br_y);
      }
    }
    QRectF rect(QPointF(tl_x, tl_y), QPointF(br_x, br_y));
    if (!flag) return;
    auto w = width();
    auto h = height();

    auto ws = w / rect.width();
    auto hs = h / rect.height();
    auto scale = std::min(ws, hs);
    QTransform trans;
    auto x = w / 2 - rect.center().x() * scale;
    auto y = h / 2 - rect.center().y() * scale;

    trans.translate(x, y);
    trans.scale(scale, scale);
    if (_trans_node) {
      _trans_node->setMatrix(trans);
      updateSelectDragNodes();
      faceChanged();
    }
  });
  emit sigUpdate();
}

void HBoard::checkItems() {
  pushTask([this]() {
    auto sel = selects();
    if (1 == sel.size()) {
      auto node = *sel.begin();
      if (containNodes(node)) {
        auto n = getNodeById(node);
        if (n) setItems(n->param());
      }
    } else {
      setItems({});
    }
  });
}

int HBoard::save(const QString &path) {
  QJsonArray out;
  save(out);
  return HJsonCommon::writeJson(path, out);
}

int HBoard::save(QJsonArray &save) {
  for (const auto &node : _nodes) {
    QJsonObject o;
    if (0 == node->save(o)) {
      save.push_back(o);
    }
  }
  return 0;
}

int HBoard::load(const QString &path) {
  QJsonArray array;

  if (0 != HJsonCommon::readJsonArray(path, array)) {
    DEBUG << "isn't array file";
    return -1;
  }
  return load(array);
}

int HBoard::load(const QJsonArray &nodes) {
  bool flag = false;
  HNodeFactory factory;
  for (int i = 0; i < nodes.size(); i++) {
    QJsonObject o = nodes[i].toObject();
    auto node = factory.create(o);
    if (node && 0 == node->load(o) && !containNodes(node->id())) {
      pushNode(node);
      flag = true;
    }
    //    HNodeBase::NODETYPE type =
    //        static_cast<HNodeBase::NODETYPE>(o.value("nodeType").toInt());
    //    switch (type) {
    //      case HNodeBase::NODETYPE::SHAPE: {
    //        auto node = std::make_shared<HFillNode>();
    //        if (0 == node->load(o) && !containNodes(node->id())) {
    //          pushNode(node);
    //          flag = true;
    //        } else {
    //          node->clear();
    //        }
    //      } break;
    //      case HNodeBase::NODETYPE::IMAGE: {
    //        auto node = std::make_shared<HImageNode>();
    //        if (0 == node->load(o) && !containNodes(node->id())) {
    //          pushNode(node);
    //          flag = true;
    //        }
    //      } break;
    //    }
  }
  if (flag) home();
  return 0;
}

void HBoard::pushTransform(const QTransform &trans) {
  pushTask([=]() {
    if (_trans_node) _trans_node->setMatrix(trans);
    updateSelectDragNodes();
    faceChanged();
  });
}

void HBoard::setScale(double scale) {
  if (!transformNode()) return;
  pushTask([=]() {
    auto s = getScale();
    auto pos = getWCSBoundRect().center();
    auto trans = transformNode()->matrix().toTransform();
    trans.translate(pos.x(), pos.y());
    trans.scale(scale / s, scale / s);
    trans.translate(-pos.x(), -pos.y());
    if (_trans_node) _trans_node->setMatrix(trans);
    updateSelectDragNodes();
    faceChanged();
  });
  emit sigUpdate();
}

void HBoard::face(double x, double y) {
  pushTask([=]() {
    auto rect = getWCSBoundRect();
    auto center = rect.center();
    auto last = transform();
    QTransform trans;
    trans.translate(center.x() - x, center.y() - y);
    trans = trans * last;
    if (_trans_node) _trans_node->setMatrix(trans);
    updateSelectDragNodes();
    faceChanged();
  });

  emit sigUpdate();
}

void HBoard::pushNode(std::shared_ptr<HNodeBase> node, bool flag) {
  if (containNodes(node->id())) return;
  if (flag) _nodes.insert(node->id(), node);
  pushTask([=]() {
    if (node) {
      _trans_node->appendChildNode(node->build(this));
    }
  });
  emit sigUpdate();
}

void HBoard::removeNode(const QUuid &id) {
  auto node = getNodeById(id);
  if (!node) {
    //    DEBUG << "hasn't node id" << id;
    return;
  }
  removeNodeToList(id);
  pushTask([=]() { removeNode(node); });
  emit sigUpdate();
}

void HBoard::removeNodes(const QList<QUuid> &nodes) {
  QList<std::shared_ptr<HNodeBase>> ns;
  for (const auto &id : nodes) {
    auto node = getNodeById(id);
    if (!node) {
      DEBUG << "hasn't node id" << id;
      return;
    }
    removeNodeToList(id);
    ns.push_back(node);
  }
  pushTask([=]() {
    for (const auto &n : ns) {
      if (n) removeNode(n);
    }
  });
  emit sigUpdate();
}

void HBoard::clearNode() {
  auto nodes = _nodes;
  _nodes.clear();
  pushTask([=]() {
    for (auto node : nodes) {
      removeNode(node);
    }
  });
  emit sigUpdate();
}

void HBoard::removeSelectNode() {
  auto sel = selects().toList();
  removeNodes(sel);
}

void HBoard::setHandle(std::shared_ptr<HHandleBase> handle) {
  if (_handle) _handle->boardLeaveOffThisHandle(this);
  _handle = handle;
}

void HBoard::setSelect(const QUuid &s) {
  clearSelect();
  pushSelect(s);
}

void HBoard::clearSelect() {
  pushTask([=]() {
    for (auto n : _nodes) {
      if (n->isSelect()) {
        n->changedSelectStatus();
        removeDragNode(n);
      }
    }
  });
  emit sigUpdate();
}

void HBoard::pushSelect(const QUuid &s) {
  pushTask([=]() {
    if (containNodes(s)) {
      auto n = getNodeById(s);
      if (n && !n->isSelect() && n->canSelect()) {
        n->changedSelectStatus();
        auto node = n->buildDragNode(this);
        if (node && _drag_nodes && !node->parent()) {
          _drag_nodes->appendChildNode(node);
        }
      }
    }
  });
  emit sigUpdate();
}

void HBoard::removeSelect(const QUuid &s) {
  pushTask([=]() {
    if (containNodes(s)) {
      auto n = getNodeById(s);
      if (n && n->isSelect()) {
        removeDragNode(n);
        n->changedSelectStatus();
      }
    }
  });
  emit sigUpdate();
}

void HBoard::changeSelectStatus(const QUuid &s) {
  if (containNodes(s)) {
    auto n = getNodeById(s);
    if (n && n->isSelect()) {
      removeSelect(s);
    } else {
      pushSelect(s);
    }
  }
}

void HBoard::changeSelectParam(const QString &key, const QJsonValue &value) {
  pushTask([=]() {
    auto sel = selects();
    if (1 == sel.size()) {
      auto node = *sel.begin();
      if (containNodes(node)) {
        auto n = getNodeById(node);
        if (n) {
          auto p = n->param();
          p.insert(key, value);
          n->setParam(p);
        }
      }
    }
  });
  emit sigUpdate();
}

QSet<QUuid> HBoard::selects() {
  QSet<QUuid> set;
  for (auto n : _nodes) {
    if (n->isSelect()) {
      set.insert(n->id());
    }
  }
  return set;
}

void HBoard::setKeysControlFactory(std::shared_ptr<HKeyFactoryBase> ctrl) {
  _keys_control = ctrl;
}

std::shared_ptr<HKeyFactoryBase> HBoard::getKeysControl() {
  return _keys_control;
}

QSet<int> HBoard::keys() {
  if (_keys_control) return _keys_control->keys();
  return {};
}

void HBoard::setCommand(std::shared_ptr<HCommandBase> command) {
  _command_base = command;
}

std::shared_ptr<HCommandBase> HBoard::getCommand() { return _command_base; }

QHash<QUuid, std::shared_ptr<HNodeBase>> HBoard::nodes() {
  //  QHash<QUuid, std::shared_ptr<HNodeBase>> node;
  //  for (const auto &n : _nodes) {
  //    node.insert(n->id(), n);
  //  }
  return _nodes;
}

QHash<QUuid, std::shared_ptr<HNodeBase>> HBoard::visibleNodes() {
  QHash<QUuid, std::shared_ptr<HNodeBase>> node;
  for (const auto &n : _nodes) {
    if (n && n->visible()) {
      node.insert(n->id(), n);
    }
  }
  return node;
}

void HBoard::moveNode(const QUuid &n, QPointF dlt) {
  pushTask([=]() {
    if (containNodes(n)) {
      auto node = getNodeById(n);
      if (node) {
        node->move(dlt);
        if (node->isSelect()) node->updateDragNodePoint(this);
      }
    }
  });
  emit sigUpdate();
}

void HBoard::nodeMoveTo(const QUuid &n, QPointF point) {
  pushTask([=]() {
    if (containNodes(n)) {
      auto node = getNodeById(n);
      if (node) node->moveTo(point);
    }
  });
  emit sigUpdate();
}

void HBoard::drawNodePoint(const QUuid &node, const QList<QPointF> points) {
  pushTask([=]() {
    if (containNodes(node) && getNodeById(node))
      getNodeById(node)->drawPoints(points);
    else
      DEBUG << "hasn't this node " << node;
  });
  emit sigUpdate();
}

void HBoard::updateNodeIndexPoint(const QUuid &node, int index,
                                  const QPointF &point) {
  pushTask([=]() {
    if (containNodes(node)) {
      auto n = getNodeById(node);
      if (n) {
        n->updateIndexPoint(index, point);
        updateSelectDragNodes();
      }
    }
  });
  emit sigUpdate();
}

int HBoard::updateNodeText(const QUuid &node, const QString &text,
                           const QRectF &rect, int pixel_size) {
  pushTask([=]() {
    if (!containNodes(node)) {
      return;
    }
    auto n = getNodeById(node);
    if (n) {
      n->setText(text, rect, pixel_size);
      n->buildTextNode(this);
    }
  });
  emit sigUpdate();
  return 0;
}

int HBoard::updateNodeDrawMode(const QUuid &node, unsigned long mode) {
  pushTask([=]() {
    if (containNodes(node)) {
      auto n = getNodeById(node);
      if (n) {
        n->updateDrawMode(mode);
      }
    }
  });
  emit sigUpdate();
  return 0;
}

bool HBoard::updateNodeMat(const QUuid &node, const QImage &mat,
                           const QPointF &start) {
  if (!containNodes(node)) {
    return false;
  }
  auto n = getNodeById(node);
  if (!n) {
    return false;
  }

  pushTask([=]() {
    if (!containNodes(node)) {
      return false;
    }
    auto n = getNodeById(node);
    if (n) {
      n->updateMat(this, mat, start);
    }
    return true;
  });
  return true;
}

bool HBoard::hasNode(const QUuid &node) { return containNodes(node); }

void HBoard::visibleNode(const QUuid &node, bool flag) {
  if (containNodes(node)) {
    auto n = getNodeById(node);
    if (flag == n->visible()) return;
    n->setVisible(flag);
    if (n->visible()) {
      pushTask([=]() {
        if (n) _trans_node->appendChildNode(n->get());
      });
    } else {
      pushTask([=]() {
        if (n) {
          if (n->isSelect()) n->changedSelectStatus();
          _trans_node->removeChildNode(n->get());
        }
      });
    }
    emit sigUpdate();
  }
}

std::shared_ptr<HNodeBase> HBoard::getNodeById(const QUuid &id) {
  std::shared_ptr<HNodeBase> node = nullptr;
  if (_nodes.contains(id)) node = _nodes[id];
  //  for (const auto &n : _nodes) {
  //    if (n && n->id() == id) {
  //      node = n;
  //      break;
  //    }
  //  }
  return node;
}

QString HBoard::name() { return _name; }

void HBoard::setName(const QString &name) {
  if (name != _name) {
    auto old = _name;
    _name = name;
    HBoardManager::getInstance()->changeBoardName(old, this);
    nameChanged();
  }
}

QJsonObject HBoard::items() { return _items; }

void HBoard::setItems(const QJsonObject &item) {
  _items = item;
  itemsChanged();
}

bool HBoard::rule() { return _rule_flag; }

void HBoard::setRule(bool f) {
  if (_rule_flag != f) {
    _rule_flag = f;
    ruleChanged();
    emit sigUpdate();
  }
}

QPointF HBoard::WCS2LCS(const QPointF &point) {
  QPointF pt;
  if (_trans_node) {
    auto trans_form = _trans_node->matrix().toTransform();
    pt = trans_form.inverted().map(point);
  }
  return pt;
}

QPointF HBoard::LCS2WCS(const QPointF &point) {
  QPointF pt;
  if (_trans_node) {
    auto trans_form = _trans_node->matrix().toTransform();
    pt = trans_form.map(point);
  }
  return pt;
}

double HBoard::getScale() {
  auto w = width();
  auto h = height();
  auto pt = WCS2LCS(QPointF((w), (h)));
  auto pt2 = WCS2LCS(QPointF(0, 0));
  auto d_w = pt.x() - pt2.x();
  auto d_h = pt.y() - pt2.y();
  return ((1.0 * w / d_w) + (1.0 * h / d_h)) / 2;
}

QRectF HBoard::getWCSBoundRect() {
  auto w = width();
  auto h = height();
  auto pt = WCS2LCS(QPointF((w), (h)));
  auto pt2 = WCS2LCS(QPointF(0, 0));
  return QRectF(pt2, pt);
}

QSGTransformNode *HBoard::transformNode() { return _trans_node; }

QTransform HBoard::transform() {
  QTransform trans;
  if (_trans_node) trans = _trans_node->matrix().toTransform();
  return trans;
}

QSGNode *HBoard::updatePaintNode(QSGNode *node,
                                 QQuickItem::UpdatePaintNodeData *) {
  DEBUG << "paint";
  if (!node) {
    node = new QSGNode();
    _trans_node = new QSGTransformNode();
    _trans_node->setMatrix(QMatrix4x4(QTransform()));
    node->appendChildNode(_trans_node);
    node->setFlag(QSGNode::OwnedByParent);
    updateRule(node);
    _drag_nodes = new QSGNode();
    node->appendChildNode(_drag_nodes);
  } else {
    {
      QMutexLocker lock(&_mutex);
      while (!_tasks.empty()) {
        auto f = _tasks.dequeue();
        f();
      }
    }
    updateRule(node);
  }
  emit updated();
  return node;
}

void HBoard::mousePressEvent(QMouseEvent *event) {
  if (_handle) _handle->mousePressEvent(this, event, getHandleParam());
  emit sigUpdate();
}

void HBoard::mouseMoveEvent(QMouseEvent *event) {
  if (_handle) _handle->mouseMoveEvent(this, event, getHandleParam());
  auto pos = WCS2LCS(event->pos());
  hoverPoint(int(pos.x()), int(pos.y()));
  emit sigUpdate();
}

void HBoard::mouseReleaseEvent(QMouseEvent *event) {
  if (_handle) _handle->mouseReleaseEvent(this, event, getHandleParam());
  emit sigUpdate();
}

void HBoard::wheelEvent(QWheelEvent *event) {
  if (_handle) _handle->wheelEvent(this, event);
  emit sigUpdate();
}

void HBoard::hoverEnterEvent(QHoverEvent *e) {
  setFocus(true);
  if (_handle) _handle->hoverEnterEvent(this, e, getHandleParam());
}

void HBoard::hoverMoveEvent(QHoverEvent *event) {
  auto pos = WCS2LCS(event->pos());
  hoverPoint(int(pos.x()), int(pos.y()));
  if (_handle) _handle->hoverMoveEvent(this, event, getHandleParam());
}

void HBoard::hoverLeaveEvent(QHoverEvent *e) {
  if (_handle) _handle->hoverLeaveEvent(this, e, getHandleParam());
  if (_keys_control) _keys_control->clearKeys();
  setFocus(false);
}

void HBoard::keyPressEvent(QKeyEvent *event) {
  if (_keys_control) _keys_control->keyPressEvent(this, event);
}

void HBoard::keyReleaseEvent(QKeyEvent *event) {
  if (event && _keys_control) {
    _keys_control->keyReleaseEvent(this, event);
  }
}

// void HBoard::sceneGraphInvalidated() { DEBUG << "sceneGraphInvalidated"; }

void HBoard::pushTask(const HBoard::task &t) {
  QMutexLocker lock(&_mutex);
  _tasks.push_back(t);
}

QJsonObject HBoard::getHandleParam() {
  QJsonObject object;
  if (_handle) {
    object = HHandleFlyWeight::getInstance()->getBoardHandleParam(
        name(), _handle->getName());
  }
  return object;
}

void HBoard::updateRule(QSGNode *parent) {
  if (_rule_flag) {
    bool f = false;
    if (!_rule) {
      _rule = new QSGGeometryNode();
      f = true;
    }
    if (_rule) {
      QList<QPointF> list;
      int count = _rule->childCount();
      for (int i = 0; i < count; i++) {
        auto node = _rule->childAtIndex(0);
        _rule->removeChildNode(node);
        auto image = dynamic_cast<QSGImageNode *>(node);
        if (image) {
          HSGNodeCommon::releaseTextureNode(image);
        } else {
          delete node;
          node = nullptr;
        }
      }
      _rule->removeAllChildNodes();
      buildTopRule(list);
      buildLeftRule(list);
      auto geo = HSGNodeCommon::buildGeometry(list, GL_LINES);
      _rule->setGeometry(geo);
      if (!_rule->material()) {
        _rule->setMaterial(HSGNodeCommon::buildColor(Qt::red));
        _rule->setFlag(QSGNode::OwnsMaterial);
      }
      _rule->setFlag(QSGNode::OwnsGeometry);
    }
    if (f) parent->appendChildNode(_rule);
  } else {
    if (_rule) {
      parent->removeChildNode(_rule);
      delete _rule;
      _rule = nullptr;
    }
  }
}

void HBoard::buildTopRule(QList<QPointF> &list) {
  int s = 0;
  int w = int(width());
  for (int i = 0; i < w; i += 10) {
    auto start = i + s;
    auto p1 = QPoint(start, s);
    list.push_back(p1);
    if (0 == i % 100) {
      list.push_back(QPoint(start, s + 10));
      auto p = WCS2LCS(p1);
      QImage text = HSGNodeCommon::createTextImage(
          QString::number(std::round(p.x() * 100) / 100), 50, 20);
      auto texture = window()->createTextureFromImage(text);
      QSGImageNode *texture_node = window()->createImageNode();
      texture_node->setTexture(texture);
      texture_node->setRect(start, 10, 50, 20);
      _rule->appendChildNode(texture_node);
    } else if (0 == i % 50) {
      list.push_back(QPoint(start, s + 5));
    } else {
      list.push_back(QPoint(start, s + 2));
    }
  }
}

void HBoard::buildLeftRule(QList<QPointF> &list) {
  int w = 20, h = 50;
  int hei = int(height());
  for (int i = 30; i < hei; i += 10) {
    auto p1 = QPoint(0, i);
    list.push_back(p1);
    if (0 == i % 100) {
      list.push_back(QPoint(10, i));
      auto p = WCS2LCS(p1);
      QImage text;
      {
        text = QImage(w, h, QImage::Format_ARGB32);
        QPainter painter(&text);
        painter.setCompositionMode(QPainter::CompositionMode_DestinationIn);
        painter.fillRect(0, 0, w, h, QColor(0, 0, 0, 0));
        painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
        auto font = painter.font();
        font.setPointSizeF(10);
        painter.setFont(font);
        painter.setPen(Qt::red);
        painter.translate(QPoint(w, 0));
        painter.rotate(90);
        painter.drawText(QRectF(0, 0, h, w),
                         QString::number(std::round(p.y() * 100) / 100));
      }
      QSGImageNode *texture_node = window()->createImageNode();
      auto texture = window()->createTextureFromImage(text);
      texture_node->setTexture(texture);
      texture_node->setRect(10, i, w, h);
      _rule->appendChildNode(texture_node);
    } else if (0 == i % 50) {
      list.push_back(QPoint(5, i));
    } else {
      list.push_back(QPoint(2, i));
    }
  }
}

bool HBoard::containNodes(std::shared_ptr<HNodeBase> node) {
  if (!node) return false;
  //  for (const auto &n : _nodes) {
  //    if (n && n->id() == node->id()) return true;
  //  }
  return _nodes.contains(node->id());
}

bool HBoard::containNodes(const QUuid &id) {
  //  for (const auto &n : _nodes) {
  //    if (n && n->id() == id) return true;
  //  }
  return _nodes.contains(id);
}

int HBoard::removeNodeToList(const QUuid &id) {
  int res = -1;
  if (_nodes.contains(id)) {
    _nodes.remove(id);
    res = 0;
  }
  //  for (int i = 0; i < _nodes.size(); i++) {
  //    auto n = _nodes[i];
  //    if (n && n->id() == id) {
  //      res = 0;
  //      _nodes.removeAt(i);
  //      break;
  //    }
  //  }
  return res;
}

int HBoard::removeNode(std::shared_ptr<HNodeBase> node) {
  if (!node) return -1;
  if (node->isSelect()) node->changedSelectStatus();
  _trans_node->removeChildNode(node->get());
  removeDragNode(node);
  if (HNodeBase::NODETYPE::IMAGE == node->nodeType()) {
    DEBUG << "remove image node " << node->id();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
  return 0;
}

void HBoard::updateSelectDragNodes() {
  for (const auto &node : _nodes) {
    if (node->isSelect()) {
      node->updateDragNodePoint(this);
    }
  }
}

void HBoard::removeDragNode(std::shared_ptr<HNodeBase> node) {
  if (!node) return;
  auto drag = node->getDragNode();
  if (drag && drag->parent() == _drag_nodes) {
    _drag_nodes->removeChildNode(drag);
    node->destroyDragNode();
  }
}
