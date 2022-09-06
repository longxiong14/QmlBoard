#ifndef HIMAGENODE_H
#define HIMAGENODE_H
#include <QSGSimpleTextureNode>

#include "hnodebase.h"
class QSGImageNode;
class HBOARD_EXPORT HImageNode : public HNodeBase {
 public:
  HImageNode();
  HImageNode(const QString &path, const QPointF &start_point = QPointF());
  HImageNode(const QImage &mat, const QPointF &start_point = QPointF());
  virtual ~HImageNode() override;

  virtual QSGNode *build(HBoard *) override;
  virtual QSGNode *get() override;
  virtual QRectF getBoundRect() override;
  virtual QList<QPointF> getPointList() override;
  virtual void move(const QPointF &) override;
  virtual NODETYPE nodeType() override;

  // start: relative position
  virtual void updateMat(HBoard *board, const QImage &mat,
                         const QPointF &start);

  // roi: relative rectangle
  virtual void updateRoi(HBoard *board, const QRectF &roi);

 public:
  virtual int save(QJsonObject &o) override;
  virtual int load(const QJsonObject &o) override;
  virtual int save(const QString &path) override;
  virtual int load(const QString &path) override;

 public:
  void setSplitSize(const QSize &size);
  QSize getSplitSize();

  void setMat(const QImage &mat);
  QImage getMat();

 public:
  QSGImageNode *BuildQImageNode(const QImage &image, HBoard *board,
                                const QRectF &rect);

 protected:
  QImage _mat;
  QSize _split_size;
  QSGNode *_node;
  QRectF _bound_rect;
  QPointF _start_point;
};

#endif  // HIMAGENODE_H
