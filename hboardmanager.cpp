#include "hboardmanager.h"
#include "hboard.h"
#include <QMutexLocker>
HBoardManager::HBoardManager()
{

}

HBoardManager* HBoardManager::getInstance()
{
    static HBoardManager manager;
    return &manager;
}

void HBoardManager::pushBoard(HBoard *board)
{
    QMutexLocker lock(&_mutex);
    if(!_boards.contains(board->name())){
        _boards.insert(board->name(), board);
    }
}

HBoard* HBoardManager::getBoard(const QString &name)
{
    QMutexLocker lock(&_mutex);
    HBoard * ptr = nullptr;
    if(_boards.contains(name)){
        ptr = _boards[name];
    }
    return ptr;
}

void HBoardManager::changeBoardNmae(const QString &old, HBoard *board)
{
    QMutexLocker lock(&_mutex);
    if(_boards.contains(old)){
        _boards.remove(old);
    }
    _boards.insert(board->name(), board);
}
