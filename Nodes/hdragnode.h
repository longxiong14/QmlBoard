#ifndef HDRAGNODE_H
#define HDRAGNODE_H

#include <QCursor>
#include <QJsonObject>
#include <QSGNode>
#include <QUuid>

#include "../HBoard_global.h"
class HBOARD_EXPORT HDragNode : public QSGGeometryNode, public QJsonObject {
  HDragNode();

 public:
  static HDragNode* buildNode(const QPointF& center, float size,
                              const QUuid& parent);

  void moveTo(const QPointF& center);

  bool pointIn(const QPointF& point);

  void setPointIndex(int index);

  int getPointIndex();

  void setFollowIndex(int index);

  int getFollowIndex();

  void setCurSor(const QCursor& cursor);

  QCursor getCursor();

  void setParent(const QUuid& p);

  QUuid getParent();

 protected:
  QCursor _cursor;
  QUuid _parent;
  float _size;
};

#endif  // HDRAGNODE_H
