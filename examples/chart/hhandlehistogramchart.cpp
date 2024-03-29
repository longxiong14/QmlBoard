﻿#include "hhandlehistogramchart.h"

#include <QDebug>
#include <QRgb>
#include <QUuid>

#include "../../Nodes/himagemapnode.h"
#include "../../Nodes/hnodebase.h"
#include "../../hboard.h"
#include "../../hboardmanager.h"
#include "../../hboarduicontrol.h"
#define DEBUG qDebug() << __FUNCTION__ << __LINE__
HHandleHistogramChart::HHandleHistogramChart(QObject *parent)
    : QObject(parent), _image_node(nullptr) {}

void HHandleHistogramChart::mouseReleaseEvent(HBoard *board, QMouseEvent *event,
                                              const QJsonObject &object) {
  (void)object;
  if (board && event && isButtonPress(event)) {
    board->removeNode(_last_node);
    if (!_node.isNull()) {
      auto rect_node = board->getNodeById(_node);
      if (rect_node) {
        if (_image_node) {
          auto node_rect =
              rect_node->getBoundRect() & _image_node->getBoundRect();
          auto topleft = _image_node->getBoundRect().topLeft();

          QRectF rect(node_rect.topLeft() - topleft, node_rect.size());
          auto image = _image_node->getImage(rect, 1.0);
          unsigned int arr[3][256] = {{0}, {0}, {0}};
          if (!image.isNull()) {
            if (QImage::Format_RGB32 == image.format() ||
                QImage::Format_ARGB32 == image.format()) {
              for (int x = 0; x < image.width(); x++) {
                for (int y = 0; y < image.height(); y++) {
                  auto px = image.pixelColor(x, y);
                  arr[0][px.blue()]++;
                  arr[1][px.green()]++;
                  arr[2][px.red()]++;
                }
              }
            } else if (QImage::Format_Grayscale8 == image.format()) {
              for (int l = 0; l < image.height(); l++) {
                auto line = image.scanLine(l);
                if (line) {
                  for (int w = 0; w < image.width(); w++) {
                    arr[0][line[w]]++;
                  }
                }
              }
            }
          }
          QJsonArray array;
          for (int i = 0; i < 3; i++) {
            QJsonArray tmp;
            for (int j = 0; j < 256; j++) {
              tmp.push_back(int(arr[i][j]));
            }
            array.push_back(tmp);
          }
          setCharts(array);
        }
      }
    }
    _last_node = _node;
  }
  HHandleDrawRect::mouseReleaseEvent(board, event, object);
}

QJsonArray HHandleHistogramChart::charts() { return _chart_array; }

void HHandleHistogramChart::setCharts(const QJsonArray &arr) {
  _chart_array = arr;
  chartsChanged();
}

int HHandleHistogramChart::openPicture(const QString &path,
                                       const QString &name) {
  //
  auto board = HBoardManager::getInstance()->getBoard(name);
  if (!board)
    return -1;
  board->clearNode();
  if (board) {
    _image_node = std::make_shared<HImageMapNode>(path);
    board->pushNode(_image_node);
    board->home();
  }
  return 0;
  //  HBoardUIControl control;
  //  return control.openBoardPicture(name, path);
}
