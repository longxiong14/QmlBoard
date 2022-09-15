#ifndef HIMAGEMAPBOARD_H
#define HIMAGEMAPBOARD_H
#include "hboard.h"
class HBOARD_EXPORT HImageMapBoard : public HBoard {
  Q_OBJECT
 public:
  HImageMapBoard();

  virtual void pushNode(std::shared_ptr<HNodeBase> node,
                        bool flag = true) override;

  virtual void pushTransform(const QTransform &trans) override;

  virtual QSGNode *updatePaintNode(QSGNode *node,
                                   UpdatePaintNodeData *) override;

 signals:

 public slots:

 protected:
  QSGNode *_image_node;
  QSGNode *_shape_node;
};

#endif  // HIMAGEMAPBOARD_H
