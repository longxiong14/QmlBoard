#ifndef HIMAGENODE_H
#define HIMAGENODE_H
#include <QSGSimpleTextureNode>

#include "hnodebase.h"
class HBOARD_EXPORT HImageNode : public HNodeBase, public QSGSimpleTextureNode {
 public:
  HImageNode();
  HImageNode(const QString& path, const QRectF& rect = QRectF());

  virtual QSGNode* build(HBoard* board) override;
  virtual QSGNode* get() override;
  virtual QRectF getBoundRect() override;
  virtual QList<QPointF> getPointList() override;
  virtual void move(const QPointF&) override;
  virtual NODETYPE nodeType() override;

 public:
  void setPath(const QString& path);
  QString getPath();

 private:
  QString _path;
  QRectF _rect;
};

#endif  // HIMAGENODE_H
