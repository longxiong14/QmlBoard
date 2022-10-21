#include "hthreadpool.h"

#include <QDebug>
#define DEBUG qDebug() << __FUNCTION__ << __LINE__
HThreadPool::HThreadPool(std::size_t size, std::function<void()> t)
    : _flag(0), _task_end(t), _waiter(size) {
  for (std::size_t i = 0; i < size; i++) {
    _works.emplace_back([=]() {
      while (true) {
        std::function<void()> task;
        {
          std::unique_lock<std::mutex> lock(_mutex);
          _condition.wait(lock, [=]() {
            return 2 == _flag || (1 == _flag && !_tasks.empty());
          });
          _waiter--;
          if (2 == _flag) break;
          task = std::move(_tasks.front());
          _tasks.pop();
        }
        task();
        {
          std::unique_lock<std::mutex> lock(_mutex);
          _waiter++;
          if (_waiter == size && _tasks.empty() && _task_end) {
            _task_end();
          }
        }
      }
      {
        std::unique_lock<std::mutex> lock(_mutex);
        //        _waiter++;
        if (_task_end && ++_waiter == size) {
          _task_end();
        }
      }
    });
  }
}

HThreadPool::~HThreadPool() {
  _flag = 2;
  _condition.notify_all();
  for (auto &t : _works) {
    if (t.joinable()) t.join();
  }
}

void HThreadPool::run() {
  std::unique_lock<std::mutex> lock(_mutex);
  _flag = 1;
  _condition.notify_all();
}

void HThreadPool::stop() {
  std::unique_lock<std::mutex> lock(_mutex);
  _flag = 0;
  _condition.notify_all();
}

HSyncThreadPool::HSyncThreadPool(std::size_t size)
    : _pool(new HThreadPool(size,
                            [this]() {
                              DEBUG;
                              _flag = true;
                              _condition.notify_one();
                            })),
      _flag(false) {}

HSyncThreadPool::~HSyncThreadPool() { _pool = nullptr; }

void HSyncThreadPool::run() {
  _flag = false;
  _pool->run();
  std::unique_lock<std::mutex> lock(_mutex);
  _condition.wait(lock, [=]() { return _flag; });
}

void HSyncThreadPool::stop() { _pool->stop(); }
