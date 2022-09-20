#ifndef HIMAGEMAPNODE_H
#define HIMAGEMAPNODE_H

#include "../HBoard_global.h"
#include "QImage"
#include "hfillnode.h"

class HBOARD_EXPORT HImageMapNodeDelegate : public HFillNode {
 public:
  HImageMapNodeDelegate(const QRectF &rect = QRectF());

  virtual NODETYPE nodeType() override;

  virtual QImage getImage(const QRectF &roi, double scale) = 0;
};

class HBOARD_EXPORT HImageMapNode : public HImageMapNodeDelegate {
 public:
  HImageMapNode();
  HImageMapNode(const QString &path, const QPointF &start_point = QPointF());
  HImageMapNode(const QImage &mat, const QPointF &start_point = QPointF());
  HImageMapNode(const HImageMapNode &node) = default;
  HImageMapNode &operator=(const HImageMapNode &node) = default;
  virtual ~HImageMapNode() override;

  // start: relative position
  virtual void updateMat(HBoard *board, const QImage &mat,
                         const QPointF &start) override;

  virtual QImage getImage(const QRectF &roi, double scale) override;

  QImage getImage();
  void setImage(const QImage &i);

 protected:
  QImage _image;
};

#endif  // HIMAGEMAPNODE_H
