#include <sys/sem.h>

#include <type_traits>

#ifndef __SEMAPHORE_H__
#define __SEMAPHORE_H__

#define SEM_SEED 1000

typedef union {
  int val;
  semid_ds *buf;
  unsigned short *array;
} semun;

namespace atd {
namespace common {
namespace utility {

class SemMutex {
 public:
  ::key_t get_Key() const;
  virtual void lock() = 0;
  virtual void unlock() = 0;

 protected:
  virtual void init() = 0;
  ::key_t key_;

 public:
  SemMutex() = delete;
  explicit SemMutex(::key_t);
  virtual ~SemMutex() = default;
};

class Dual_SemMutex : public SemMutex {
 public:
  virtual void lock() override;
  virtual void unlock() override;

 private:
  virtual void init() override;
  int semid_;
  bool is_locked_ = false;

 public:
  Dual_SemMutex() = delete;
  explicit Dual_SemMutex(::key_t);
  ~Dual_SemMutex();
};

class Shared_SemMutex : public SemMutex {
 public:
  virtual void lock() override;
  virtual void unlock() override;
  void shared_lock();
  void shared_unlock();

 private:
  virtual void init() override;
  int semid_;
  bool is_locked_ = false;
  bool is_shared_locked_ = false;

 public:
  Shared_SemMutex() = delete;
  explicit Shared_SemMutex(::key_t);
  ~Shared_SemMutex();
};

/**
 * Semlock RAII container
 *   lock_guard<LOCK_TYPE>
 *   shared_lock<LOCK_TYPE>
 * */

template <typename LOCK_TYPE>
class lock_guard {
  typedef typename std::enable_if<std::is_base_of<SemMutex, LOCK_TYPE>::value,
                                  LOCK_TYPE>::type LOCK;

 private:
  LOCK &lock_;

 public:
  lock_guard() = delete;
  lock_guard(LOCK &lock) : lock_(lock) { lock_.lock(); }
  ~lock_guard() { lock_.unlock(); }
};

template <typename LOCK_TYPE>
class shared_lock {
  typedef typename std::enable_if<
      std::is_base_of<Shared_SemMutex, LOCK_TYPE>::value, LOCK_TYPE>::type
      SHARED_LOCK;

 private:
  SHARED_LOCK &lock_;

 public:
  shared_lock() = delete;
  shared_lock(SHARED_LOCK &lock) : lock_(lock) { lock_.shared_lock(); }
  ~shared_lock() { lock_.shared_unlock(); }
};

}  // namespace utility
}  // namespace common
}  // namespace atd

#endif