#ifndef HIMAGEMAPBOARD_H
#define HIMAGEMAPBOARD_H
#include "hboard.h"
class HBOARD_EXPORT HImageMapBoard : public HBoard {
  Q_OBJECT
 public:
  HImageMapBoard();

  virtual void home() override;

  virtual void pushNode(std::shared_ptr<HNodeBase> node,
                        bool flag = true) override;

  virtual void pushTransform(const QTransform &trans) override;

  virtual void moveNode(const QUuid &n, QPointF dlt) override;
  virtual void nodeMoveTo(const QUuid &n, QPointF point) override;

  virtual QSGNode *updatePaintNode(QSGNode *node,
                                   UpdatePaintNodeData *) override;

 protected:
  bool scaleChanged();

  void updateImages();

  void clearImageNodes();
  void updateImageTask();
 signals:

 public slots:

 protected:
  QSGNode *_image_node;
  QSGNode *_shape_node;
  double _scale;
};

#endif  // HIMAGEMAPBOARD_H
