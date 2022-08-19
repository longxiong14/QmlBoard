#include "hhandlerule.h"

#include <QDebug>
#include <QSGGeometryNode>

#include "../Common/hcommons.h"
#include "../Common/hplanvector.h"
#include "../Nodes/hfillnode.h"
#include "hboard.h"
#define DEBUG qDebug() << __FUNCTION__ << __LINE__
HHandleRule::HHandleRule() { _name = "rule"; }

void HHandleRule::mousePressEvent(HBoard *board, QMouseEvent *event,
                                  const QJsonObject &) {
  HHandleMove::mousePressEvent(board, event);
  if (board && event && isButtonPress(event)) {
    _point = board->WCS2LCS(event->pos());
    QList<QPointF> list{_point};
    auto node = std::make_shared<HFillNode>(list, GL_LINES);
    board->pushNode(node);
    _node = node->id();
  }
}

void HHandleRule::mouseMoveEvent(HBoard *board, QMouseEvent *event,
                                 const QJsonObject &o) {
  HHandleMove::mouseMoveEvent(board, event);
  if (board && event && leftButtonPress(event)) {
    auto point = board->WCS2LCS(event->pos());
    QList<QPointF> list{_point, point};
    board->drawNodePoint(_node, list);
    auto rect = HCommon::BuildRect(_point, point);

    HPlanVector vec;
    auto dims = vec.dis(_point, point);
    auto dlt_x = std::fabs(point.x() - _point.x());
    auto dlt_y = std::fabs(point.y() - _point.y());

    QString length = QString("length:%1").arg(dims);
    double size = o.value("size").toDouble();
    auto d_width = (length.size() + 2) * size;
    auto d_height = 6 * size;
    QRectF d((rect.width() - d_width) / 2, (rect.height() - d_height) / 2,
             d_width, d_height);

    QString txt = QString("dx:%1\ndy:%2\n%3").arg(dlt_x).arg(dlt_y).arg(length);
    board->updateNodeText(_node, txt, d, int(size));
  }
}

QJsonObject HHandleRule::getDefaultParam() {
  QJsonObject object;
  object.insert("size", 32);
  return object;
}
