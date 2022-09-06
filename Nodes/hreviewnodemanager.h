#ifndef HREVIEWNODEMANAGER_H
#define HREVIEWNODEMANAGER_H

#include <QImage>
#include <QSet>
#include <QUuid>

#include "../HBoard_global.h"
#include "hnodebase.h"
class HBOARD_EXPORT HReviewNodeManager {
 public:
  HReviewNodeManager(const QString& name);
  HReviewNodeManager(const HReviewNodeManager& other);
  HReviewNodeManager& operator=(const HReviewNodeManager& other);
  virtual ~HReviewNodeManager();

  int pushNode(const QString& key, std::shared_ptr<HNodeBase> node);
  int pushNode(const QString& key, const QImage& image,
               const QPointF& start_point);

  int removeNode(const QString& key);

  void clearButUnremoveNodes();

  bool hasReviewNode();

  QHash<QString, QUuid> nodes();

 private:
  QHash<QString, QUuid> _nodes;
  QString _board_name;
};

#endif  // HREVIEWNODEMANAGER_H
