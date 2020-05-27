#include <cstring>

#include "shared_memory.h"

namespace atd {
namespace common {
namespace utility {

template <typename TYPE>
void SharedMemory::send_Msg(const TYPE& msg, size_t size) {
  if (size < 1) {
    return;
  }
  if (size > shmsize_) {
    std::stringstream sstm;
    sstm << "shm message overflow, input size " << size << " shm size "
         << shmsize_;
    throw ShmException(shmid_, ShmException::MSG_OVERFLOW, sstm.str());
  }
  auto res_ptr = memcpy(addr_, static_cast<const void*>(&msg), size);
  if (!res_ptr) {
    throw ShmException(shmid_, ShmException::UNABLE_TO_WRITE,
                       "memcpy returns a nullptr");
  }
}

template <typename TYPE>
void SharedMemory::read_Msg(TYPE& msg, size_t size) {
  if (size > shmsize_) {
    std::stringstream sstm;
    sstm << "shm message overflow, required size " << size << " shm size "
         << shmsize_;
    throw ShmException(shmid_, ShmException::MSG_OVERFLOW, sstm.str());
  }
  auto res_ptr = memcpy(static_cast<void*>(&msg), addr_, size);
  if (!res_ptr) {
    throw ShmException(shmid_, ShmException::UNABLE_TO_READ,
                       "memcpy returns a nullptr");
  }
}

}  // namespace utility
}  // namespace common
}  // namespace atd