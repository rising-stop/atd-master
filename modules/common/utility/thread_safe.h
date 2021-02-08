#pragma once

#include <condition_variable>
#include <deque>
#include <functional>
#include <mutex>
#include <unordered_map>

namespace Themis {
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

template <typename Key, typename TYPE, typename Hash = std::hash<Key>,
          typename Pred = std::equal_to<Key>,
          typename Alloc = std::allocator<std::pair<const Key, TYPE>>>
class ThreadSafe_HashMap final {
 public:
  bool try_insert(const Key &key, const TYPE &val) {
    unique_writeguard<WfirstRWLock> w_lock(rwlock_);
    return container_.insert({key, val}).second;
  }
  void insert_change(const Key &key, const TYPE &val) {
    unique_writeguard<WfirstRWLock> w_lock(rwlock_);
    if (!container_.insert({key, val}).second) {
      container_[key] = val;
    }
  }
  bool alter_value(const Key &key, const TYPE &val) {
    unique_writeguard<WfirstRWLock> w_lock(rwlock_);
    auto itr = container_.find(key);
    if (itr == container_.cend()) {
      return false;
    }
    container_[key] = val;
    return true;
  }
  bool remove_key(const Key &key) {
    unique_writeguard<WfirstRWLock> w_lock(rwlock_);
    auto itr = container_.find(key);
    if (itr == container_.end()) {
      return false;
    }
    container_.erase(itr);
    return true;
  }

 public:
  bool empty() const {
    unique_readguard<WfirstRWLock> r_lock(rwlock_);
    return container_.empty();
  }
  size_t size() const {
    unique_readguard<WfirstRWLock> r_lock(rwlock_);
    return container_.size();
  }
  std::pair<TYPE, bool> read_value(const Key &key) const {
    unique_readguard<WfirstRWLock> r_lock(rwlock_);
    auto itr = container_.find(key);
    if (itr == container_.end()) {
      return {{}, false};
    }
    return {itr->second, true};
  }
  void for_each_value(std::function<void(const Key &, const TYPE &)> func) {
    unique_readguard<WfirstRWLock> r_lock(rwlock_);
    const auto citr = container_.cbegin();
    while (citr != container_.cend()) {
      func(citr->first, citr->second);
    }
  }

 private:
  mutable WfirstRWLock rwlock_;
  std::unordered_map<Key, TYPE, Hash, Pred, Alloc> container_;
};

template <typename TYPE, typename Container = std::deque<TYPE>>
class ThreadSafe_Deque final {
 private:
  Container container_;
  mutable std::mutex deque_mutex_;
  std::condition_variable wait_cond_;

 public:
  bool empty() const;
  void push_back_with_limits(const TYPE &, int);
  void push_back(const TYPE &);
  void push_front(const TYPE &);
  bool try_pop_back(TYPE &);
  bool try_pop_front(TYPE &);
  std::shared_ptr<TYPE> wait_pop_back();
  std::shared_ptr<TYPE> wait_pop_front();

  bool try_read(Container &);
  void wait_and_read(Container &);

 public:
  ThreadSafe_Deque() = default;
  ThreadSafe_Deque(const ThreadSafe_Deque &);
  ThreadSafe_Deque &operator=(const ThreadSafe_Deque &) = delete;
  ThreadSafe_Deque(ThreadSafe_Deque &&) = delete;
};

}  // namespace utility
}  // namespace Themis

#include "thread_safe.tcc"