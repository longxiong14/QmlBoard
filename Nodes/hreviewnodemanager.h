#ifndef HREVIEWNODEMANAGER_H
#define HREVIEWNODEMANAGER_H

#include <QSet>
#include <QUuid>
#include <opencv2/core.hpp>

#include "../HBoard_global.h"
#include "hnodebase.h"
class HBOARD_EXPORT HReviewNodeManager {
 public:
  HReviewNodeManager(const QString& name);
  HReviewNodeManager(const HReviewNodeManager& other);
  HReviewNodeManager& operator=(const HReviewNodeManager& other);
  virtual ~HReviewNodeManager();

  int pushNode(const QString& key, std::shared_ptr<HNodeBase> node);
  int pushNode(const QString& key, const cv::Mat& mat,
               const QPointF& start_point);

  int removeNode(const QString& key);

  bool hasReviewNode();

  QHash<QString, QUuid> nodes();

 private:
  QHash<QString, QUuid> _nodes;
  QString _board_name;
};

#endif  // HREVIEWNODEMANAGER_H
