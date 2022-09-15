#include "himagemapboard.h"

#include <QSGNode>

#include "Nodes/hnodebase.h"
HImageMapBoard::HImageMapBoard() : _image_node(nullptr), _shape_node(nullptr) {}

void HImageMapBoard::pushNode(std::shared_ptr<HNodeBase> node, bool flag) {
  if (HNodeBase::NODETYPE::IMAGE == node->nodeType()) {
    pushTask([=]() {
      if (_image_node && node) {
        _image_node->appendChildNode(node->build(this));
        _nodes.insert(node->id(), node);
      }
    });
  } else {
    HBoard::pushNode(node, flag);
  }
}

void HImageMapBoard::pushTransform(const QTransform &trans) {
  HBoard::pushTransform(trans);
  pushTask([=]() {

  });
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
