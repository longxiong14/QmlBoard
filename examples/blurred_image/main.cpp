#include <QDebug>
#include <QGuiApplication>
#include <QObject>
#include <QPainter>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <memory>

#include "../../Common/hthreadpool.h"
#include "../../HChart/hhistogramchart.h"
#include "../../Handles/hhandlearrow.h"
#include "../../Handles/hhandlebase.h"
#include "../../Handles/hhandleflyweight.h"
#include "../../Handles/hhandlemove.h"
#include "../../Handles/hhandlepaint.h"
#include "../../Nodes/hblurredimage.h"
#include "../../Nodes/hfillnode.h"
#include "../../Nodes/hnodebase.h"
#include "../../Nodes/hshapenodes.h"
#include "../../hboardmanager.h"
#include "../../hboarduicontrol.h"
#include "../../himagemapboard.h"
#include "maincontrol.h"

int main(int argc, char *argv[]) {
  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
  QCoreApplication::setOrganizationName("qmlboard chart example");

  QGuiApplication app(argc, argv);

  QQmlApplicationEngine engine;
  MainControl control;

  qmlRegisterType<HBlurredImageBoard>("hBoard", 1, 0, "HBoard");

  auto root = engine.rootContext();
  root->setContextProperty("gCtrl", &control);
  const QUrl url(QStringLiteral("qrc:/main.qml"));
  QObject::connect(
      &engine, &QQmlApplicationEngine::objectCreated, &app,
      [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl) QCoreApplication::exit(-1);
      },
      Qt::QueuedConnection);
  engine.load(url);

  auto board = HBoardManager::getInstance()->getBoard("test_board");
  if (board) {
    auto node = std::make_shared<HShapeRectNode>(
        QRectF(0, 0, 100, 100),
        QJsonObject(
            {{"b", 0}, {"r", 255}, {"g", 0}, {"a", 255}, {"line_width", 1}}));
    board->pushNode(node);
  }

  return app.exec();
}
