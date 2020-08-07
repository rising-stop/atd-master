#include "thread_safe.h"

namespace atd {
namespace utility {

template <typename TYPE, typename Container>
bool ThreadSafe_Deque<TYPE, Container>::empty() const {
  std::unique_lock<std::mutex> lock(deque_mutex_);
  return container_.empty();
}

template <typename TYPE, typename Container>
void ThreadSafe_Deque<TYPE, Container>::push_back_with_limits(
    const TYPE &element, int limit) {
  {
    std::lock_guard<std::mutex> lock(deque_mutex_);
    container_.push_back(element);
    if (container_.size() > limit) {
      container_.pop_front();
    }
  }
  wait_cond_.notify_one();
}

template <typename TYPE, typename Container>
void ThreadSafe_Deque<TYPE, Container>::push_back(const TYPE &element) {
  {
    std::lock_guard<std::mutex> lock(deque_mutex_);
    container_.push_back(element);
  }
  wait_cond_.notify_one();
}

template <typename TYPE, typename Container>
void ThreadSafe_Deque<TYPE, Container>::push_front(const TYPE &element) {
  {
    std::lock_guard<std::mutex> lock(deque_mutex_);
    container_.push_back(element);
  }
  wait_cond_.notify_one();
}

template <typename TYPE, typename Container>
bool ThreadSafe_Deque<TYPE, Container>::try_pop_back(TYPE &item) {
  std::lock_guard<std::mutex> lock(deque_mutex_);
  if (container_.empty()) {
    return false;
  }
  item = container_.back();
  container_.pop_back();
  return true;
}

template <typename TYPE, typename Container>
bool ThreadSafe_Deque<TYPE, Container>::try_pop_front(TYPE &item) {
  std::lock_guard<std::mutex> lock(deque_mutex_);
  if (container_.empty()) {
    return false;
  }
  item = container_.front();
  container_.pop_front();
  return true;
}

template <typename TYPE, typename Container>
std::shared_ptr<TYPE> ThreadSafe_Deque<TYPE, Container>::wait_pop_back() {
  std::unique_lock<std::mutex> lock(deque_mutex_);
  wait_cond_.wait(lock, [&]() -> bool { return !container_.empty(); });
  auto res = std::make_shared<TYPE>(container_.back());
  container_.pop_back();
  return res;
}

template <typename TYPE, typename Container>
std::shared_ptr<TYPE> ThreadSafe_Deque<TYPE, Container>::wait_pop_front() {
  std::unique_lock<std::mutex> lock(deque_mutex_);
  wait_cond_.wait(lock, [&]() -> bool { return !container_.empty(); });
  auto res = std::make_shared<TYPE>(container_.front());
  container_.pop_front();
  return res;
}

template <typename TYPE, typename Container>
ThreadSafe_Deque<TYPE, Container>::ThreadSafe_Deque(
    const ThreadSafe_Deque &deque) {
  std::lock_guard<std::mutex> lock(deque_mutex_);
  container_ = deque.container_;
}

}  // namespace utility
}  // namespace atd