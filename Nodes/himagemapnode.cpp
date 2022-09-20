#include "himagemapnode.h"

#include <QDebug>
#include <QPainter>

#include "hfillnode.h"
#define DEBUG qDebug() << __FUNCTION__ << __LINE__
HImageMapNodeDelegate::HImageMapNodeDelegate(const QRectF &rect)
    : HFillNode(rect) {}

HNodeBase::NODETYPE HImageMapNodeDelegate::nodeType() {
  return HNodeBase::NODETYPE::MAPINAGE;
}

HImageMapNode::HImageMapNode() : HImageMapNodeDelegate() {}

HImageMapNode::HImageMapNode(const QString &path, const QPointF &start_point)
    : HImageMapNodeDelegate(), _image(path) {
  setRect(QRectF(start_point, _image.size()));
}

HImageMapNode::HImageMapNode(const QImage &mat, const QPointF &start_point)
    : _image(mat) {
  setRect(QRectF(start_point, _image.size()));
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
  if (!out.isNull()) {
    QPainter painter(&out);
    painter.drawImage(s, _image, roi);
  }
  return out;
}
