#include <QDebug>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQmlEngine>
#include <opencv2/core.hpp>

#include "../../Common/hthreadpool.h"
#include "../../HChart/hhistogramchart.h"
#include "../../Handles/hhandlearrow.h"
#include "../../Handles/hhandleflyweight.h"
#include "../../Handles/hhandlemove.h"
#include "../../Handles/hhandlepaint.h"
#include "../../Nodes/hfillnode.h"
#include "../../Nodes/hnodebase.h"
#include "../../hboardmanager.h"
#include "../../hboarduicontrol.h"
#include "../../himagemapboard.h"
#include "control/huicontrol.h"
#include "model/huimodel.h"
int main(int argc, char *argv[]) {
  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
  QCoreApplication::setOrganizationName("qmlboard picture example");
  QGuiApplication app(argc, argv);
  QQmlApplicationEngine engine;

  HUIModel model;

  auto root = engine.rootContext();
  qmlRegisterType<HImageMapBoard>("hBoard", 1, 0, "HBoard");
  qmlRegisterType<HBoardUIControl>("hUIControl", 1, 0, "HUIControl");
  root->setContextProperty("gModel", &model);
  const QUrl url(QStringLiteral("qrc:/main.qml"));
  QObject::connect(
      &engine, &QQmlApplicationEngine::objectCreated, &app,
      [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl) QCoreApplication::exit(-1);
      },
      Qt::QueuedConnection);
  engine.load(url);

  return app.exec();
}
