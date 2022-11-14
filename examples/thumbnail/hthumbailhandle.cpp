#include "hthumbailhandle.h"

#include <QDebug>

#include "../../hboard.h"
#include "../../hboardmanager.h"
HThumbailHandle::HThumbailHandle() : _dst_board("max_board"), _src_scale(0.1) {}

void HThumbailHandle::mousePressEvent(HBoard *board, QMouseEvent *event,
                                      const QJsonObject &object) {
  HHandleBase::mousePressEvent(board, event, object);
  if (board && event) {
    _last_point = board->WCS2LCS(event->pos()) - board->WCS2LCS(QPointF(0, 0));
    _last_trans = board->transform();
  }
}

void HThumbailHandle::mouseMoveEvent(HBoard *board, QMouseEvent *event,
                                     const QJsonObject &o) {
  HHandleBase::mouseMoveEvent(board, event, o);
  if (board && event && _buttons.contains(Qt::LeftButton)) {
    QPointF ds = board->WCS2LCS(event->pos()) - board->WCS2LCS(QPointF(0, 0));
    QTransform trans;
    trans.translate(ds.x() - _last_point.x(), ds.y() - _last_point.y());
    board->pushTransform(trans * _last_trans);
  }
}

void HThumbailHandle::mouseReleaseEvent(HBoard *board, QMouseEvent *event,
                                        const QJsonObject &o) {
  HHandleBase::mouseReleaseEvent(board, event, o);
  auto center = board->getWCSBoundRect().center();
  auto dst = HBoardManager::getInstance()->getBoard(_dst_board);
  if (dst) {
    dst->setScale(board->getScale());
    dst->face(center.x() / _src_scale, center.y() / _src_scale);

    _last_point = QPointF();
    _last_trans = QTransform();
    if (!_node.isNull()) {
      board->clearSelect();
      board->setSelect(_node);
      _node = "";
    }
  }
}
