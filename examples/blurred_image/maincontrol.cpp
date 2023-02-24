#include "maincontrol.h"

#include <QPainter>

#include "../../Nodes/hblurredimage.h"
#include "../../Nodes/himagemapnode.h"
#include "../../hboard.h"
#include "../../hboardmanager.h"

static QImage _image;
static QUuid _blurred;
static QUuid _src;
MainControl::MainControl(QObject *parent) : QObject(parent) {}

int MainControl::open(const QString &path) {
  auto board = HBoardManager::getInstance()->getBoard("test_board");
  if (!board) {
    return -1;
  }
  board->clearNode();
  _image = QImage(path);
  if (_image.isNull()) {
    return -1;
  }
  double scale = 0.01;
  auto s = QRectF(0, 0, _image.width() * scale, _image.height() * scale);

  QImage out(s.toRect().size(), _image.format());
  if (!out.isNull()) {
    QPainter painter(&out);
    painter.drawImage(s, _image);
  }

  auto node = std::make_shared<HBlurredImageNode>(out, QPointF(), scale);
  node->setColor(QColor(0, 0, 0, 0));
  _blurred = node->id();
  node->setZOrder(-1);
  board->pushNode(node);

  return 0;
}

HBlurredImageBoard::HBlurredImageBoard(QQuickItem *parent)
    : HImageMapBoard(parent) {}

void HBlurredImageBoard::mouseReleaseEvent(QMouseEvent *event) {
  HImageMapBoard::mouseReleaseEvent(event);
  if (!_src.isNull()) removeNode(_src);

  auto roi = getWCSBoundRect();
  roi &= _image.rect();
  //  auto scale = getScale();
  auto scale = 1.0;
  auto s = QRectF(0, 0, roi.width() * scale, roi.height() * scale);
  QImage out(s.toRect().size(), _image.format());
  if (!out.isNull()) {
    QPainter painter(&out);
    painter.drawImage(s, _image, roi, Qt::ThresholdDither);
  }
  auto node = std::make_shared<HImageMapNode>(out, roi.topLeft());
  node->setColor(QColor(0, 0, 0, 0));
  _src = node->id();
  pushNode(node);
}

HBlurredImage2::HBlurredImage2(const QImage &image, const QPointF &point,
                               double scale)
    : HBlurredImageNode(image, point, scale) {}
