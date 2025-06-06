#include <atomic>
#include <condition_variable>
#include <cstdint>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <utility>
#include <vector>

struct ThreadPool {
private:
  std::vector<std::thread> threads;
  std::queue<std::function<void()>> jobs;
  std::condition_variable cv;
  std::mutex queue_mutex;
  std::atomic<bool> stop;

public:
  ThreadPool(uint8_t size) : stop(false) {
    for (int i = 0; i < size; i++)
      threads.emplace_back([this] {
        while (true) {
          std::unique_lock<std::mutex> lock(queue_mutex);
          cv.wait(lock, [this] { return stop || !jobs.empty(); });
          if (stop && jobs.empty())
            return;
          auto job = std::move(jobs.front());
          jobs.pop();
          lock.unlock();
          job();
        }
      });
  }

  template <typename F> void push(F &&job) {
    std::unique_lock<std::mutex> lock(queue_mutex);
    jobs.emplace(std::forward<F>(job));
    lock.unlock();
    cv.notify_one();
  }
  ~ThreadPool() {
    std::unique_lock<std::mutex> lock(queue_mutex);
    stop = true;
    lock.unlock();
    cv.notify_all();
    for (auto &thread : threads) {
      if (thread.joinable())
        thread.join();
    }
  }
};
