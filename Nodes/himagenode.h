#ifndef HIMAGENODE_H
#define HIMAGENODE_H
#include <QSGSimpleTextureNode>

#include "hnodebase.h"
class HBOARD_EXPORT HImageNode : public HNodeBase, public QSGSimpleTextureNode {
 public:
  HImageNode(const QString& path, const QRectF& rect = QRectF());

  virtual QSGNode* build(HBoard* board) override;
  virtual QRect getBoundRect() override;
  virtual QList<QPoint> getPointList() override;
  virtual void changedSelectStatus() override;
  virtual void move(const QPoint&) override;
  virtual SELECTTYPE selectType() override;
  virtual void setVisible(bool) override;
  virtual bool visible() override;

 private:
  QString _path;
  QRectF _rect;
};

#endif  // HIMAGENODE_H
