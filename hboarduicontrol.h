#ifndef HBOARDUICONTROL_H
#define HBOARDUICONTROL_H

#include <QJsonArray>
#include <QObject>

#include "HBoard_global.h"
class HBOARD_EXPORT HBoardUIControl : public QObject {
  Q_OBJECT
 public:
  explicit HBoardUIControl(QObject* parent = nullptr);

  Q_INVOKABLE
  int setBoardHandle(const QString& board, const QString& handle);

  Q_INVOKABLE
  QJsonArray handleList();

  Q_INVOKABLE
  void test();

 signals:

 public slots:
};

#endif  // HBOARDUICONTROL_H
