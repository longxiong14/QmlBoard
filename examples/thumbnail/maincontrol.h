#ifndef MAINCONTROL_H
#define MAINCONTROL_H

#include <QObject>

class MainControl : public QObject {
  Q_OBJECT
 public:
  explicit MainControl(QObject* parent = nullptr);

  Q_INVOKABLE
  int open(const QString& path);

 signals:

 public slots:
};

#endif  // MAINCONTROL_H
