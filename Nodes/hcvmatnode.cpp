#include "hcvmatnode.h"

#include <QDebug>
#include <QImage>
#include <QQuickWindow>
#include <QSGNode>
#include <QSGSimpleTextureNode>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#include "../Common/hcommons.h"
#include "../hboard.h"

#define DEBUG qDebug() << __FUNCTION__ << " " << __LINE__ << " "

HCVMatNode::HCVMatNode(const QString &path, const QPointF &start_point)
    : HNodeBase(),
      _split_size(1920, 1080),
      _node(nullptr),
      _start_point(start_point) {
  _mat = cv::imread(path.toStdString());
  if (!_mat.empty()) {
    _bound_rect =
        QRectF(start_point.x(), start_point.y(), _mat.cols, _mat.rows);
  }
}

HCVMatNode::HCVMatNode(const cv::Mat &mat, const QPointF &start_point)
    : HNodeBase(), _mat(mat), _start_point(start_point) {
  if (!_mat.empty()) {
    _bound_rect =
        QRectF(start_point.x(), start_point.y(), _mat.cols, _mat.rows);
  }
}

QSGNode *HCVMatNode::build(HBoard *board) {
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

HNodeBase::NODETYPE HCVMatNode::nodeType() { return NODETYPE::RECTANGLE; }

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
