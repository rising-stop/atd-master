/* Created by Liangle */

#pragma once

#include <sys/sem.h>

#include <type_traits>
#include <unordered_map>

#include "modules/common/utility/utility.h"

#define MAX_PROCESS_NUM 1000

/**
 * @union semun
 * @brief union semun
 * union struct for sem, used for setting value of single sem
 * general style used in sem for linux
 */
typedef union {
  int val;               /* setting sem value */
  semid_ds *buf;         /* innner struct */
  unsigned short *array; /**/
} semun;

namespace atd {
namespace common {
namespace utility {

/**
 * @class SemDispatcher
 * @brief static member used for cleaning all sems
 * @note all sems will be cleared so it just supposed to run only once
 */
class SemDispatcher {
 public:
  /**
   * @brief static member used for cleaning all sems
   * @note all sems will be cleared so it just supposed to run only once
   */
  void release_all_sem();

  /**
   * @brief cleaning current sem
   * @note throw no exception, clear sem forcely
   */
  void release_sem(::key_t);

  /**
   * @brief sem register funtion
   * using key to generate unique sem firstly, if successed, initialize sem
   * values and return semid. If failed, try to get exising sem used same key.
   * If all operation above failed, return -1
   * @param int number of sems in semid_
   * @return int semid
   * @note register throw no exceptions, when the key is occupied, the sem will
   * be reset forcely
   */
  int register_sem(::key_t, int);

  std::pair<int, int> get_SemInfo(::key_t) const;

 private:
  std::unordered_map<::key_t, std::pair<int, int>>
      registered_sems_; /* static member for restore all semid and its signal
                           number*/
  SINGLETON(SemDispatcher)
};
#define GET_SEMDISPATCHER atd::common::utility::SemDispatcher::instance()

/**
 * @brief common base class SemMutex
 *   1. provide lock and unlock virtual method
 *   2. provide registration pool of all sem locks
 *   3. provide common register method Register_Sem
 *   4. provide common unregister method Release_Sem
 */
class SemMutex {
 public:
  int get_SemID() const;

  /**
   * @brief general lock & unlock operation for Class SemMutex
   */
  virtual void lock() = 0;
  virtual void unlock() = 0;

 protected:
  /**
   * @brief virtual method init for Class SemMutex
   * usually setting sem value as 1
   */
  virtual void init() = 0;
  int sem_id_ = -1; /* sem id generate by semget function */

 public:
  SemMutex() = delete;
  /**
   * @brief default constructor forbidened, using key value to initialize sem
   * mutex
   * @param int key, unique key value
   */
  explicit SemMutex(int);
  /**
   * @brief virtual deconstructor, clear all registered semid_
   */
  virtual ~SemMutex();
};

/**
 * @brief DualSemMutex, provide dual mutex between different process
 *   1. provide dual mutex lock and unlock virtual method
 */
class DualSemMutex : public SemMutex {
 public:
  /**
   * @brief overloaded lock & unlock operation for Dual_Mutex
   */
  virtual void lock() override;
  virtual void unlock() override;

 private:
  /**
   * @brief setting sem value as 1
   */
  virtual void init() override;
  bool is_locked_ = false; /* mutex flag for lock and unlock operatiion */

 public:
  DualSemMutex() = delete;
  explicit DualSemMutex(int);
  virtual ~DualSemMutex();
};

/**
 * @brief SharedSemMutex, read lock will not be blocked by other read locks
 *   1. provide shared mutex lock and unlock virtual method
 *   2. provide shared mutex shared_lock and shared_unlock method
 */
class SharedSemMutex : public SemMutex {
 public:
  virtual void lock() override;
  virtual void unlock() override;
  /**
   * @brief shared_lock, the process will not be blocked by other shared_lock,
   * it blocks the process uses lock()
   */
  void shared_lock();
  /**
   * @brief release the shared_lock
   */
  void shared_unlock();

 private:
  virtual void init() override;
  bool is_locked_ = false; /* mutex flag for lock and unlock */
  bool is_shared_locked_ =
      false; /* mutex flag for shared_lock and shared_unlock */

 public:
  SharedSemMutex() = delete;
  explicit SharedSemMutex(int);
  ~SharedSemMutex();
};

/**
 * @brief RAII class for SemMutex, automaticly release lock when out of scope
 */
template <typename LOCK_TYPE>
class lock_guard {
  typedef typename std::enable_if<std::is_base_of<SemMutex, LOCK_TYPE>::value,
                                  LOCK_TYPE>::type LOCK;

 public:
  void unlock() { lock_.unlock(); }

 private:
  bool is_locked_ = false;
  LOCK &lock_;

 public:
  lock_guard() = delete;
  lock_guard(LOCK &lock) : lock_(lock) {
    lock_.lock();
    is_locked_ = true;
  }
  ~lock_guard() {
    if (is_locked_) {
      lock_.unlock();
    }
  }
};

/**
 * @brief RAII class for shared_mutex, automaticly release lock when out of
 * scope
 */
template <typename LOCK_TYPE>
class shared_lock {
  typedef
      typename std::enable_if<std::is_base_of<SharedSemMutex, LOCK_TYPE>::value,
                              LOCK_TYPE>::type SHARED_LOCK;

 public:
  void unlock() { lock_.shared_unlock(); }

 private:
  bool is_locked_ = false;
  SHARED_LOCK &lock_;

 public:
  shared_lock() = delete;
  shared_lock(SHARED_LOCK &lock) : lock_(lock) {
    lock_.shared_lock();
    is_locked_ = true;
  }
  ~shared_lock() {
    if (is_locked_) {
      lock_.shared_unlock();
    }
  }
};

}  // namespace utility
}  // namespace common
}  // namespace atd