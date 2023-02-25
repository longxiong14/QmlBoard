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
  HPushNodeAction(const HPushNodeAction &other) = default;
  HPushNodeAction &operator=(const HPushNodeAction &other) = default;
  virtual ~HPushNodeAction();

  virtual int excute();
  virtual int undo();

 protected:
  QString _board_name;
  std::shared_ptr<HNodeBase> _node;
};

class HBOARD_EXPORT HRemoveNodeAction : public HBoardActionBase {
 public:
  HRemoveNodeAction(const QString &name, const QUuid &id);
  HRemoveNodeAction(const HRemoveNodeAction &other) = default;
  HRemoveNodeAction &operator=(const HRemoveNodeAction &other) = default;
  virtual ~HRemoveNodeAction();

  virtual int excute();
  virtual int undo();

 protected:
  QString _board_name;
  QUuid _id;
  std::shared_ptr<HNodeBase> _node;
};

class HBOARD_EXPORT HHandeUpdatePointsAction : public HBoardActionBase {
 public:
  HHandeUpdatePointsAction(const QString &name, const QUuid &id, int size,
                           const QList<QPointF> &points);
  HHandeUpdatePointsAction(const HHandeUpdatePointsAction &other) = default;
  HHandeUpdatePointsAction &operator=(const HHandeUpdatePointsAction &other) =
      default;
  virtual ~HHandeUpdatePointsAction();

  virtual int excute();
  virtual int undo();

 protected:
  QString _board_name;
  QUuid _id;
  int _size;
  QList<QPointF> _points;
};

#endif  // HBOARDACTIONBASE_H
