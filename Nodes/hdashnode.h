#ifndef HDASHNODE_H
#define HDASHNODE_H
#include "../HBoard_global.h"
#include "hnodebase.h"

class HBOARD_EXPORT HDashNode : public HNodeBase {
 public:
  HDashNode(HNodeBase* ptr);

  virtual QSGNode* get() override;
  virtual QSGNode* build(HBoard*) override;

 protected:
  QList<QPoint> _list;
  QSGNode* _node;
};

#endif  // HDASHNODE_H
