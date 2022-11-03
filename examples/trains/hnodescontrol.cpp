#include "hnodescontrol.h"

#include <QDebug>
#include <QGuiApplication>
#include <QJsonObject>
#include <QPointF>
#include <QQmlApplicationEngine>

#include "../../Common/hjsoncommon.h"
#include "../../Common/hthreadpool.h"
#include "../../HChart/hhistogramchart.h"
#include "../../Handles/hhandlearrow.h"
#include "../../Handles/hhandleflyweight.h"
#include "../../Handles/hhandlemove.h"
#include "../../Handles/hhandlepaint.h"
#include "../../Nodes/hfillnode.h"
#include "../../Nodes/hnodebase.h"
#include "../../Nodes/hreviewnodemanager.h"
#include "../../Nodes/hshapenodes.h"
#include "../../hboardmanager.h"
#include "../../hboarduicontrol.h"
#include "../../himagemapboard.h"
#define DEBUG qDebug() << __FUNCTION__ << __LINE__
HNodesControl::HNodesControl(QObject *parent) : QObject(parent) {}

int HNodesControl::push() {
  QJsonObject object;
  auto path =
      "G:\\FpcCode\\FPC2.0\\build-fpcmanager-Desktop_Qt_5_13_2_MSVC2017_"
      "64bit-Release-with-Debug-Information\\bin\\configs\\local_test_"
      "data\\open_"
      "gerber.json";
  HJsonCommon::readJsonObject(path, object);
  auto value = object.value("value").toObject();
  auto panel_contours = value.value("panel_contours").toObject();

  QList<QString> keys{"Circuit", "Coverlay", "FlexibleCircuit"};
  for (const auto &k : keys) {
    auto layer = panel_contours.value(k).toObject();
    auto contours = layer.value("contours").toArray();
    auto review = std::make_shared<HReviewNodeManager>("test_board");
    for (int i = 0; i < contours.size(); i++) {
      auto con = contours[i].toArray();
      QList<QPointF> list;
      QPointF point;
      for (int j = 0; j < con.size(); j++) {
        if (j % 2 == 0) {
          point.setX(con[j].toDouble());
        } else {
          point.setY(con[j].toDouble());
          list.push_back(point);
        }
      }
      auto node = std::make_shared<HFillNode>(list);
      node->setFlag(HNodeBase::NODEFLAG::CANSELECT, false);
      review->pushNode(node->id().toString(), node);
      _reviews.push_back(review);
    }
  }
  return 0;
}

int HNodesControl::clear() {
  _reviews.clear();
  return 0;
}

void HNodesControl::test() {
  std::vector<int> vec;
  auto pool = std::make_shared<HSyncThreadPool>(100);
  std::mutex mutex;
  for (int i = 0; i < 100000; i++) {
    pool->push([&vec, &mutex, i]() {
      std::unique_lock<std::mutex> lock(mutex);
      vec.push_back(i);
      DEBUG << vec.size();
    });
  }

  pool->run();
  DEBUG << vec.size();

  //  std::condition_variable condition;
  //  auto pool = std::make_shared<HThreadPool>(100, [&]() {
  //    condition.notify_one();
  //    DEBUG << "task end";
  //  });
  //  std::mutex mutex, vec_mutex;
  //  for (int i = 0; i < 100000; i++) {
  //    pool->push([&vec, &vec_mutex, i]() {
  //      std::unique_lock<std::mutex> lock(vec_mutex);
  //      vec.push_back(i);
  //      DEBUG << vec.size();
  //    });
  //  }
  //  pool->run();
  //  std::unique_lock<std::mutex> lock(mutex);
  //  condition.wait(lock);
}
