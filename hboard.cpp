#include "hboard.h"

#include <QDebug>
#include <QJsonArray>
#include <QQmlEngine>
#include <QQuickWindow>
#include <QSGImageNode>
#include <QSGOpaqueTextureMaterial>
#include <QSGSimpleRectNode>
#include <QSGSimpleTextureNode>
#include <QSGTexture>

#include "Common/hcommons.h"
#include "Common/hjsoncommon.h"
#include "Handles/hhandlearrow.h"
#include "Handles/hhandlebase.h"
#include "Handles/hhandleflyweight.h"
#include "Handles/hhandlemove.h"
#include "Nodes/hcvmatnode.h"
#include "Nodes/hfillnode.h"
#include "Nodes/hnodebase.h"
#include "hboardmanager.h"
#define DEBUG qDebug() << __FUNCTION__ << " " << __LINE__ << " "

HBoard::HBoard(QQuickItem *parent)
    : QQuickItem(parent),
      _trans_node(nullptr),
      _handle(new HHandleArrow()),
      _name("") {
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
      bool flag = false;
      for (auto n : _nodes) {
        if (n->isSelect()) {
          flag = true;
          pushTask([=]() { n->timeOut(); });
        }
      }
      if (flag) update();
    }
  });
}

HBoard::~HBoard() { _nodes.clear(); }

void HBoard::home() {
  pushTask([=]() {
    bool flag = false;
    QRectF rect(INT_MAX, INT_MAX, INT_MIN, INT_MIN);
    DEBUG << _nodes.size();
    for (const auto &key : _nodes.keys()) {
      auto node = _nodes.value(key);
      if (node) {
        flag = true;
        auto r = node->getBoundRect();
        rect.setX(std::min(r.x(), rect.x()));
        rect.setY(std::min(r.y(), rect.y()));
        rect.setWidth(std::max(r.width(), rect.width()));
        rect.setHeight(std::max(r.height(), rect.height()));
      }
    }
    if (!flag) return;
    auto w = width();
    auto h = height();
    DEBUG << rect;

    auto ws = w / rect.width();
    auto hs = h / rect.height();
    auto scale = std::min(ws, hs);
    QTransform trans;
    auto x = (w - rect.width() * scale) / 2 - rect.x() * scale;
    auto y = (h - rect.height() * scale) / 2 - rect.y() * scale;
    trans.translate(x, y);
    trans.scale(scale, scale);
    if (_trans_node) _trans_node->setMatrix(trans);
  });
  update();
}

void HBoard::checkItems() {
  pushTask([this]() {
    auto sel = selects();
    if (1 == sel.size()) {
      auto node = *sel.begin();
      if (_nodes.contains(node)) setItems(_nodes[node]->param());
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
  for (int i = 0; i < nodes.size(); i++) {
    QJsonObject o = nodes[i].toObject();
    HNodeBase::NODETYPE type =
        static_cast<HNodeBase::NODETYPE>(o.value("nodeType").toInt());
    switch (type) {
      case HNodeBase::NODETYPE::SHAPE: {
        auto node = std::make_shared<HFillNode>();
        if (0 == node->load(o) && !_nodes.contains(node->id())) {
          pushNode(node);
          flag = true;
        } else {
          node->clear();
        }
      } break;
      case HNodeBase::NODETYPE::IMAGE: {
        auto node = std::make_shared<HCVMatNode>();
        if (0 == node->load(o) && !_nodes.contains(node->id())) {
          pushNode(node);
          flag = true;
        }
      } break;
    }
  }
  if (flag) home();
  return 0;
}

void HBoard::pushTransform(const QTransform &trans) {
  pushTask([=]() {
    if (_trans_node) _trans_node->setMatrix(trans);
  });
}

void HBoard::pushNode(std::shared_ptr<HNodeBase> node, bool flag) {
  if (flag) _nodes.insert(node->id(), node);
  pushTask([=]() {
    if (node) {
      _trans_node->appendChildNode(node->build(this));
    }
  });
  update();
}

void HBoard::removeNode(const QUuid &id) {
  pushTask([=]() {
    if (_nodes.contains(id)) {
      auto node = _nodes[id];
      if (node) {
        _trans_node->removeChildNode(node->get());
        _nodes.remove(id);
      }
    }
  });
  update();
}

void HBoard::clearNode() {
  pushTask([=]() {
    int count = _trans_node->childCount();
    for (int i = 0; i < count; i++) {
      _trans_node->removeChildNode(_trans_node->childAtIndex(0));
    }
    _nodes.clear();
  });
  update();
}

void HBoard::removeSelectNode() {
  auto sel = selects();
  for (const auto &id : sel) {
    removeNode(id);
  }
  update();
}

void HBoard::setHandle(HHandleBase *handle) { _handle = handle; }

void HBoard::setSelect(const QUuid &s) {
  clearSelect();
  pushSelect(s);
}

void HBoard::clearSelect() {
  for (auto n : _nodes) {
    if (n->isSelect()) {
      pushTask([=]() { n->changedSelectStatus(); });
    }
  }
  update();
}

void HBoard::pushSelect(const QUuid &s) {
  pushTask([=]() {
    if (_nodes.contains(s)) {
      if (!_nodes[s]->isSelect()) {
        _nodes[s]->changedSelectStatus();
      }
    }
  });
  update();
}

void HBoard::removdSelect(const QUuid &s) {
  pushTask([=]() {
    if (_nodes.contains(s)) {
      if (_nodes[s]->isSelect()) {
        _nodes[s]->changedSelectStatus();
      }
    }
  });
  update();
}

void HBoard::changeSelectStatus(const QUuid &s) {
  if (_nodes.contains(s)) {
    if (_nodes[s]->isSelect()) {
      removdSelect(s);
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
      if (_nodes.contains(node)) {
        auto p = _nodes[node]->param();
        p.insert(key, value);
        _nodes[node]->setParam(p);
      }
    }
  });
  update();
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

QSet<int> HBoard::keys() { return _keys; }

QHash<QUuid, std::shared_ptr<HNodeBase>> HBoard::nodes() { return _nodes; }

QHash<QUuid, std::shared_ptr<HNodeBase>> HBoard::visibleNodes() {
  QHash<QUuid, std::shared_ptr<HNodeBase>> node;
  for (const auto &k : _nodes.keys()) {
    if (_nodes.value(k)->visible()) {
      node.insert(k, _nodes.value(k));
    }
  }
  return node;
}

void HBoard::moveNode(const QUuid &n, QPointF dlt) {
  pushTask([=]() {
    if (_nodes.contains(n)) {
      _nodes[n]->move(dlt);
    }
  });
  update();
}

void HBoard::nodeMoveTo(const QUuid &n, QPointF point) {
  pushTask([=]() {
    if (_nodes.contains(n)) {
      _nodes[n]->moveTo(point);
    }
  });
  update();
}

void HBoard::drawNodePoint(const QUuid &node, const QList<QPointF> points) {
  pushTask([=]() {
    if (_nodes.contains(node))
      _nodes[node]->drawPoints(points);
    else
      DEBUG << "hasn't this node " << node;
  });
  update();
}

bool HBoard::hasNode(const QUuid &node) { return _nodes.contains(node); }

void HBoard::visibleNode(const QUuid &node, bool flag) {
  if (_nodes.contains(node)) {
    auto n = _nodes[node];
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
    update();
  }
}

QString HBoard::name() { return _name; }

void HBoard::setName(const QString &name) {
  if (name != _name) {
    auto old = _name;
    _name = name;
    HBoardManager::getInstance()->changeBoardNmae(old, this);
    nameChanged();
  }
}

QJsonObject HBoard::items() { return _items; }

void HBoard::setItems(const QJsonObject &item) {
  _items = item;
  itemsChanged();
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
  return ((1.0 * w / pt.x()) + (1.0 * h / pt.y())) / 2;
}

QSGTransformNode *HBoard::transformNode() { return _trans_node; }

QTransform HBoard::transform() {
  QTransform trans;
  if (_trans_node) trans = _trans_node->matrix().toTransform();
  return trans;
}

QSGNode *HBoard::updatePaintNode(QSGNode *node,
                                 QQuickItem::UpdatePaintNodeData *) {
  if (!node) {
    node = new QSGNode();
    _trans_node = new QSGTransformNode();
    _trans_node->setMatrix(QMatrix4x4(QTransform()));
    node->appendChildNode(_trans_node);
    node->setFlag(QSGNode::OwnedByParent);
  }

  QMutexLocker lock(&_mutex);
  while (!_tasks.empty()) {
    auto f = _tasks.dequeue();
    f();
  }
  return node;
}

void HBoard::mousePressEvent(QMouseEvent *event) {
  if (_handle) _handle->mousePressEvent(this, event, getHandleParam());
  update();
}

void HBoard::mouseMoveEvent(QMouseEvent *event) {
  if (_handle) _handle->mouseMoveEvent(this, event, getHandleParam());
  auto pos = WCS2LCS(event->pos());
  hoverPoint(int(pos.x()), int(pos.y()));
  update();
}

void HBoard::mouseReleaseEvent(QMouseEvent *event) {
  if (_handle) _handle->mouseReleaseEvent(this, event, getHandleParam());
  update();
}

void HBoard::wheelEvent(QWheelEvent *event) {
  if (_handle) _handle->wheelEvent(this, event);
  update();
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
  _keys.clear();
  setFocus(false);
}

void HBoard::keyPressEvent(QKeyEvent *event) {
  if (event) {
    _keys.insert(event->key());
  }
  switch (event->key()) {
    case Qt::Key_Delete:
      removeSelectNode();
      break;
  }
}

void HBoard::keyReleaseEvent(QKeyEvent *event) {
  if (event) {
    _keys.remove(event->key());
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
