#include "himagemapnode.h"

#include <QDebug>
#include <QPainter>

#include "../Common/hplanvector.h"
#include "hfillnode.h"
#define DEBUG qDebug() << __FUNCTION__ << __LINE__

HImageMapNodeDelegate::HImageMapNodeDelegate(const QRectF &rect)
    : HFillNode(rect, GL_LINE_LOOP,
                QJsonObject({{"b", 0},
                             {"g", 0},
                             {"r", 255},
                             {"a", 255},
                             {"line_width", 1}})) {}

HNodeBase::NODETYPE HImageMapNodeDelegate::nodeType() {
  return HNodeBase::NODETYPE::MAPINAGE;
}

HImageMapNode::HImageMapNode() : HImageMapNodeDelegate() {}

HImageMapNode::HImageMapNode(const QString &path, const QPointF &start_point)
    : HImageMapNodeDelegate(), _image(path) {
  setRect(QRectF(start_point, _image.size()), GL_LINE_LOOP,
          QJsonObject(
              {{"b", 0}, {"g", 0}, {"r", 255}, {"a", 255}, {"line_width", 1}}));
}

HImageMapNode::HImageMapNode(const QImage &mat, const QPointF &start_point)
    : _image(mat) {
  setRect(QRectF(start_point, _image.size()), GL_LINE_LOOP,
          QJsonObject(
              {{"b", 0}, {"g", 0}, {"r", 255}, {"a", 255}, {"line_width", 1}}));
}

HImageMapNode::~HImageMapNode() {}

void HImageMapNode::updateMat(HBoard *, const QImage &mat,
                              const QPointF &start) {
  QPainter p(&_image);
  auto roi = QRectF(start, mat.size());
  p.drawImage(roi, mat);
}

QImage HImageMapNode::getImage() { return _image; }

void HImageMapNode::setImage(const QImage &i) { _image = i; }

QImage HImageMapNode::getImage(const QRectF &roi, double scale) {
  auto s = QRectF(0, 0, roi.width() * scale, roi.height() * scale);
  QImage out(s.toRect().size(), _image.format());
  HPlanVector vec;
  if (!out.isNull()) {
    QPainter painter(&out);
    //    double rotate = -20;
    //    QPointF out;
    //    vec.pointRotateByOtherPoint(QPointF(), s.center(), rotate, out);
    //    painter.translate(out);
    //    painter.rotate(rotate);
    painter.drawImage(s, _image, roi, Qt::ThresholdDither);
  }
  return out;
}
