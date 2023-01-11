#include "himagemapboard.h"

#include <QDebug>
#include <QPainter>
#include <QQuickWindow>
#include <QSGNode>
#include <thread>

#include "Common/hsgnodecommon.h"
#include "Common/hthreadpool.h"
#include "Nodes/himagemapnode.h"
#include "Nodes/hnodebase.h"
#define DEBUG \
  if (_debug) qDebug() << __FUNCTION__ << __LINE__
HImageMapBoard::HImageMapBoard(QQuickItem *parent)
    : HBoard(parent), _image_node(nullptr), _debug(false) {}

void HImageMapBoard::home() {
  HBoard::home();
  updateImageTask();
}

void HImageMapBoard::pushNode(std::shared_ptr<HNodeBase> node, bool flag) {
  HBoard::pushNode(node, flag);
  if (node && HNodeBase::NODETYPE::MAPINAGE == node->nodeType()) {
    updateImageTask();
  }
}

void HImageMapBoard::removeNode(const QUuid &id) {
  auto node = getNodeById(id);
  HBoard::removeNode(id);
  if (node) DEBUG << node->id();
  if (node && HNodeBase::NODETYPE::MAPINAGE == node->nodeType()) {
    updateImageTask();
  }
}

void HImageMapBoard::pushTransform(const QTransform &trans) {
  HBoard::pushTransform(trans);
  updateImageTask();
}

void HImageMapBoard::face(double x, double y) {
  HBoard::face(x, y);
  updateImageTask();
}

void HImageMapBoard::setScale(double scale) {
  HBoard::setScale(scale);
  updateImageTask();
}

void HImageMapBoard::moveNode(const QUuid &n, QPointF dlt) {
  HBoard::moveNode(n, dlt);
  auto node = getNodeById(n);
  if (node && HNodeBase::NODETYPE::MAPINAGE == node->nodeType()) {
    updateImageTask();
  }
}

void HImageMapBoard::nodeMoveTo(const QUuid &n, QPointF point) {
  HBoard::nodeMoveTo(n, point);
  auto node = getNodeById(n);
  if (node && HNodeBase::NODETYPE::MAPINAGE == node->nodeType()) {
    updateImageTask();
  }
}

QSGNode *HImageMapBoard::updatePaintNode(QSGNode *node,
                                         QQuickItem::UpdatePaintNodeData *) {
  if (!node) {
    node = new QSGNode();
    if (!_image_node) {
      _image_node = new QSGNode();
      node->appendChildNode(_image_node);
    }
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
  return node;
}

bool HImageMapBoard::updateNodeMat(const QUuid &node, const QImage &mat,
                                   const QPointF &start) {
  bool flag = HBoard::updateNodeMat(node, mat, start);
  auto n = getNodeById(node);
  if (HNodeBase::NODETYPE::MAPINAGE == n->nodeType()) updateImageTask();
  return flag;
}

bool HImageMapBoard::debug() { return _debug; }

void HImageMapBoard::setDebug(bool f) {
  if (_debug != f) {
    _debug = f;
    debugChanged();
  }
}

void HImageMapBoard::updateImages() {
  if (true) {
    double scale = getScale();
    auto r = getWCSBoundRect();
    QImage screen_image(int(width()), int(height()),
                        QImage::Format::Format_ARGB32);
    QPainter painter(&screen_image);
    painter.setCompositionMode(QPainter::CompositionMode_DestinationIn);
    painter.fillRect(screen_image.rect(), QColor(0, 0, 0, 0));
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    bool flag = false;
    auto nodes = getZOrderNodes();
    for (auto node : nodes) {
      if (node && HNodeBase::NODETYPE::MAPINAGE == node->nodeType()) {
        auto map_image_node = dynamic_cast<HImageMapNodeDelegate *>(node.get());
        if (map_image_node) {
          auto rect = node->getBoundRect();
          auto dst = r & rect;
          auto size = dst.size();
          auto tl = dst.topLeft() - rect.topLeft();
          auto clone_dst = QRectF(tl, size);
          auto lcs_rect =
              QRectF(LCS2WCS(dst.topLeft()), LCS2WCS(dst.bottomRight()));
          auto image = map_image_node->getImage(clone_dst, scale);
          painter.drawImage(lcs_rect.topLeft(), image, image.rect(),
                            Qt::ThresholdDither);

          flag = true;
        }
      }
    }

    if (flag) {
      QSGImageNode *image_node = window()->createImageNode();
      auto texture = window()->createTextureFromImage(screen_image);

      image_node->setTexture(texture);
      image_node->setRect(0, 0, width(), height());
      if (_image_node) {
        _image_node->appendChildNode(image_node);
      }
    }
  } else {
    auto scale = getScale();
    auto r = getWCSBoundRect();
    bool flag = false;
    for (auto node : _nodes) {
      if (node && HNodeBase::NODETYPE::MAPINAGE == node->nodeType()) {
        auto map_image_node = dynamic_cast<HImageMapNode *>(node.get());
        if (map_image_node) {
          auto rect = node->getBoundRect();
          auto dst = r & rect;
          auto size = dst.size();
          auto tl = dst.topLeft() - rect.topLeft();
          auto clone_dst = QRectF(tl, size);

          auto image = map_image_node->getImage();
          image = image.copy(clone_dst.toRect());
          image = image.scaled(int(image.width() * scale),
                               int(image.height() * scale));
          auto lcs_rect =
              QRectF(LCS2WCS(dst.topLeft()), LCS2WCS(dst.bottomRight()));
          if (image.isNull()) {
            DEBUG << "image is null";
            continue;
          }
          auto image_node = window()->createImageNode();
          auto texture = window()->createTextureFromImage(image);
          image_node->setTexture(texture);
          image_node->setRect(lcs_rect);
          if (_image_node) {
            _image_node->appendChildNode(image_node);
          }
          flag = true;
        }
      }
    }
  }
}

void HImageMapBoard::clearImageNodes() {
  if (_image_node) {
    for (int i = 0; i < _image_node->childCount(); i++) {
      auto image = dynamic_cast<QSGImageNode *>(_image_node->childAtIndex(0));
      if (image) {
        auto texture = image->texture();
        if (texture) {
          delete texture;
          texture = nullptr;
        }
        _image_node->removeChildNode(image);
        delete image;
        image = nullptr;
      }
    }
  }
}

void HImageMapBoard::updateImageTask() {
  pushTask([=]() {
    clearImageNodes();
    updateImages();
    DEBUG;
  });
}

QList<std::shared_ptr<HNodeBase> > HImageMapBoard::getZOrderNodes() {
  QList<std::shared_ptr<HNodeBase> > out;
  for (const auto &node : _nodes) {
    if (node->nodeType() == HNodeBase::NODETYPE::MAPINAGE) {
      out.push_back(node);
    }
  }
  std::sort(out.begin(), out.end(),
            [](const std::shared_ptr<HNodeBase> &f,
               const std::shared_ptr<HNodeBase> &s) {
              return f->getZOrder() < s->getZOrder();
            });
  return out;
}
