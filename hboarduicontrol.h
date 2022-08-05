#ifndef HBOARDUICONTROL_H
#define HBOARDUICONTROL_H

#include <QJsonArray>
#include <QJsonObject>
#include <QObject>

#include "HBoard_global.h"
class HBOARD_EXPORT HBoardUIControl : public QObject {
  Q_OBJECT
 public:
  explicit HBoardUIControl(QObject* parent = nullptr);

  Q_INVOKABLE
  int setBoardHandle(const QString& board, const QString& handle);

  Q_INVOKABLE
  int openBoardPicture(const QString& board, const QString& path);

  Q_INVOKABLE
  int setBoardHandleParam(const QString& board, const QString& handle,
                          const QString& key, const QJsonValue& value);

  Q_INVOKABLE
  QJsonObject getBoardHandleParam(const QString& board, const QString& handle);

  Q_INVOKABLE
  int setBoardNodeParam(const QString& board, const QString& key,
                        const QJsonValue& value);

  Q_INVOKABLE
  QJsonArray handleList();

  Q_INVOKABLE
  QJsonArray paramToUIItems(const QJsonObject& object);

  Q_INVOKABLE
  void test();

 public:
  void setTranslateMap(const QJsonObject& object);

 signals:

 public slots:
 protected:
  QJsonObject _translate_map;
};

#endif  // HBOARDUICONTROL_H
