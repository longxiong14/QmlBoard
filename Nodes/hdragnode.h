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
  static HDragNode* buildNode(const QPointF& center, double size,
                              const QUuid& parent);

  void moveTo(const QPointF& center);

  bool pointIn(const QPointF& point, double scale);

  void setPointIndex(int index);

  int getPointIndex();

  void setFollowIndex(int index);

  int getFollowIndex();

  void setCurSor(const QCursor& cursor);

  QCursor getCursor();

  void setParent(const QUuid& p);

  QUuid getParent();

  void setCenter(const QPointF& c);

  QPointF getCenter();

 protected:
  QCursor _cursor;
  QUuid _parent;
  double _size;
  QPointF _center;
};

#endif  // HDRAGNODE_H
