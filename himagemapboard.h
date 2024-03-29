﻿#ifndef HIMAGEMAPBOARD_H
#define HIMAGEMAPBOARD_H
#include "hboard.h"
class HBOARD_EXPORT HImageMapBoard : public HBoard {
  Q_OBJECT
  Q_PROPERTY(bool debug READ debug WRITE setDebug NOTIFY debugChanged)
 public:
  HImageMapBoard(QQuickItem *parent = nullptr);

  virtual void home() override;

  virtual void pushNode(std::shared_ptr<HNodeBase> node,
                        bool flag = true) override;

  virtual void removeNode(const QUuid &id) override;

  virtual void pushTransform(const QTransform &trans) override;

  virtual void face(double x, double y) override;

  virtual void setScale(double scale) override;

  virtual void moveNode(const QUuid &n, QPointF dlt) override;
  virtual void nodeMoveTo(const QUuid &n, QPointF point) override;

  virtual QSGNode *updatePaintNode(QSGNode *node,
                                   UpdatePaintNodeData *) override;

  virtual bool updateNodeMat(const QUuid &node, const QImage &mat,
                             const QPointF &start) override;

 public:
  bool debug();
  void setDebug(bool f);

 protected:
  void updateImages();

  void clearImageNodes();
  void updateImageTask();

  QList<std::shared_ptr<HNodeBase>> getZOrderNodes();
 signals:
  void debugChanged();

 public slots:

 protected:
  QSGNode *_image_node;
  bool _debug;
};

#endif  // HIMAGEMAPBOARD_H
