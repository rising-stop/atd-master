#ifndef _SHARED_MEMORY_H_
#define _SHARED_MEMORY_H_

#include <sys/shm.h>

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
 private:
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

#endif