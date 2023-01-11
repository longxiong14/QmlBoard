#ifndef MAINCONTROL_H
#define MAINCONTROL_H

#include <QImage>
#include <QObject>

#include "../../Nodes/hblurredimage.h"
#include "../../himagemapboard.h"
class HBlurredImage2;
class MainControl : public QObject {
  Q_OBJECT
 public:
  explicit MainControl(QObject* parent = nullptr);

  Q_INVOKABLE
  int open(const QString& path);

 signals:

 public slots:
};

class HBlurredImageBoard : public HImageMapBoard {
  Q_OBJECT
 public:
  HBlurredImageBoard(QQuickItem* parent = nullptr);

  virtual void mouseReleaseEvent(QMouseEvent* event) override;

 signals:
  void sigMouseRelease();
};

class HBlurredImage2 : public HBlurredImageNode {
 public:
  HBlurredImage2(const QImage& image, const QPointF& point, double scale);
};

#endif  // MAINCONTROL_H
