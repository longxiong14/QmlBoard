#include "hboard.h"

#include <QDebug>
#include <QJsonArray>
#include <QPainter>
#include <QQmlEngine>
#include <QQuickWindow>
#include <QSGImageNode>
#include <QSGOpaqueTextureMaterial>
#include <QSGSimpleRectNode>
#include <QSGTexture>

#include "Common/hcommons.h"
#include "Common/hjsoncommon.h"
#include "Common/hsgnodecommon.h"
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
    : QQuickItem(parent), _trans_node(nullptr), _handle(new HHandleArrow()),
      _name(""), _rule(nullptr) {
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
      pushTask([=]() {
        for (auto node : _nodes) {
          if (node->isSelect() && node->get()) {
            node->timeOut();
          }
        }
      });
      update();
    }
    //    if (_trans_node) {
    //      bool flag = false;
    //      for (auto n : _nodes) {
    //        if (n->isSelect()) {
    //          flag = true;
    //          pushTask([=]() { n->timeOut(); });
    //        }
    //      }
    //      if (flag)
    //        update();
    //    }
  });
}

HBoard::~HBoard() {
  DEBUG << _nodes.size();
  for (const auto &node : nodes()) {
    node->setDestory(false);
  }
  DEBUG << "clear nodes";
  auto instance = HBoardManager::getInstance();
  if (instance) {
    instance->removeBoard(name());
  }
  DEBUG << "end distruct";
}

void HBoard::home() {
  pushTask([=]() {
    bool flag = false;
    QRectF rect(INT_MAX, INT_MAX, INT_MIN, INT_MIN);
    DEBUG << _nodes.size();
    for (const auto &key : _nodes.keys()) {
      auto node = _nodes.value(key);
      if (node && node->enableHome()) {
        flag = true;
        auto r = node->getBoundRect();
        rect.setX(std::min(r.x(), rect.x()));
        rect.setY(std::min(r.y(), rect.y()));
        rect.setWidth(std::max(r.width(), rect.width()));
        rect.setHeight(std::max(r.height(), rect.height()));
      }
    }
    if (!flag)
      return;
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
    if (_trans_node)
      _trans_node->setMatrix(trans);
  });
  update();
}

void HBoard::checkItems() {
  pushTask([this]() {
    auto sel = selects();
    if (1 == sel.size()) {
      auto node = *sel.begin();
      if (_nodes.contains(node))
        setItems(_nodes[node]->param());
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
  if (flag)
    home();
  return 0;
}

void HBoard::pushTransform(const QTransform &trans) {
  pushTask([=]() {
    if (_trans_node)
      _trans_node->setMatrix(trans);
  });
}

void HBoard::pushNode(std::shared_ptr<HNodeBase> node, bool flag) {
  {
    QMutexLocker lock(&_mutex);
    if (_nodes.contains(node->id()))
      return;
    if (flag)
      _nodes.insert(node->id(), node);
  }
  pushTask([=]() {
    if (node) {
      _trans_node->appendChildNode(node->build(this));
    }
  });
  update();
}

void HBoard::removeNode(const QUuid &id) {
  if (false) {
    std::shared_ptr<HNodeBase> node = nullptr;
    {
      QMutexLocker lock(&_mutex);
      if (_nodes.contains(id)) {
        node = _nodes[id];
        if (node && node->isSelect())
          node->changedSelectStatus();
        _nodes.remove(id);
      }
    }
    pushTask([=]() {
      if (node && node->get()) {
        for (int i = 0; i < _trans_node->childCount(); i++) {
          auto n = _trans_node->childAtIndex(i);
          auto g = node->get();
          if (n == g) {
            _trans_node->removeChildNode(node->get());
            break;
          }
        }
      }
    });
  } else {
    pushTask([=]() {
      if (_nodes.contains(id)) {
        auto node = _nodes[id];
        if (node && node->isSelect())
          node->changedSelectStatus();
        _nodes.remove(id);
        for (int i = 0; i < _trans_node->childCount(); i++) {
          auto n = _trans_node->childAtIndex(i);
          auto g = node->get();
          if (n == g) {
            _trans_node->removeChildNode(node->get());
            break;
          }
        }
      }
    });
  }
  update();
}

void HBoard::clearNode() {
  {
    QMutexLocker lock(&_mutex);
    _nodes.clear();
  }
  pushTask([=]() {
    int count = _trans_node->childCount();
    for (int i = 0; i < count; i++) {
      _trans_node->removeChildNode(_trans_node->childAtIndex(0));
    }
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

void HBoard::setHandle(HHandleBase *handle) {
  if (_handle)
    _handle->boardLeaveOffThisHandle(this);
  _handle = handle;
}

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

int HBoard::updateNodeText(const QUuid &node, const QString &text,
                           const QRectF &rect) {
  if (!_nodes.contains(node)) {
    return -1;
  }
  pushTask([=]() {
    auto n = _nodes.value(node);
    if (n) {
      n->setText(text, rect);
      n->buildTextNode(this);
    }
  });
  update();

  return 0;
}

bool HBoard::updateNodeMat(const QUuid &node, const cv::Mat &mat,
                           const QPointF &start) {
  if (!_nodes.contains(node)) {
    return false;
  }
  auto n = _nodes[node];
  if (!n || HNodeBase::NODETYPE::IMAGE != n->nodeType()) {
    return false;
  }

  pushTask([=]() {
    if (!_nodes.contains(node)) {
      return false;
    }
    auto n = _nodes[node];
    if (!n || HNodeBase::NODETYPE::IMAGE != n->nodeType()) {
      return false;
    }
    auto image_node = dynamic_cast<HCVMatNode *>(n.get());
    if (image_node) {
      image_node->updateMat(this, mat, start);
    }
    return true;
  });
  return 0;
}

bool HBoard::hasNode(const QUuid &node) { return _nodes.contains(node); }

void HBoard::visibleNode(const QUuid &node, bool flag) {
  if (_nodes.contains(node)) {
    auto n = _nodes[node];
    if (flag == n->visible())
      return;
    n->setVisible(flag);
    if (n->visible()) {
      pushTask([=]() {
        if (n)
          _trans_node->appendChildNode(n->get());
      });
    } else {
      pushTask([=]() {
        if (n) {
          if (n->isSelect())
            n->changedSelectStatus();
          _trans_node->removeChildNode(n->get());
        }
      });
    }
    update();
  }
}

std::shared_ptr<HNodeBase> HBoard::getNodeById(const QUuid &id) {
  if (!_nodes.contains(id)) {
    return nullptr;
  }
  return _nodes[id];
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
  if (_trans_node)
    trans = _trans_node->matrix().toTransform();
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
    _rule = new QSGGeometryNode();
    updateRule();
    node->appendChildNode(_rule);
  }
  {
    QMutexLocker lock(&_mutex);
    while (!_tasks.empty()) {
      auto f = _tasks.dequeue();
      f();
    }
  }
  updateRule();
  return node;
}

void HBoard::mousePressEvent(QMouseEvent *event) {
  if (_handle)
    _handle->mousePressEvent(this, event, getHandleParam());
  update();
}

void HBoard::mouseMoveEvent(QMouseEvent *event) {
  if (_handle)
    _handle->mouseMoveEvent(this, event, getHandleParam());
  auto pos = WCS2LCS(event->pos());
  hoverPoint(int(pos.x()), int(pos.y()));
  update();
}

void HBoard::mouseReleaseEvent(QMouseEvent *event) {
  if (_handle)
    _handle->mouseReleaseEvent(this, event, getHandleParam());
  update();
}

void HBoard::wheelEvent(QWheelEvent *event) {
  if (_handle)
    _handle->wheelEvent(this, event);
  update();
}

void HBoard::hoverEnterEvent(QHoverEvent *e) {
  setFocus(true);
  if (_handle)
    _handle->hoverEnterEvent(this, e, getHandleParam());
}

void HBoard::hoverMoveEvent(QHoverEvent *event) {
  auto pos = WCS2LCS(event->pos());
  hoverPoint(int(pos.x()), int(pos.y()));
  if (_handle)
    _handle->hoverMoveEvent(this, event, getHandleParam());
}

void HBoard::hoverLeaveEvent(QHoverEvent *e) {
  if (_handle)
    _handle->hoverLeaveEvent(this, e, getHandleParam());
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

void HBoard::updateRule() {
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
