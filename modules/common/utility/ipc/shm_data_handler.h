#pragma once

#include "semaphore_mutex.h"
#include "shared_memory.h"

namespace atd {
namespace utility {

class ShmDataHandler {
 public:
  void set_TargetShm(int, size_t);

 private:
  std::unique_ptr<SharedMemory> uptr_shm_;

 public:
  ShmDataHandler() = default;
  ShmDataHandler(int, size_t);
  ~ShmDataHandler() = default;
};

}  // namespace utility
}  // namespace atd