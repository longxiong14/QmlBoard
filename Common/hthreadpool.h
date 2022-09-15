#ifndef HTHREADPOOL_H
#define HTHREADPOOL_H
#include <condition_variable>
#include <functional>
#include <iostream>
#include <list>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>

#include "../HBoard_global.h"
class HBOARD_EXPORT HThreadPool {
 public:
  HThreadPool(std::size_t size, std::function<void(void)> t = nullptr);
  ~HThreadPool();

  template <class Func, class... ARGS>
  void push(Func &&f, ARGS &&... args);

  void run();

  void stop();

 protected:
  std::list<std::thread> _works;
  std::queue<std::function<void()>> _tasks;
  std::condition_variable _condition;
  std::mutex _mutex;
  int _flag;
  std::function<void()> _task_end;
  std::atomic_size_t _waiter;
};

class HBOARD_EXPORT HSyncThreadPool {
 public:
  HSyncThreadPool(std::size_t size);
  ~HSyncThreadPool();

  template <class Func, class... ARGS>
  void push(Func &&f, ARGS &&... args);

  void run();

  void stop();

 protected:
  std::condition_variable _condition;
  std::mutex _mutex;
  std::shared_ptr<HThreadPool> _pool;
  bool _flag;
};

template <class Func, class... ARGS>
void HThreadPool::push(Func &&f, ARGS &&... args) {
  auto func = std::bind(std::forward<Func>(f), std::forward<ARGS>(args)...);
  _tasks.push(func);
  _condition.notify_one();
}

template <class Func, class... ARGS>
void HSyncThreadPool::push(Func &&f, ARGS &&... args) {
  _pool->push(std::forward<Func>(f), std::forward<ARGS>(args)...);
}

#endif  // HTHREADPOOL_H
