#ifndef HDRAGNODE_H
#define HDRAGNODE_H

#include <QSGNode>

#include "../HBoard_global.h"
class HBOARD_EXPORT HDragNode : public QSGNode {
 public:
  HDragNode();

  void setFlag(int f);

 protected:
  int _flag;
};

#endif  // HDRAGNODE_H
