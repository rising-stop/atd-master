/* Created by Liangle */

#pragma once

#include <sys/shm.h>

#include <unordered_map>

#define SHM_SEED 1001
#define SINGLE_SHM_PIECE 2048 * 2048

typedef struct {
  int end_flag_;  //用来标记进程间的内存共享是否结束: 0, 未结束； 1， 结束
  int embeded_space_;
  char shm_space_[SINGLE_SHM_PIECE];  //共享内存的空间
} shm_piece;

namespace atd {
namespace common {
namespace utility {

class SharedMemory {
 public:
  ::key_t get_key() const;
  int get_shmID() const;
  int get_shm_size() const;

  void* get_assigned_addr() const;

 private:
  ::key_t key_;
  int shmid_ = -1;
  int shmsize_ = -1;
  void* addr_;

 public:
  static void release_all_shm();

  void release_shm();

 private:
  std::pair<int, int> register_shm(int);

  void release_shm();

  static std::unordered_map<::key_t, std::pair<int, int>> registered_shm_;

 public:
  SharedMemory() = delete;
  SharedMemory(::key_t);
  ~SharedMemory();
};

// ShmSyncLink
// ShmAsyncLink

}  // namespace utility
}  // namespace common
}  // namespace atd