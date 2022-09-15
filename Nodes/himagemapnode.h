#ifndef HIMAGEMAPNODE_H
#define HIMAGEMAPNODE_H

#include "../HBoard_global.h"
#include "QImage"
#include "hnodebase.h"
class HBOARD_EXPORT HImageMapNode : public HNodeBase {
 public:
  HImageMapNode();
  //    HImageMapNode(const QString &path, const QPointF &start_point =
  //    QPointF()); HImageMapNode(const QImage &mat, const QPointF &start_point
  //    = QPointF()); virtual ~HImageMapNode() override;
};

#endif  // HIMAGEMAPNODE_H
