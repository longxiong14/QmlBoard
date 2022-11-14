﻿#include <QDebug>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQmlEngine>

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
#include "hthumbnailboard.h"
#include "maincontrol.h"
int main(int argc, char *argv[]) {
  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
  QCoreApplication::setOrganizationName("qmlboard picture example");

  QGuiApplication app(argc, argv);

  QQmlApplicationEngine engine;
  MainControl control;

  auto root = engine.rootContext();
  qmlRegisterType<HImageMapBoard>("hBoard", 1, 0, "HBoard");
  qmlRegisterType<HBoardUIControl>("hUIControl", 1, 0, "HUIControl");
  qmlRegisterType<HThumbnailBoard>("hThumbnail", 1, 0, "HThumbnail");
  root->setContextProperty("gCtrl", &control);
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
