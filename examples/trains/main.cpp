#include <QDebug>
#include <QGuiApplication>
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
#include "hnodescontrol.h"
#include "htrans.h"
int main(int argc, char *argv[]) {
  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
  QCoreApplication::setOrganizationName("qmlboard nodes example");

  QGuiApplication app(argc, argv);

  QQmlApplicationEngine engine;

  qmlRegisterType<HImageMapBoard>("hBoard", 1, 0, "HBoard");
  qmlRegisterType<HNodesControl>("hNodes", 1, 0, "HNodes");
  qmlRegisterType<HBoardUIControl>("hUIControl", 1, 0, "HUIControl");
  //  HNodesControl()

  HTranslator trans;
  trans.beforeUiReady(engine.rootContext(), "./configs/");

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
