#ifndef HIMAGENODE_H
#define HIMAGENODE_H
#include <QSGSimpleTextureNode>

#include "hnodebase.h"
class HBOARD_EXPORT HImageNode : public HNodeBase, public QSGSimpleTextureNode {
 public:
  HImageNode(const QString& path, const QRect& rect = QRect());

  virtual QSGNode* build(HBoard* board) override;
  virtual QRect getBoundRect() override;
  virtual void changedSelectStatus() override;
  virtual void move(const QPoint&) override;

 private:
  QString _path;
  QRect _rect;
};

#endif  // HIMAGENODE_H
