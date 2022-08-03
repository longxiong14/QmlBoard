#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "../Handles/hhandlearrow.h"
#include "../Handles/hhandleflyweight.h"
#include "../Handles/hhandlemove.h"
#include "../Handles/hhandlepaint.h"
#include "../Nodes/hcvmatnode.h"
#include "../Nodes/hfillnode.h"
#include "../Nodes/himagenode.h"
#include "../Nodes/hnodebase.h"
#include "../hboard.h"
#include "../hboardmanager.h"
#include "../hboarduicontrol.h"
int main(int argc, char *argv[]) {
  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

  QGuiApplication app(argc, argv);

  QQmlApplicationEngine engine;

  qmlRegisterType<HBoard>("hBoard", 1, 0, "HBoard");
  qmlRegisterType<HBoardUIControl>("hUIControl", 1, 0, "HUIControl");

  const QUrl url(QStringLiteral("qrc:/main.qml"));
  QObject::connect(
      &engine, &QQmlApplicationEngine::objectCreated, &app,
      [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl) QCoreApplication::exit(-1);
      },
      Qt::QueuedConnection);
  engine.load(url);

  QString path = "C:\\Users\\xiaolong\\Pictures\\test.jpg";
  {
    auto board = HBoardManager::getInstance()->getBoard("test_board");
    //    board->pushNode(new
    //    HImageNode("C:\\Users\\xiaolong\\Pictures\\ttt.png"));
    auto node = new HCVMatNode("C:\\Users\\xiaolong\\Pictures\\ttt.png",
                               QPointF(100, 100));
    //    auto node = new HCVMatNode(path);
    board->pushNode(node);
    board->home();
  }

  return app.exec();
}
