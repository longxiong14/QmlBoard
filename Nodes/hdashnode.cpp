#include "hdashnode.h"

#include "hfillnode.h"

HDashNode::HDashNode(HNodeBase *ptr) : HNodeBase(), _node(nullptr) {
  if (ptr) {
    auto rect = ptr->getBoundRect();
  }
}

QSGNode *HDashNode::get() { return _node; }

QSGNode *HDashNode::build(HBoard *board) {
  if (!_node) {
    HFillNode n(_list, Qt::blue, GL_LINES);
    _node = n.build(board);
  }
  return _node;
}
