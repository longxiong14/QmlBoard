#ifndef HBOARDACTIONBASE_H
#define HBOARDACTIONBASE_H
#include <QList>
#include <QPointF>
#include <QString>
#include <QUuid>
#include <memory>

#include "../HBoard_global.h"
class HNodeBase;
class HBOARD_EXPORT HBoardActionBase {
 public:
  HBoardActionBase();
  HBoardActionBase(const HBoardActionBase &other) = default;
  HBoardActionBase &operator=(const HBoardActionBase &other) = default;
  virtual ~HBoardActionBase();

  virtual int excute() = 0;
  virtual int undo() = 0;
};

class HBOARD_EXPORT HPushNodeAction : public HBoardActionBase {
 public:
  HPushNodeAction(const QString &name, std::shared_ptr<HNodeBase> node);

  virtual int excute() override;
  virtual int undo() override;

 protected:
  QString _board_name;
  std::shared_ptr<HNodeBase> _node;
};

class HBOARD_EXPORT HRemoveNodeAction : public HBoardActionBase {
 public:
  HRemoveNodeAction(const QString &name, const QSet<QUuid> &id);
  virtual int excute() override;
  virtual int undo() override;

 protected:
  QString _board_name;
  QHash<QUuid, std::shared_ptr<HNodeBase>> _nodes;
};

class HBOARD_EXPORT HUpdateNodePointsAction : public HBoardActionBase {
 public:
  HUpdateNodePointsAction(const QString &name, const QUuid &node_id,
                          const QList<QPointF> &list);

  virtual int excute() override;
  virtual int undo() override;

 protected:
  int update();

 protected:
  QString _board_name;
  QUuid _id;
  QList<QPointF> _list;
};

class HBOARD_EXPORT HMoveNodeAction : public HBoardActionBase {
 public:
  HMoveNodeAction(const QString &board, const QSet<QUuid> &nodes,
                  const QPointF &dlt, bool flag = false);
  virtual int excute() override;
  virtual int undo() override;

 protected:
  QString _board_name;
  QSet<QUuid> _nodes;
  QPointF _dlt;
  bool _flag;
};

#endif  // HBOARDACTIONBASE_H
