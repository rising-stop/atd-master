/* Created by Liangle */

#pragma once

#include <sys/shm.h>

#include <unordered_map>

#include "modules/common/utility/utility.h"

namespace atd {
namespace common {
namespace utility {

class ShmDispatcher {
 public:
  /**
   * @brief static member used for cleaning all sems
   * @note all sems will be cleared so it just supposed to run only once
   */
  void release_all_shm();

  /**
   * @brief cleaning current sem
   * @note throw no exception, clear sem forcely
   */
  void release_shm(::key_t);

  /**
   * @brief sem register funtion
   * using key to generate unique sem firstly, if successed, initialize sem
   * values and return semid. If failed, try to get exising sem used same key.
   * If all operation above failed, return -1
   * @param ::key_t shm key_
   *        int shm size
   * @return int semid
   * @note register throw no exceptions, when the key is occupied, the sem will
   * be reset forcely
   */
  int register_shm(::key_t, int);

  std::pair<int, int> get_ShmInfo(::key_t) const;

 private:
  std::unordered_map<::key_t, std::pair<int, int>>
      registered_shms_; /* static member for restore all semid and its size*/

 public:
  static ShmDispatcher* instance() {
    std::lock_guard<std::mutex> lk(instance_mutex_);
    std::call_once(flag_init_, &init);
    // if (!instance_) {
    //   instance_ = new ShmDispatcher();
    // }
    return instance_;
  }

 private:
  static void init() { instance_ = new ShmDispatcher(); }
  static ShmDispatcher* instance_;
  static std::once_flag flag_init_;
  static std::mutex instance_mutex_;
  ShmDispatcher() = default;
  ~ShmDispatcher() = default;
  ShmDispatcher(const ShmDispatcher&) = delete;
  ShmDispatcher(ShmDispatcher&&) = delete;
};
#define GET_SHMDISPATCHER atd::common::utility::ShmDispatcher::instance()

class SharedMemory {
 public:
  int get_ShmID() const;
  int get_ShmSize() const;

  void* get_AssignedAddr() const;

  void send_Msg(const std::string&);
  void read_Msg(std::string&, size_t);

  template <typename TYPE>
  void send_Msg(const TYPE&, size_t);
  template <typename TYPE>
  void read_Msg(TYPE&, size_t);

 private:
  void mount_Shm();
  void unmount_Shm();

  int shmid_ = -1;   /* registered shm's id */
  int shmsize_ = -1; /* registered shm's size */
  void* addr_;       /* pointer that shared memory mapped */

 public:
  SharedMemory() = delete;
  SharedMemory(int, int);
  SharedMemory(std::pair<int, int>);
  ~SharedMemory() = default;
};

// ShmSyncLink
// ShmAsyncLink

}  // namespace utility
}  // namespace common
}  // namespace atd

#include "shared_memory.tcc"