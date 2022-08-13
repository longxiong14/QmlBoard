#include "hdrawmathandle.h"
#include "../Nodes/hnodebase.h"
#include "../hboard.h"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include <QDebug>
#define DEBUG qDebug() << __FUNCTION__ << " " << __LINE__ << " "
HDrawMatHandle::HDrawMatHandle() {}

void HDrawMatHandle::mousePressEvent(HBoard *board, QMouseEvent *event,
                                     const QJsonObject &object) {
  HHandleMove::mousePressEvent(board, event, object);
  if (board && event) {
    auto sel = board->selects();
    for (const auto &k : sel) {
      auto n = board->getNodeById(k);
      if (HNodeBase::NODETYPE::IMAGE == n->nodeType()) {
        auto mat = cv::imread("C:\\Users\\longxiong\\Pictures\\ttt.png");
        if (mat.empty()) {
          DEBUG << "mat empty";
        }
        board->updateNodeMat(k, mat, QPointF(0, 0));
      }
    }
  }
}
