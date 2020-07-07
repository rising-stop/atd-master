#pragma once

#include <condition_variable>
#include <deque>
#include <mutex>

namespace atd {
namespace utility {

class WfirstRWLock {
 public:
  void lock_read() {
    std::unique_lock<std::mutex> ulk(counter_mutex_);
    cond_r_.wait(ulk, [=]() -> bool { return write_cnt_ == 0; });
    ++read_cnt_;
  }
  void lock_write() {
    std::unique_lock<std::mutex> ulk(counter_mutex_);
    ++write_cnt_;
    cond_w_.wait(ulk,
                 [=]() -> bool { return read_cnt_ == 0 && !inwriteflag_; });
    inwriteflag_ = true;
  }
  void release_read() {
    std::unique_lock<std::mutex> ulk(counter_mutex_);
    if (--read_cnt_ == 0 && write_cnt_ > 0) {
      cond_w_.notify_one();
    }
  }
  void release_write() {
    std::unique_lock<std::mutex> ulk(counter_mutex_);
    if (--write_cnt_ == 0) {
      cond_r_.notify_all();
    } else {
      cond_w_.notify_one();
    }
    inwriteflag_ = false;
  }

 private:
  volatile size_t read_cnt_{0};
  volatile size_t write_cnt_{0};
  volatile bool inwriteflag_{false};
  std::mutex counter_mutex_;
  std::condition_variable cond_w_;
  std::condition_variable cond_r_;

 public:
  WfirstRWLock() = default;
  ~WfirstRWLock() = default;
};

template <typename _RWLockable>
class unique_writeguard {
 public:
  explicit unique_writeguard(_RWLockable &rw_lockable)
      : rw_lockable_(rw_lockable) {
    rw_lockable_.lock_write();
  }
  ~unique_writeguard() { rw_lockable_.release_write(); }

 private:
  unique_writeguard() = delete;
  unique_writeguard(const unique_writeguard &) = delete;
  unique_writeguard &operator=(const unique_writeguard &) = delete;

 private:
  _RWLockable &rw_lockable_;
};
template <typename _RWLockable>
class unique_readguard {
 public:
  explicit unique_readguard(_RWLockable &rw_lockable)
      : rw_lockable_(rw_lockable) {
    rw_lockable_.lock_read();
  }
  ~unique_readguard() { rw_lockable_.release_read(); }

 private:
  unique_readguard() = delete;
  unique_readguard(const unique_readguard &) = delete;
  unique_readguard &operator=(const unique_readguard &) = delete;

 private:
  _RWLockable &rw_lockable_;
};

template <typename TYPE, typename Container = std::deque<TYPE>>
class ThreadSafe_Deque {
 private:
  Container container_;
  mutable std::mutex deque_mutex_;
  std::condition_variable wait_cond_;

 public:
  bool empty() const;
  void push_back_with_limits(const TYPE &, int);
  void push_back(const TYPE &);
  void push_front(const TYPE &);
  bool try_pop_back(TYPE&);
  bool try_pop_front(TYPE&);
  std::shared_ptr<TYPE> wait_pop_back();
  std::shared_ptr<TYPE> wait_pop_front();

 public:
  ThreadSafe_Deque() = default;
  ThreadSafe_Deque(const ThreadSafe_Deque &);
  ThreadSafe_Deque &operator=(const ThreadSafe_Deque &) = delete;
  ThreadSafe_Deque(ThreadSafe_Deque &&) = delete;
};

}  // namespace utility
}  // namespace atd

#include "thread_safe.tcc"