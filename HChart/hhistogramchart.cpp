#include "hhistogramchart.h"

#include <QColor>
#include <QCursor>
#include <QDebug>
#include <QQuickWindow>
#include <QSGGeometryNode>
#include <QSGImageNode>
#include <QSGNode>
#define DEBUG qDebug() << __FUNCTION__ << __LINE__
#include "../Common/hsgnodecommon.h"
HHistogramChart::HHistogramChart(QQuickItem *parent)
    : QQuickItem(parent),
      _array_changed(true),
      _chart_node(nullptr),
      _left_node(nullptr),
      _right_node(nullptr),
      _bottom_rule(nullptr),
      _left(0),
      _right(255),
      _color("red"),
      _font_size(10) {
  setFlag(QQuickItem::ItemHasContents, true);
  setAcceptHoverEvents(true);
  setClip(true);
  setFocus(true);
  setAcceptedMouseButtons(Qt::MouseButton::LeftButton);
}

QSGNode *HHistogramChart::updatePaintNode(QSGNode *node,
                                          QQuickItem::UpdatePaintNodeData *) {
  if (!node) {
    node = new QSGNode();
  }
  if (node) {
    updateChart(node);
    updateLeft(node);
    updateRight(node);
    _array_changed = false;
  }
  return node;
}

void HHistogramChart::mousePressEvent(QMouseEvent *event) {
  if (event) {
    auto pos = event->pos();
    double step = width() / _array.size();
    auto l = _left * step;
    auto r = _right * step;
    if (std::fabs(l - pos.x()) < 2) {
      _lor = LOR::LEFT;
    } else if (std::fabs(r - pos.x()) < 2) {
      _lor = LOR::RIGHT;
    } else {
      _lor = LOR::NONE;
    }
  }
}

void HHistogramChart::mouseMoveEvent(QMouseEvent *event) {
  if (event) {
    auto pos = event->pos();
    auto step = width() / _array.size();
    switch (_lor) {
      case LOR::LEFT:
        setLeft(int(pos.x() / step));
        update();
        break;
      case LOR::RIGHT:
        setRight(int(pos.x() / step));
        update();
        break;
      case LOR::NONE:
        break;
    }
  }
}

void HHistogramChart::mouseReleaseEvent(QMouseEvent *) {
  //
  switch (_lor) {
    case LOR::LEFT:
      leftChanged();
      break;
    case LOR::RIGHT:
      rightChanged();
      break;
    case LOR::NONE:
      break;
  }
}

void HHistogramChart::hoverMoveEvent(QHoverEvent *event) {
  if (event) {
    auto pos = event->pos();
    double step = width() / _array.size();
    auto l = _left * step;
    auto r = _right * step;
    if (std::fabs(l - pos.x()) < 2 || std::fabs(r - pos.x()) < 2) {
      setCursor(QCursor(Qt::CursorShape::SplitHCursor));
    } else {
      setCursor(QCursor(Qt::CursorShape::ArrowCursor));
    }
  }
}

QJsonArray HHistogramChart::array() { return _array; }

void HHistogramChart::setArray(const QJsonArray &array) {
  _array = array;
  arrayChanged();
}

int HHistogramChart::left() { return _left; }

void HHistogramChart::setLeft(int l) {
  if (_left != l && l < _right && l >= 0) {
    _left = l;
  }
}

int HHistogramChart::right() { return _right; }

void HHistogramChart::setRight(int r) {
  if (_right != r && _left < r && r >= 0 && r <= 255) {
    _right = r;
  }
}

QString HHistogramChart::color() { return _color; }

void HHistogramChart::setColor(const QString &c) {
  if (c != _color) {
    _color = c;
    colorChanged();
  }
}

int HHistogramChart::fontSize() { return _font_size; }
void HHistogramChart::setFontSize(int size) {
  if (size != _font_size) {
    _font_size = size;
    fontSizeChanged();
  }
}

void HHistogramChart::updateChart(QSGNode *node) {
  if (node) {
    if (!_chart_node) {
      _chart_node = new QSGGeometryNode();
    }
    if (!_bottom_rule) {
      _bottom_rule = new QSGNode();
    }
    if (_bottom_rule) {
      for (int i = 0; i < _bottom_rule->childCount(); i++) {
        QSGImageNode *n =
            dynamic_cast<QSGImageNode *>(_bottom_rule->childAtIndex(0));
        if (n) HSGNodeCommon::releaseTextureNode(n);
      }
      _bottom_rule->removeAllChildNodes();
    }
    double bottom = height() - 20;
    double step = width() / _array.size();
    double max = INT_MIN;
    for (int i = 0; i < _array.size(); i++) {
      max = std::max(max, _array[i].toDouble());
    }
    QList<QPointF> list;
    for (int i = 0; i < _array.size(); i++) {
      list.push_back(QPointF(i * step, bottom));
      list.push_back(
          QPointF(i * step, bottom * (1 - _array[i].toDouble() / max)));
      if (0 == i % 50) {
        auto image = HSGNodeCommon::createTextImage(
            QString::number(i), _font_size * 3, _font_size * 2);
        auto image_node = window()->createImageNode();
        auto texture = window()->createTextureFromImage(image);
        image_node->setTexture(texture);
        image_node->setRect(i * step, bottom, _font_size * 3, _font_size * 2);
        _bottom_rule->appendChildNode(image_node);
      }
    }
    auto geo = HSGNodeCommon::buildGeometry(list, GL_LINES);
    _chart_node->setGeometry(geo);
    _chart_node->setFlag(QSGNode::OwnsGeometry);
    auto c = QColor();
    c.setNamedColor(_color);
    QSGFlatColorMaterial *material = HSGNodeCommon::buildColor(c);
    _chart_node->setMaterial(material);
    _chart_node->setFlag(QSGNode::OwnsMaterial);
    if (!_chart_node->parent()) node->appendChildNode(_chart_node);
    if (!_bottom_rule->parent()) node->appendChildNode(_bottom_rule);
  }
}

void HHistogramChart::updateLeft(QSGNode *node) {
  if (node) {
    updateNode(_left_node, _left);
    if (!_left_node->parent()) {
      node->appendChildNode(_left_node);
    }
  }
}

void HHistogramChart::updateRight(QSGNode *node) {
  if (node) {
    updateNode(_right_node, _right);
    if (!_right_node->parent()) {
      node->appendChildNode(_right_node);
    }
  }
}

void HHistogramChart::updateNode(QSGGeometryNode *&node, int num) {
  double step = width() / _array.size();
  if (!node) {
    node = new QSGGeometryNode();
  }
  if (node) {
    for (int i = 0; i < node->childCount(); i++) {
      QSGImageNode *n = dynamic_cast<QSGImageNode *>(node->childAtIndex(0));
      if (n) HSGNodeCommon::releaseTextureNode(n);
    }
    node->removeAllChildNodes();
  }

  QList<QPointF> list{{step * num, 0}, {step * num, height()}};
  auto geo = HSGNodeCommon::buildGeometry(list, GL_LINES);
  geo->setLineWidth(2);
  node->setGeometry(geo);
  node->setFlag(QSGNode::OwnsGeometry);

  QColor c;
  c.setNamedColor(color());
  auto red = c.red() ^ 0xff;
  auto green = c.green() ^ 0xff;
  auto blue = c.blue() ^ 0xff;
  c = QColor(red, green, blue);
  {
    auto image = HSGNodeCommon::createTextImage(QString::number(num),
                                                _font_size * 3, _font_size * 2);
    auto image_node = window()->createImageNode();
    auto texture = window()->createTextureFromImage(image);
    image_node->setTexture(texture);
    image_node->setRect(step * num, 0, _font_size * 3, _font_size * 2);
    node->appendChildNode(image_node);
  }

  QSGFlatColorMaterial *material = HSGNodeCommon::buildColor(c);
  node->setMaterial(material);
  node->setFlag(QSGNode::OwnsMaterial);
}
