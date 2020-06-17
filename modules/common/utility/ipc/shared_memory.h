/* Created by Liangle */

#pragma once

#include <sys/shm.h>

#include <unordered_map>

#include "modules/common/utility/utility.h"

namespace atd {
namespace utility {

class ShmDispatcher : public Singleton {
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

  SINGLETON_DERIVED(ShmDispatcher)
};

class SharedMemory {
 public:
  static std::pair<int, size_t> try_get_ShmID(::key_t, size_t);

  int get_ShmID() const;
  size_t get_ShmSize() const;
  const void* get_AssignedAddr() const;

  void write_Msg(const std::string&);
  void read_Msg(std::string&, size_t);

  template <typename TYPE>
  void write_Msg(const TYPE&, size_t);
  template <typename TYPE>
  void read_Msg(TYPE&, size_t);

 private:
  void mount_Shm();
  void unmount_Shm();

  int shmid_ = -1;      /* registered shm's id */
  size_t shmsize_ = -1; /* registered shm's size */
  void* addr_;          /* pointer that shared memory mapped */

 public:
  SharedMemory() = delete;
  SharedMemory(int, size_t);
  SharedMemory(std::pair<int, size_t>);
  ~SharedMemory() = default;
  SharedMemory(const SharedMemory&) = delete;
  SharedMemory& operator=(const SharedMemory&) = delete;
};

class ShmFactory : public Singleton {
  friend class atd::utility::Singleton;

 public:
  std::unique_ptr<SharedMemory> get_Shm(int, size_t);

 protected:
  ShmFactory();
  virtual ~ShmFactory() = default;

 private:
  Factory<std::string, SharedMemory, SharedMemory*(*)(int, size_t)> shm_factory_;
};

// ShmSyncLink
// ShmAsyncLink

}  // namespace utility
}  // namespace atd

#include "shared_memory.tcc"