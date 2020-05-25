/* Created by Liangle */

#pragma once

#include <sys/shm.h>

#include <unordered_map>

#include "modules/common/utility/utility.h"

#define SHM_SEED 1001

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
  SINGLETON(ShmDispatcher)
};

class SharedMemory {
 public:
  int get_ShmID() const;
  int get_ShmSize() const;

  void* get_AssignedAddr() const;

  void send_Msg(const std::string&);
  void read_Msg(std::string&, int);

 private:
  void mount_Shm();
  void unmount_Shm();

  int shmid_ = -1;
  int shmsize_ = -1;
  void* addr_;

 public:
  SharedMemory() = delete;
  SharedMemory(int, int);
  ~SharedMemory() = default;
};

// ShmSyncLink
// ShmAsyncLink

}  // namespace utility
}  // namespace common
}  // namespace atd