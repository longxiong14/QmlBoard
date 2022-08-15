#include "hcvmatnode.h"

#include <QDebug>
#include <QImage>
#include <QQuickWindow>
#include <QSGNode>
#include <QSGSimpleTextureNode>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#include "../Common/hcommons.h"
#include "../Common/hjsoncommon.h"
#include "../hboard.h"

#define DEBUG qDebug() << __FUNCTION__ << " " << __LINE__ << " "

HCVMatNode::HCVMatNode()
    : HNodeBase(),
      _split_size(1920, 1080),
      _node(nullptr),
      _start_point(0, 0) {}

HCVMatNode::HCVMatNode(const QString &path, const QPointF &start_point)
    : HNodeBase(),
      _split_size(1920, 1080),
      _node(nullptr),
      _start_point(start_point) {
  _mat = cv::imread(path.toLocal8Bit().toStdString());
  if (!_mat.empty()) {
    _bound_rect =
        QRectF(start_point.x(), start_point.y(), _mat.cols, _mat.rows);
  }
}

HCVMatNode::HCVMatNode(const cv::Mat &mat, const QPointF &start_point)
    : HNodeBase(),
      _mat(mat),
      _split_size(1920, 1080),
      _node(nullptr),
      _start_point(start_point) {
  if (!_mat.empty()) {
    _bound_rect =
        QRectF(start_point.x(), start_point.y(), _mat.cols, _mat.rows);
  }
}

QSGNode *HCVMatNode::build(HBoard *board) {
  DEBUG << !_node << " " << !_mat.empty();
  if (!_node && !_mat.empty()) {
    _node = new QSGNode();
    int col = int(_mat.cols / _split_size.width) + 1;
    int row = int(_mat.rows / _split_size.height) + 1;
    cv::Rect src = cv::Rect(0, 0, _mat.cols, _mat.rows);
    for (int i = 0; i < col; i++) {
      for (int j = 0; j < row; j++) {
        cv::Rect rect =
            src & cv::Rect(i * _split_size.width, j * _split_size.height,
                           _split_size.width, _split_size.height);
        auto image = CVMat2Qimage(_mat(rect));
        auto r = QRectF(rect.x + _start_point.x(), rect.y + _start_point.y(),
                        rect.width, rect.height);
        auto n = BuildQImageNode(image, board, r);
        if (n) {
          _node->appendChildNode(n);
        }
      }
    }
  }

  return _node;
}

QSGNode *HCVMatNode::get() { return _node; }

QRectF HCVMatNode::getBoundRect() { return _bound_rect; }

QList<QPointF> HCVMatNode::getPointList() {
  return HCommon::BuildRectList(_bound_rect);
}

void HCVMatNode::move(const QPointF &point) {
  if (_node) {
    _start_point += point;
    auto rect = QRectF(_start_point, _bound_rect.size());
    _bound_rect = rect;

    auto size = _node->childCount();
    for (int i = 0; i < size; i++) {
      auto node = _node->childAtIndex(i);
      if (node) {
        auto texture = dynamic_cast<QSGSimpleTextureNode *>(node);
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

HNodeBase::NODETYPE HCVMatNode::nodeType() { return NODETYPE::IMAGE; }

void HCVMatNode::updateMat(HBoard *board, const cv::Mat &mat,
                           const QPointF &s) {
  if (mat.empty()) {
    DEBUG << "mat empty";
    return;
  }
  if (mat.type() != _mat.type()) {
    DEBUG << "mat isn't _mat type";
    return;
  }
  auto start = s.toPoint();
  QRectF roi(start.x(), start.y(), mat.cols, mat.rows);
  if (!_mat.empty()) {
    cv::Rect r(start.x(), start.y(), mat.cols, mat.rows);
    r = r & cv::Rect(0, 0, _mat.cols, _mat.rows);
    if (r.area() <= 0) return;
    if (start.x() == r.x && start.y() == r.y) {
      mat(cv::Rect(cv::Point(0, 0), r.size())).copyTo(_mat(r));
    } else if (start.x() == r.x) {
      mat(cv::Rect(0, std::abs(start.y()), r.width, r.height)).copyTo(_mat(r));
    } else if (start.y() == r.y) {
      mat(cv::Rect(std::abs(start.x()), 0, r.width, r.height)).copyTo(_mat(r));
    } else {
      mat(cv::Rect(std::abs(start.x()), std::abs(start.y()), r.width, r.height))
          .copyTo(_mat(r));
    }
  }
  updateRoi(board, roi);
}

void HCVMatNode::updateRoi(HBoard *board, const QRectF &roi) {
  if (!board) return;
  auto top_left = getBoundRect().topLeft();
  if (_node) {
    int count = _node->childCount();
    for (int i = 0; i < count; i++) {
      auto n = dynamic_cast<QSGSimpleTextureNode *>(_node->childAtIndex(i));
      if (n) {
        auto texture_rect = n->rect();
        auto relative_rect =
            QRectF(texture_rect.topLeft() - top_left, texture_rect.size());
        auto relative_rect_int = relative_rect.toRect();
        if (HCommon::RectHasOverlap(roi, relative_rect)) {
          //          DEBUG << texture_rect;
          if (board) {
            cv::Rect t_rect =
                cv::Rect(relative_rect_int.x(), relative_rect_int.y(),
                         relative_rect_int.width(), relative_rect_int.height());
            t_rect &= cv::Rect(0, 0, _mat.cols, _mat.rows);
            if (t_rect.area() <= 0) continue;
            auto t = board->window()->createTextureFromImage(
                CVMat2Qimage(_mat(t_rect)));
            n->setTexture(t);
            DEBUG << "success update mat";
          }
        }
      }
    }
  }
}

int HCVMatNode::save(QJsonObject &o) {
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
    size.insert("w", _split_size.width);
    size.insert("h", _split_size.height);
    o.insert("split_size", size);
  }
  o.insert("nodeType", nodeType());
  o.insert("id", id);
  o.insert("param", _param);
  return 0;
}

int HCVMatNode::load(const QJsonObject &o) {
  if (!(o.contains("split_size") && o.value("split_size").isObject() &&
        o.contains("param") && o.value("param").isObject() &&
        o.contains("rect") && o.value("rect").isObject() &&
        o.contains("nodeType") && o.value("nodeType").isDouble() &&
        o.value("nodeType").toInt() == nodeType() && o.contains("id") &&
        o.value("id").isString())) {
    DEBUG << "isn't type";
    return -1;
  }
  {
    QJsonObject r = o.value("rect").toObject();
    _start_point = QPointF(r.value("x").toDouble(), r.value("y").toDouble());
    _mat = cv::Mat::zeros(r.value("h").toInt(), r.value("w").toInt(), CV_8UC3);
    _bound_rect = QRectF(r.value("x").toInt(), r.value("y").toInt(),
                         r.value("w").toInt(), r.value("h").toInt());
    DEBUG << _bound_rect;
  }

  {
    auto split_size = o.value("split_size").toObject();
    _split_size =
        cv::Size(split_size.value("w").toInt(), split_size.value("h").toInt());
  }
  _id = o.value("id").toString();
  setParam(o.value("param").toObject());
  return 0;
}

int HCVMatNode::save(const QString &path) {
  QJsonObject o;
  if (0 != save(o)) {
    return -1;
  }
  return HJsonCommon::writeJson(path, o);
}

int HCVMatNode::load(const QString &path) {
  QJsonObject o;
  if (0 != HJsonCommon::readJsonObject(path, o)) {
    return -1;
  }
  return load(o);
}

void HCVMatNode::setSplitSize(const cv::Size &size) { _split_size = size; }

cv::Size HCVMatNode::getSplitSize() { return _split_size; }

void HCVMatNode::setMat(const cv::Mat &mat) { _mat = mat; }

cv::Mat HCVMatNode::getMat() { return _mat; }

QImage HCVMatNode::CVMat2Qimage(const cv::Mat &mat) {
  if (mat.type() == CV_8UC1) {
    QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);
    image.setColorCount(256);
    for (int i = 0; i < 256; i++) {
      image.setColor(i, qRgb(i, i, i));
    }
    uchar *pSrc = mat.data;
    for (int row = 0; row < mat.rows; row++) {
      uchar *pDest = image.scanLine(row);
      memcpy(pDest, pSrc, static_cast<std::size_t>(mat.cols));
      pSrc += mat.step;
    }
    return image;
  } else if (mat.type() == CV_8UC3) {
    const uchar *pSrc = const_cast<const uchar *>(mat.data);
    QImage image(pSrc, mat.cols, mat.rows, int(mat.step),
                 QImage::Format_RGB888);

    return image.rgbSwapped();
  } else if (mat.type() == CV_8UC4) {
    const uchar *pSrc = const_cast<const uchar *>(mat.data);
    QImage image(pSrc, mat.cols, mat.rows, int(mat.step),
                 QImage::Format_ARGB32);
    return image.copy();
  }
  return QImage();
}

QSGNode *HCVMatNode::BuildQImageNode(const QImage &image, HBoard *board,
                                     const QRectF &rect) {
  if (image.isNull()) {
    DEBUG << "image is null";
    return nullptr;
  }
  QSGSimpleTextureNode *node = nullptr;
  if (board) {
    node = new QSGSimpleTextureNode();
    QSGTexture *texture = board->window()->createTextureFromImage(image);
    node->setTexture(texture);
    node->setRect(rect);
  }
  return node;
}
