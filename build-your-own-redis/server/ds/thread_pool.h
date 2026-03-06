#pragma once

#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <pthread.h>
#include <queue>
#include <stddef.h>
#include <thread>
#include <type_traits>
#include <vector>

using TaskFunction = std::function<void()>;

class TheadPool {
private:
  std::vector<std::thread> workers;        // 工作线程
  std::queue<TaskFunction> tasks;          // 任务队列
  std::mutex queue_mutex;                  // 任务锁
  std::condition_variable queue_condition; // 条件锁
  bool stop = false;                       // 停止标志

public:
  TheadPool() : TheadPool(0) {};
  TheadPool(size_t num_threads) {
    unsigned int core_count = std::thread::hardware_concurrency();
    core_count = core_count == 0 ? 2 : core_count;
    num_threads = num_threads <= 0
                      ? core_count
                      : std::min(core_count, (unsigned int)num_threads);

    for (size_t i = 0; i < num_threads; ++i) {
      this->workers.emplace_back([this] {
        for (;;) {
          TaskFunction task;
          {
            auto guard = std::unique_lock(this->queue_mutex);
            this->queue_condition.wait(
                guard, [this] { return this->stop || !this->tasks.empty(); });
            // 保证所有任务都完成, 才进行退出
            if (this->stop && this->tasks.empty()) {
              return;
            }
            task = std::move(this->tasks.front());
            this->tasks.pop();
          }
          task();
        }
      });
      this->workers.shrink_to_fit();
    }
  }
  ~TheadPool() {
    {
      std::lock_guard<std::mutex> lock(this->queue_mutex);
      stop = true;
    }
    this->queue_condition.notify_all();
    for (std::thread &worker : workers) {
      worker.join();
    }
  }
  TheadPool(const TheadPool &) = delete;
  TheadPool &operator=(const TheadPool &) = delete;
  TheadPool(TheadPool &&) = delete;
  TheadPool &operator=(TheadPool &&) = delete;

  template <typename _Func, typename... _BoundArgs>
  auto enqueue(_Func &&f, _BoundArgs &&...args)
      -> std::future<typename std::result_of<_Func(_BoundArgs...)>::type> {
    // 推导返回类型
    using return_type = typename std::result_of<_Func(_BoundArgs...)>::type;

    auto task = std::make_shared<std::packaged_task<return_type()>>(
        std::bind(std::forward<_Func>(f), std::forward<_BoundArgs>(args)...));

    std::future<return_type> res = task->get_future();
    {
      std::lock_guard<std::mutex> lock(this->queue_mutex);
      if (this->stop) {
        // don't allow enqueueing after stopping the pool
        throw std::runtime_error("enqueue on stopped ThreadPool");
      }
      this->tasks.emplace([task]() { (*task)(); });
    }
    this->queue_condition.notify_one();
    return res;
  }
};
