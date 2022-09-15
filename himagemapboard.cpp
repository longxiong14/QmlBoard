﻿#include "himagemapboard.h"

#include <QDebug>
#include <QPainter>
#include <QQuickWindow>
#include <QSGNode>
#include <thread>

#include "Common/hsgnodecommon.h"
#include "Common/hthreadpool.h"
#include "Nodes/himagemapnode.h"
#include "Nodes/hnodebase.h"
#define DEBUG qDebug() << __FUNCTION__ << __LINE__
HImageMapBoard::HImageMapBoard()
    : _image_node(nullptr), _shape_node(nullptr), _scale(0) {}

void HImageMapBoard::home() {
  HBoard::home();
  updateImageTask();
}

void HImageMapBoard::pushNode(std::shared_ptr<HNodeBase> node, bool flag) {
  if (HNodeBase::NODETYPE::IMAGE == node->nodeType()) {
    pushTask([=]() {
      if (_image_node && node) {
        _image_node->appendChildNode(node->build(this));
        _nodes.push_back(node);
      }
    });
  } else {
    HBoard::pushNode(node, flag);
  }
}

void HImageMapBoard::pushTransform(const QTransform &trans) {
  HBoard::pushTransform(trans);
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

QSGNode *HImageMapBoard::updatePaintNode(
    QSGNode *node, QQuickItem::UpdatePaintNodeData *data) {
  if (!node) {
    node = new QSGNode();
    if (!_image_node) {
      _image_node = new QSGNode();
      node->appendChildNode(_image_node);
    }
    _shape_node = HBoard::updatePaintNode(_shape_node, data);
    if (_shape_node) {
      node->appendChildNode(_shape_node);
    }
  }
  _shape_node = HBoard::updatePaintNode(_shape_node, data);
  return node;
}

bool HImageMapBoard::scaleChanged() {
  bool res = false;
  auto scale = getScale();
  if (std::fabs(_scale - scale) > 1E-10) {
    _scale = scale;
    res = true;
  }
  return res;
}

void HImageMapBoard::updateImages() {
  if (true) {
    auto scale = getScale();
    auto r = getWCSBoundRect();
    QImage screen_image(int(width()), int(height()),
                        QImage::Format::Format_ARGB32);
    QPainter painter(&screen_image);
    painter.setCompositionMode(QPainter::CompositionMode_DestinationIn);
    painter.fillRect(screen_image.rect(), QColor(0, 0, 0, 0));
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
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
          DEBUG << image.size();
          painter.drawImage(lcs_rect, image);
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
          DEBUG << image.size();
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
  pushTask([=]() { clearImageNodes(); });
  pushTask([=]() { updateImages(); });
}