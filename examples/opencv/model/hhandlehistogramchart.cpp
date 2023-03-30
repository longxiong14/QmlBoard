#include "hhandlehistogramchart.h"

#include <QDebug>
#include <QRgb>
#include <QUuid>

#include "../../Nodes/himagemapnode.h"
#include "../../Nodes/hnodebase.h"
#include "../../hboard.h"
#include "../../hboardmanager.h"
#include "../../hboarduicontrol.h"
#include "hcvmatboard.h"
#define DEBUG qDebug() << __FUNCTION__ << __LINE__
HHandleHistogramChart::HHandleHistogramChart(QObject *parent)
    : QObject(parent), _image_node(nullptr) {
  _name = "histogram char";
}

void HHandleHistogramChart::mouseReleaseEvent(HBoard *board, QMouseEvent *event,
                                              const QJsonObject &object) {
  (void)object;
  if (board && event && isButtonPress(event)) {
    board->removeNode(_last_node);
    if (!_node.isNull()) {
      auto b = dynamic_cast<HCVMatBoard *>(board);
      if (b) {
        auto mat = b->getMaskNode();
      }
    }
    _last_node = _node;
  }
  HHandleDrawRect::mouseReleaseEvent(board, event, object);
}
