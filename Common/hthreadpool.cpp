#include "hthreadpool.h"
HThreadPool::HThreadPool(std::size_t size, std::function<void()> t)
    : _running(false), _stop(false), _task_end(t) {
  for (std::size_t i = 0; i < size; i++) {
    _works.emplace_back([=]() {
      while (true) {
        std::function<void()> task;
        std::size_t size = 0;
        {
          std::unique_lock<std::mutex> lock(_mutex);
          _condition.wait(lock, [=]() { return _running || !_tasks.empty(); });
          if (_stop) break;
          if (_tasks.empty()) continue;
          task = std::move(_tasks.front());
          _tasks.pop();
          size = _tasks.size();
        }
        task();
        if (!size && _task_end) {
          _task_end();
        }
      }
    });
  }
}

HThreadPool::~HThreadPool() {
  stop(true);
  for (auto &t : _works) {
    if (t.joinable()) t.join();
  }
}

void HThreadPool::run(bool f) {
  std::unique_lock<std::mutex> lock(_mutex);
  _running = f;
  _condition.notify_all();
}

void HThreadPool::stop(bool f) {
  std::unique_lock<std::mutex> lock(_mutex);
  _running = true;
  _stop = f;
  _condition.notify_all();
}
