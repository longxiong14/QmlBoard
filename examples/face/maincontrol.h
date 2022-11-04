#ifndef MAINCONTROL_H
#define MAINCONTROL_H

#include <QObject>

class MainControl : public QObject {
  Q_OBJECT
 public:
  explicit MainControl(QObject* parent = nullptr);

  Q_INVOKABLE
  int face(const QString& name, int x, int y);

  Q_INVOKABLE
  int moveTo(const QString& name, int x, int y);

 signals:

 public slots:
};

#endif  // MAINCONTROL_H
