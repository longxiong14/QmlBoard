#include "hnodebase.h"

#include <QDebug>
#include <QSGGeometryNode>
#include <QSGNode>

#include "../Common/hcommons.h"
#include "../Common/hsgnodecommon.h"
#define STEP 5e3
#define DEBUG qDebug() << __FUNCTION__ << " " << __LINE__ << " "
HNodeBase::HNodeBase() : _select(false), _visible(true), _dash(nullptr) {
  _id = QUuid::createUuid();
}

HNodeBase::~HNodeBase() {}

QList<QPointF> HNodeBase::getPointList() { return {}; }

QUuid HNodeBase::id() { return _id; }

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

void HNodeBase::move(const QPointF& p) {
  // move _dash
  for (int i = 0; i < _dash_list.size(); i++) {
    _dash_list[i] += p;
  }
}

void HNodeBase::setVisible(bool flag) { _visible = flag; }

bool HNodeBase::visible() { return _visible; }

void HNodeBase::timeOut() {
  auto node = get();
  int line_width = 2;
  if (node) {
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
        case POLY:
          list = HCommon::BuildPolyLinesList(getPointList());
          break;
        case RECTANGLE:
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

QJsonObject HNodeBase::param() { return _param; }

void HNodeBase::setParam(const QJsonObject& p) { _param = p; }
