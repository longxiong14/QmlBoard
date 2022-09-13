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

  void run(bool f = true);

  void stop(bool f = true);

 protected:
  std::list<std::thread> _works;
  std::queue<std::function<void()>> _tasks;
  std::condition_variable _condition;
  std::mutex _mutex;
  bool _running;
  bool _stop;
  std::function<void()> _task_end;
};

template <class Func, class... ARGS>
void HThreadPool::push(Func &&f, ARGS &&... args) {
  auto func = std::bind(std::forward<Func>(f), std::forward<ARGS>(args)...);
  _tasks.push(func);
  if (_running) _condition.notify_one();
}
#endif  // HTHREADPOOL_H
