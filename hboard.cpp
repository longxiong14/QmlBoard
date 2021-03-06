#include "hboard.h"

#include <QDebug>
#include <QGraphicsScene>
#include <QQmlEngine>
#include <QQuickWindow>
#include <QSGImageNode>
#include <QSGOpaqueTextureMaterial>
#include <QSGSimpleRectNode>
#include <QSGSimpleTextureNode>
#include <QSGTexture>

#include "Handles/hhandlearrow.h"
#include "Handles/hhandlebase.h"
#include "Handles/hhandlemove.h"
#include "Nodes/himagenode.h"
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
    DEBUG << "timeout";
    if (_trans_node)
      for (auto dash : _dash_nodes) {
        pushTask([=]() {
          _trans_node->removeChildNode(dash->get());
          dash->timeOut();
          _trans_node->appendChildNode(dash->get());
        });
      }
  });
}

void HBoard::visibleNode(bool flag) {
  auto sel = selects();
  for (const auto &k : sel) {
    visibleNode(k, flag);
  }
}

void HBoard::showAll() {
  for (const auto &node : _nodes) {
    if (!node->visible()) {
      node->setVisible(true);
      pushTask([=]() {
        if (node) {
          _trans_node->appendChildNode(node->get());
        }
      });
    }
  }
  update();
}

void HBoard::home() {
  pushTask([=]() {
    QRect rect(INT_MAX, INT_MAX, INT_MIN, INT_MIN);
    for (const auto &key : _nodes.keys()) {
      auto node = _nodes.value(key);
      if (node) {
        auto r = node->getBoundRect();
        rect.setX(std::min(r.x(), rect.x()));
        rect.setY(std::min(r.y(), rect.y()));
        rect.setWidth(std::max(r.width(), rect.width()));
        rect.setHeight(std::max(r.height(), rect.height()));
      }
    }
    auto w = width();
    auto h = height();

    auto ws = w / rect.width();
    auto hs = h / rect.height();
    auto scale = std::min(ws, hs);
    QTransform trans;
    auto x = (w - rect.width() * scale) / 2;
    auto y = (h - rect.height() * scale) / 2;
    trans.translate(x, y);
    trans.scale(scale, scale);
    if (_trans_node) _trans_node->setMatrix(trans);
  });
}

void HBoard::pushTransform(const QTransform &trans) {
  pushTask([=]() {
    if (_trans_node) _trans_node->setMatrix(trans);
  });
}

void HBoard::pushNode(HNodeBase *node, bool flag) {
  pushTask([=]() {
    if (node) {
      _trans_node->appendChildNode(node->build(this));
      if (flag) _nodes.insert(node->id(), node);
    }
  });
}

void HBoard::removeNode(const QUuid &id) {
  if (_nodes.contains(id)) {
    auto node = _nodes[id];
    _nodes.remove(id);
    pushTask([=]() {
      if (node) {
        _trans_node->removeChildNode(node->get());
      }
    });
  }
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

QHash<QUuid, HNodeBase *> HBoard::nodes() { return _nodes; }

QHash<QUuid, HNodeBase *> HBoard::visibleNodes() {
  QHash<QUuid, HNodeBase *> node;
  for (const auto &k : _nodes.keys()) {
    if (_nodes.value(k)->visible()) {
      node.insert(k, _nodes.value(k));
    }
  }
  return node;
}

void HBoard::moveNode(const QUuid &n, QPoint dlt) {
  pushTask([=]() {
    if (_nodes.contains(n)) {
      _nodes[n]->move(dlt);
    }
  });
  update();
}

void HBoard::nodeMoveTo(const QUuid &n, QPoint point) {
  pushTask([=]() {
    if (_nodes.contains(n)) {
      _nodes[n]->moveTo(point);
    }
  });
  update();
}

void HBoard::drawNodePoint(const QUuid &node, const QList<QPoint> points) {
  if (_nodes.contains(node))
    pushTask([=]() { _nodes[node]->drawPoints(points); });
  else
    DEBUG << "hasn't this node " << node;
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

QPoint HBoard::WCS2LCS(const QPoint &point) {
  QPoint pt;
  if (_trans_node) {
    auto trans_form = _trans_node->matrix().toTransform();
    pt = trans_form.inverted().map(point);
  }
  return pt;
}

QPoint HBoard::LCS2WCS(const QPoint &point) {
  QPoint pt;
  if (_trans_node) {
    auto trans_form = _trans_node->matrix().toTransform();
    pt = trans_form.map(point);
  }
  return pt;
}

double HBoard::getScale() {
  auto w = width();
  auto h = height();
  auto pt = WCS2LCS(QPoint(static_cast<int>(w), static_cast<int>(h)));
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
  if (_handle) _handle->mousePressEvent(this, event);
  update();
}

void HBoard::mouseMoveEvent(QMouseEvent *event) {
  if (_handle) _handle->mouseMoveEvent(this, event);
  auto pos = WCS2LCS(event->pos());
  hoverPoint(pos.x(), pos.y());
  update();
}

void HBoard::mouseReleaseEvent(QMouseEvent *event) {
  if (_handle) _handle->mouseReleaseEvent(this, event);
  update();
}

void HBoard::wheelEvent(QWheelEvent *event) {
  if (_handle) _handle->wheelEvent(this, event);
  update();
}

void HBoard::hoverEnterEvent(QHoverEvent *) { setFocus(true); }

void HBoard::hoverMoveEvent(QHoverEvent *event) {
  auto pos = WCS2LCS(event->pos());
  hoverPoint(pos.x(), pos.y());
}

void HBoard::hoverLeaveEvent(QHoverEvent *) { _keys.clear(); }

void HBoard::keyPressEvent(QKeyEvent *event) {
  if (event) {
    _keys.insert(event->key());
  }
}

void HBoard::keyReleaseEvent(QKeyEvent *event) {
  if (event) {
    _keys.remove(event->key());
  }
}

void HBoard::pushTask(const HBoard::task &t) {
  QMutexLocker lock(&_mutex);
  _tasks.push_back(t);
}
