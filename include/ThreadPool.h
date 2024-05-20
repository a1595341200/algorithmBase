/*
 * @Author: yao.xie 1595341200@qq.com
 * @Date: 2023-08-01 09:39:40
 * @LastEditors: yao.xie 1595341200@qq.com
 * @LastEditTime: 2024-05-20 16:49:26
 * @FilePath: /cplusplus/submodule/algorithmBase/include/ThreadPool.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置
 * 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef THREADPOOL_H
#define THREADPOOL_H
#include <atomic>
#include <condition_variable>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <type_traits>

using Task = std::function<void()>;

class ThreadPool {
public:
  explicit ThreadPool(size_t size);
  ~ThreadPool();

  template <typename T, typename... Args>
  auto addTask(T &&t, Args &&...args)
      -> std::enable_if_t<std::is_void<decltype(t(args...))>::value, void> {
    std::unique_lock<std::mutex> _l(mLock);
    mTasks.emplace(std::bind(std::forward<T>(t), std::forward<Args>(args)...));
    _l.unlock();
    mCv.notify_one();
  }

  template <typename T, typename... Args>
  auto addTask(T &&t, Args &&...args) -> std::enable_if_t<
      !std::is_void<decltype(t(args...))>::value,
      std::future<decltype(t(args...))>> {
    std::unique_lock<std::mutex> _l(mLock);
    auto task_ptr =
        std::make_shared<std::packaged_task<decltype(t(args...))()>>(
            std::bind(std::forward<T>(t), std::forward<Args>(args)...));
    std::function<void()> warpper_func = [task_ptr]() { (*task_ptr)(); };
    mTasks.emplace(warpper_func);
    _l.unlock();
    mCv.notify_one();
    return task_ptr->get_future();
  }

  void init();

private:
  std::vector<std::thread> mThreads;
  std::queue<Task> mTasks;
  std::mutex mLock;
  std::condition_variable mCv;
  std::atomic_bool isStop{false};
  size_t mSize;
};

#endif /* THREADPOOL_H */
