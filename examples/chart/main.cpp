#include <QDebug>
#include <QGuiApplication>
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
#include "../../Nodes/hfillnode.h"
#include "../../Nodes/hnodebase.h"
#include "../../hboardmanager.h"
#include "../../hboarduicontrol.h"
#include "../../himagemapboard.h"
#include "hhandlehistogramchart.h"
int main(int argc, char *argv[]) {
  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
  QCoreApplication::setOrganizationName("qmlboard chart example");

  QGuiApplication app(argc, argv);

  QQmlApplicationEngine engine;

  qmlRegisterType<HImageMapBoard>("hBoard", 1, 0, "HBoard");
  qmlRegisterType<HHistogramChart>("hHistogramChart", 1, 0, "HHistogramChart");
  auto context = engine.rootContext();
  auto handle = std::make_shared<HHandleHistogramChart>();
  context->setContextProperty("gHandle", handle.get());

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
    board->setHandle(handle);
  }

  return app.exec();
}
