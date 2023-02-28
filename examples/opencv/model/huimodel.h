#ifndef HUIMODEL_H
#define HUIMODEL_H

#include <QJsonObject>
#include <QObject>
#include <QString>
class HUIModel : public QObject {
  Q_OBJECT
  Q_PROPERTY(QJsonObject sourceChecked READ sourceChecked WRITE setSourceChecked
                 NOTIFY sourceCheckedChanged)
  Q_PROPERTY(QString dest READ dest WRITE setDest NOTIFY destChanged)
 public:
  explicit HUIModel(QObject* parent = nullptr);

  Q_INVOKABLE
  int checkedSource(const QString& board, bool checked);

 public:
  void setSourceChecked(const QJsonObject& o);
  QJsonObject sourceChecked();
  void setDest(const QString& d);
  QString dest();

 signals:
  void sourceCheckedChanged();
  void destChanged();

 public slots:

 protected:
  QJsonObject _checked_source;
  QString _dest_board;
};

#endif  // HUIMODEL_H
