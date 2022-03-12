#ifndef HBOARDMANAGER_H
#define HBOARDMANAGER_H
#include "HBoard_global.h"
#include <QHash>
#include <QMutex>
class HBoard;
class HBOARD_EXPORT HBoardManager
{
    HBoardManager();
public:
    static HBoardManager *getInstance();

    void pushBoard(HBoard* board);

    HBoard *getBoard(const QString& name);

    void changeBoardNmae(const QString& old, HBoard* board);
private:
    QHash<QString, HBoard*> _boards;
    QMutex  _mutex;
};

#endif // HBOARDMANAGER_H
