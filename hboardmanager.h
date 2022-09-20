#ifndef HBOARDMANAGER_H
#define HBOARDMANAGER_H
#include <QHash>
#include <QMutex>

#include "HBoard_global.h"
class HBoard;
class HBOARD_EXPORT HBoardManager {
  HBoardManager();

 public:
  static HBoardManager* getInstance();

  void pushBoard(HBoard* board);

  HBoard* getBoard(const QString& name);

  int removeBoard(const QString& name);

  void changeBoardName(const QString& old, HBoard* board);

 private:
  QHash<QString, HBoard*> _boards;
  QMutex _mutex;
};

#endif  // HBOARDMANAGER_H
