﻿#include "hnodebase.h"

#include <QDebug>
#include <QQuickWindow>
#include <QSGGeometryNode>
#include <QSGNode>

#include "../Common/hcommons.h"
#include "../Common/hsgnodecommon.h"
#include "../hboard.h"
#define STEP 5e3
#define DEBUG qDebug() << __FUNCTION__ << " " << __LINE__ << " "
HNodeBase::HNodeBase()
    : _select(false),
      _visible(true),
      _dash(nullptr),
      _enable_home(true),
      _text_node(nullptr),
      _destory(true) {
  _id = QUuid::createUuid();
}

HNodeBase::~HNodeBase() {
  if (_destory && _text_node) {
    int count = _text_node->childCount();
    for (int i = 0; i < count; i++) {
      auto n = dynamic_cast<QSGImageNode *>(_text_node->childAtIndex(0));
      if (n) HSGNodeCommon::releaseTextureNode(n);
    }
    delete _text_node;
    _text_node = nullptr;
  }
}

QList<QPointF> HNodeBase::getPointList() { return {}; }

QUuid HNodeBase::id() { return _id; }

void HNodeBase::setId(const QUuid &id) { _id = id; }

void HNodeBase::changedSelectStatus() {
  _select = !_select;
  if (!_select) {
    auto n = get();
    if (n) {
      if (_dash) {
        n->removeChildNode(_dash);
        delete _dash;
        _dash = nullptr;
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

unsigned long HNodeBase::drawingMode() { return 0; }

void HNodeBase::setVisible(bool flag) { _visible = flag; }

bool HNodeBase::visible() { return _visible; }

void HNodeBase::timeOut() {
  auto node = get();
  int line_width = 2;
  if (node && _select) {
    if (_dash) {
      auto geo = _dash->geometry();
      if (geo) {
        int count = geo->vertexCount();
        if (count == _dash_list.count()) {
          auto list = _dash_list;
          list.pop_front();
          auto geo = HSGNodeCommon::buildGeometry(list, GL_LINES);
          geo->setLineWidth(line_width);
          _dash->setGeometry(geo);
        } else {
          auto geo = HSGNodeCommon::buildGeometry(_dash_list, GL_LINES);
          geo->setLineWidth(line_width);
          _dash->setGeometry(geo);
        }
      }
    } else {
      _dash = new QSGGeometryNode();
      QList<QPointF> list;
      switch (nodeType()) {
        case SHAPE:
          list = HCommon::BuildPolyLinesList(getPointList());
          break;
        case IMAGE:
          list = HCommon::BuildRectLinesList(getBoundRect());
          break;
      }
      if (list.size() > STEP) {
        double step = list.size() / STEP;
        _dash_list.clear();
        for (double i = 0; i < list.size(); i += step) {
          _dash_list.push_back(list[int(i)]);
        }
      } else {
        _dash_list = list;
      }

      auto geo = HSGNodeCommon::buildGeometry(_dash_list, GL_LINES);
      geo->setLineWidth(line_width);
      auto color = HSGNodeCommon::buildColor(Qt::blue);
      _dash->setGeometry(geo);
      _dash->setMaterial(color);
      _dash->setFlags(QSGNode::OwnsMaterial | QSGNode::OwnsGeometry);
      node->appendChildNode(_dash);
    }
  }
}

int HNodeBase::setText(const QString &text, HBoard *board,
                       const QRectF &position) {
  auto node = get();
  if (!board) return -1;
  auto tl = getBoundRect().topLeft();
  QRectF rect;
  if (position.isEmpty() || position.isNull() || position.isValid()) {
    rect = getBoundRect();
  } else {
    rect = QRectF(tl + position.topLeft(), position.size());
  }
  auto func = [=]() {
    auto image_node = board->window()->createImageNode();
    auto image = HSGNodeCommon::createTextImage(_text, int(rect.width()),
                                                int(rect.height()));
    auto texture = board->window()->createTextureFromImage(image);
    image_node->setTexture(texture);
    image_node->setRect(rect);
    return image_node;
  };
  _text = text;
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
  return 0;
}

QString HNodeBase::getText() { return _text; }

bool HNodeBase::enableHome() { return _enable_home; }

void HNodeBase::setEnableHome(bool f) { _enable_home = f; }

void HNodeBase::setDestory(bool flag) { _destory = flag; }

QJsonObject HNodeBase::param() { return _param; }

void HNodeBase::setParam(const QJsonObject &p) { _param = p; }
