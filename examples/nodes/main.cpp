#include <QDebug>
#include <QGuiApplication>
#include <QPointF>
#include <QQmlApplicationEngine>

#include "../../Common/hthreadpool.h"
#include "../../HChart/hhistogramchart.h"
#include "../../Handles/hhandlearrow.h"
#include "../../Handles/hhandleflyweight.h"
#include "../../Handles/hhandlemove.h"
#include "../../Handles/hhandlepaint.h"
#include "../../Nodes/hfillnode.h"
#include "../../Nodes/hnodebase.h"
#include "../../Nodes/hshapenodes.h"
#include "../../hboardmanager.h"
#include "../../hboarduicontrol.h"
#include "../../himagemapboard.h"
int main(int argc, char *argv[]) {
  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

  QGuiApplication app(argc, argv);

  QQmlApplicationEngine engine;

  qmlRegisterType<HImageMapBoard>("hBoard", 1, 0, "HBoard");

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
    {
      auto node = std::make_shared<HFillNode>(QRectF(0, 0, 100, 200));
      node->setText("fill node rect");
      board->pushNode(node);
    }
    {
      QList<QPointF> list{{300, 50}, {400, 200}, {500, 100}, {600, 64}};
      auto node = std::make_shared<HFillNode>(list, GL_LINE_STRIP);
      node->setText("fill node list");
      board->pushNode(node);
    }
    {
      QList<QPointF> list{{0, 300}, {100, 300}};
      auto node = std::make_shared<HShapeLineNode>(
          QPointF(0, 500), QPointF(100, 600), QJsonObject());
      node->setText("shape line node");
      board->pushNode(node);
    }
    {
      auto node = std::make_shared<HShapeRectNode>(QRectF(200, 300, 100, 100),
                                                   QJsonObject());
      node->setText("shape rect node");
      board->pushNode(node);
    }

    {
      QList<QPointF> list{{300, 250}, {400, 400}, {500, 300}, {600, 264}};
      auto node = std::make_shared<HShapeCurveNode>(list, QJsonObject());
      node->setText("shape curve node");
      board->pushNode(node);
    }
    {
      QList<QPointF> list{
          {600, 250}, {700, 400}, {800, 300}, {900, 264}, {600, 250}};
      auto node = std::make_shared<HShapePolyNode>(list, QJsonObject());
      node->setText("shape poly node");
      board->pushNode(node);
    }
    {
      auto node = std::make_shared<HShapeFillRectNode>(
          QRectF(200, 500, 100, 100), QJsonObject());
      node->setText("shape fill rect node");
      board->pushNode(node);
    }
    {
      QJsonObject o;
      o.insert("radius", 100);
      o.insert("b", 0);
      o.insert("g", 0);
      o.insert("r", 255);
      o.insert("a", 255);
      auto node = std::make_shared<HShapeCircleNode>(QPointF(550, 550), o);
      node->setText("shape circle node");
      board->pushNode(node);
    }
    {
      QJsonObject o;
      o.insert("radius", 100);
      o.insert("b", 0);
      o.insert("g", 0);
      o.insert("r", 255);
      o.insert("a", 255);
      auto node = std::make_shared<HShapeFillCircleNode>(QPointF(800, 550), o);
      node->setText("shape fill circle node", QRectF(0, -30, 100, 30));
      board->pushNode(node);
    }

    {
      QJsonObject o;
      o.insert("radius", 100);
      o.insert("b", 0);
      o.insert("g", 0);
      o.insert("r", 255);
      o.insert("a", 255);
      auto node = std::make_shared<HShapeFillCircleNode>(QPointF(800, 550), o);
      node->setText("shape circle node", QRectF(0, -30, 100, 30));
      board->pushNode(node);
    }

    {
      QJsonObject o;
      o.insert("b", 0);
      o.insert("g", 0);
      o.insert("r", 255);
      o.insert("a", 255);
      auto node = std::make_shared<HShapeXNode>(QPointF(0, 700), 50, o);
      node->setText("shape x node");
      board->pushNode(node);
    }

    {
      QJsonObject o;
      o.insert("b", 0);
      o.insert("g", 0);
      o.insert("r", 255);
      o.insert("a", 255);
      auto node = std::make_shared<HShapeCrossNode>(QPointF(200, 700), 50, o);
      node->setText("shape cross node");
      board->pushNode(node);
    }
    {
      auto node =
          std::make_shared<HShapeEllipseNode>(QRectF(300, 700, 100, 200));
      node->setText("shape ellipse node");
      board->pushNode(node);
    }

    {
      auto node =
          std::make_shared<HShapeFillEllipseNode>(QRectF(600, 700, 200, 100));
      node->setText("shape fill ellipse node", QRectF(0, -30, 100, 30));
      board->pushNode(node);
    }

    board->home();
  }

  return app.exec();
}
