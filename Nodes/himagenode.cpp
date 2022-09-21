#include "himagenode.h"

#include <QDebug>
#include <QImage>
#include <QQuickWindow>
#include <QSGImageNode>
#include <QSGNode>
#include <QSGSimpleTextureNode>

#include "../Common/hcommons.h"
#include "../Common/hjsoncommon.h"
#include "../Common/hsgnodecommon.h"
#include "hboard.h"
#define DEBUG qDebug() << __FUNCTION__ << " " << __LINE__ << " "

#define DEBUG qDebug() << __FUNCTION__ << " " << __LINE__ << " "

HImageNode::HImageNode()
    : HNodeBase(),
      _split_size(1920, 1080),
      _node(nullptr),
      _start_point(0, 0) {}

HImageNode::HImageNode(const QString &path, const QPointF &start_point)
    : HNodeBase(),
      _mat(path),
      _split_size(1920, 1080),
      _node(nullptr),
      _start_point(start_point) {
  if (!_mat.isNull()) {
    _bound_rect =
        QRectF(start_point.x(), start_point.y(), _mat.width(), _mat.height());
  }
}

HImageNode::HImageNode(const QImage &mat, const QPointF &start_point)
    : HNodeBase(),
      _mat(mat),
      _split_size(1920, 1080),
      _node(nullptr),
      _start_point(start_point) {
  if (!_mat.isNull()) {
    _bound_rect =
        QRectF(start_point.x(), start_point.y(), _mat.width(), _mat.height());
  }
}

HImageNode::~HImageNode() {
  bool destory = _flag & NODEFLAG::CANDESTORY;
  if (destory && _node) {
    DEBUG << "release HImageNode " << destory;
    int count = _node->childCount();
    for (int i = 0; i < count; i++) {
      auto n = _node->childAtIndex(0);
      if (n) {
        auto image = dynamic_cast<QSGImageNode *>(n);
        if (image) {
          HSGNodeCommon::releaseTextureNode(image);
        }
      }
    }
    _node->removeAllChildNodes();
    delete _node;
    _node = nullptr;
    _mat = QImage();
  }
  DEBUG << "release HImageNode " << destory;
}

QSGNode *HImageNode::build(HBoard *board) {
  if (!_node && !_mat.isNull()) {
    _node = new QSGNode();
    int col = int(_mat.width() / _split_size.width()) + 1;
    int row = int(_mat.height() / _split_size.height()) + 1;
    QRect src = QRect(0, 0, _mat.width(), _mat.height());
    for (int i = 0; i < col; i++) {
      for (int j = 0; j < row; j++) {
        QRect rect =
            src & QRect(i * _split_size.width(), j * _split_size.height(),
                        _split_size.width(), _split_size.height());
        //        auto image = CVMat2Qimage(_mat(rect));
        auto image = _mat.copy(rect);
        auto r =
            QRectF(rect.x() + _start_point.x(), rect.y() + _start_point.y(),
                   rect.width(), rect.height());
        auto n = BuildQImageNode(image, board, r);
        if (n) {
          _node->appendChildNode(n);
        }
      }
    }
  }
  buildTextNode(board);
  return _node;
}

QSGNode *HImageNode::get() { return _node; }

QRectF HImageNode::getBoundRect() { return _bound_rect; }

QList<QPointF> HImageNode::getPointList() {
  return HCommon::BuildRectList(_bound_rect);
}

void HImageNode::move(const QPointF &point) {
  if (_node) {
    _start_point += point;
    auto rect = QRectF(_start_point, _bound_rect.size());
    _bound_rect = rect;

    auto size = _node->childCount();
    for (int i = 0; i < size; i++) {
      auto node = _node->childAtIndex(i);
      if (node) {
        auto texture = dynamic_cast<QSGImageNode *>(node);
        if (texture) {
          auto rect = texture->rect();
          auto tl = rect.topLeft();
          tl += point;
          QRectF result_rect(tl, rect.size());
          texture->setRect(result_rect);
        }
      }
    }
  }
  HNodeBase::move(point);
}

HNodeBase::NODETYPE HImageNode::nodeType() { return NODETYPE::IMAGE; }

void HImageNode::updateMat(HBoard *board, const QImage &mat, const QPointF &s) {
  if (mat.isNull()) {
    DEBUG << "mat empty";
    return;
  }
  if (mat.format() != _mat.format()) {
    DEBUG << "mat isn't _mat type" << mat.format() << _mat.format();
    return;
  }
  auto start = s.toPoint();
  QRectF roi(start.x(), start.y(), mat.width(), mat.height());
  if (!_mat.isNull()) {
    QRect r(start.x(), start.y(), mat.width(), mat.height());
    r = r & QRect(0, 0, _mat.width(), _mat.height());
    if (r.width() * r.height() <= 0) return;
    if (start.x() == r.x() && start.y() == r.y()) {
      HSGNodeCommon::imageCopyTo(mat, QRect(QPoint(0, 0), r.size()), _mat, r);
    } else if (start.x() == r.x()) {
      HSGNodeCommon::imageCopyTo(
          mat, QRect(0, std::abs(start.y()), r.width(), r.height()), _mat, r);
    } else if (start.y() == r.y()) {
      HSGNodeCommon::imageCopyTo(
          mat, QRect(std::abs(start.x()), 0, r.width(), r.height()), _mat, r);
    } else {
      HSGNodeCommon::imageCopyTo(mat,
                                 QRect(std::abs(start.x()), std::abs(start.y()),
                                       r.width(), r.height()),
                                 _mat, r);
    }
  }
  updateRoi(board, roi);
}

void HImageNode::updateRoi(HBoard *board, const QRectF &roi) {
  if (!board) return;
  auto top_left = getBoundRect().topLeft();
  if (_node) {
    int count = _node->childCount();
    for (int i = 0; i < count; i++) {
      auto n = dynamic_cast<QSGImageNode *>(_node->childAtIndex(i));
      if (n) {
        auto texture_rect = n->rect();
        auto relative_rect =
            QRectF(texture_rect.topLeft() - top_left, texture_rect.size());
        auto relative_rect_int = relative_rect.toRect();
        if (HCommon::RectHasOverlap(roi, relative_rect)) {
          //          DEBUG << texture_rect;
          if (board) {
            QRect t_rect =
                QRect(relative_rect_int.x(), relative_rect_int.y(),
                      relative_rect_int.width(), relative_rect_int.height());
            t_rect &= QRect(0, 0, _mat.width(), _mat.height());
            if (t_rect.width() * t_rect.height() <= 0) continue;
            //            auto t = board->window()->createTextureFromImage(
            //                CVMat2Qimage(_mat(t_rect)));
            auto t = board->window()->createTextureFromImage(_mat.copy(t_rect));
            auto texture = n->texture();
            if (texture) {
              delete texture;
              texture = nullptr;
            }
            n->setTexture(t);
            //            DEBUG << "success update mat";
          }
        }
      }
    }
  }
}

int HImageNode::save(QJsonObject &o) {
  auto rect = getBoundRect();
  auto id = _id.toString();

  {
    QJsonObject r;
    r.insert("x", rect.x());
    r.insert("y", rect.y());
    r.insert("w", rect.width());
    r.insert("h", rect.height());
    o.insert("rect", r);
  }
  {
    QJsonObject size;
    size.insert("w", _split_size.width());
    size.insert("h", _split_size.height());
    o.insert("split_size", size);
  }
  o.insert("nodeType", nodeType());
  o.insert("id", id);
  o.insert("param", _param);
  return HNodeBase::save(o);
}

int HImageNode::load(const QJsonObject &o) {
  return -1;
  //  if (!(o.contains("split_size") && o.value("split_size").isObject() &&
  //        o.contains("param") && o.value("param").isObject() &&
  //        o.contains("rect") && o.value("rect").isObject() &&
  //        o.contains("nodeType") && o.value("nodeType").isDouble() &&
  //        o.value("nodeType").toInt() == nodeType() && o.contains("id") &&
  //        o.value("id").isString())) {
  //    DEBUG << "isn't type";
  //    return -1;
  //  }
  //  {
  //    QJsonObject r = o.value("rect").toObject();
  //    _start_point = QPointF(r.value("x").toDouble(),
  //    r.value("y").toDouble()); _mat = QImage::zeros(r.value("h").toInt(),
  //    r.value("w").toInt(), CV_8UC3); _bound_rect =
  //    QRectF(r.value("x").toInt(), r.value("y").toInt(),
  //                         r.value("w").toInt(), r.value("h").toInt());
  //    DEBUG << _bound_rect;
  //  }

  //  {
  //    auto split_size = o.value("split_size").toObject();
  //    _split_size =
  //        cv::Size(split_size.value("w").toInt(),
  //        split_size.value("h").toInt());
  //  }
  //  _id = o.value("id").toString();
  //  setParam(o.value("param").toObject());
  //  return HNodeBase::load(o);
}

int HImageNode::save(const QString &path) {
  QJsonObject o;
  if (0 != save(o)) {
    return -1;
  }
  return HJsonCommon::writeJson(path, o);
}

int HImageNode::load(const QString &path) {
  QJsonObject o;
  if (0 != HJsonCommon::readJsonObject(path, o)) {
    return -1;
  }
  return load(o);
}

void HImageNode::setSplitSize(const QSize &size) { _split_size = size; }

QSize HImageNode::getSplitSize() { return _split_size; }

void HImageNode::setMat(const QImage &mat) { _mat = mat; }

QImage HImageNode::getMat() { return _mat; }

QSGImageNode *HImageNode::BuildQImageNode(const QImage &image, HBoard *board,
                                          const QRectF &rect) {
  if (image.isNull()) {
    DEBUG << "image is null";
    return nullptr;
  }
  QSGImageNode *node = nullptr;
  if (board) {
    node = board->window()->createImageNode();
    QSGTexture *texture = board->window()->createTextureFromImage(image);
    texture->bind();
    node->setTexture(texture);
    node->setRect(rect);
  }
  return node;
}
