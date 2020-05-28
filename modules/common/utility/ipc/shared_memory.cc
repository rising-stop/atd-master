#include "shared_memory.h"

#include <cstring>
#include <sstream>

namespace atd {
namespace common {
namespace utility {

SINGLETON_MEMBER_REGISTER(ShmDispatcher)

int ShmDispatcher::register_shm(::key_t key, int size) {
  auto shmid = shmget(key, size, 0666 | IPC_CREAT | IPC_EXCL);
  if (shmid == -1) {
    shmid = shmget(key, size, 0666 | IPC_CREAT);
  }
  if (shmid == -1) {
    std::stringstream error_msg;
    error_msg << "semget error, errno: " << errno;
    throw DispatcherException(key, DispatcherException::KEY_INVALID,
                              error_msg.str());
  }

  registered_shms_.insert(std::make_pair(key, std::make_pair(shmid, size)));
}

void ShmDispatcher::release_all_shm() {
  for (auto item4loop : registered_shms_) {
    release_shm(item4loop.first);
  }
}

void ShmDispatcher::release_shm(::key_t key) {
  auto itr_shm = registered_shms_.find(key);
  if (itr_shm != registered_shms_.end()) {
    auto res_semctl = shmctl(itr_shm->second.first, IPC_RMID, nullptr);
    if (res_semctl == -1) {
      std::stringstream error_msg;
      error_msg << "semctl error, errno: " << errno;
      throw DispatcherException(key, DispatcherException::UNABLE_RELEASE,
                                error_msg.str());
    }
    registered_shms_.erase(itr_shm);
  }
}

std::pair<int, int> ShmDispatcher::get_ShmInfo(::key_t key) const {
  if (registered_shms_.find(key) != registered_shms_.end()) {
    return registered_shms_.at(key);
  } else {
    std::stringstream error_msg;
    error_msg << "key " << key << " not found";
    throw DispatcherException(key, DispatcherException::KEY_NOT_EXIST,
                              error_msg.str());
  }
}

int SharedMemory::get_ShmID() const { return shmid_; }
size_t SharedMemory::get_ShmSize() const { return shmsize_; }
const void* SharedMemory::get_AssignedAddr() const { return addr_; }

void SharedMemory::mount_Shm() {
  if (shmid_ < 0) {
    std::stringstream sstm;
    sstm << "shm id received abnormal, required id " << shmid_;
    throw ShmException(shmid_, ShmException::INVALID_ID, sstm.str());
  }
  if (shmsize_ < 0) {
    std::stringstream sstm;
    sstm << "shm size received abnormal, required size " << shmsize_;
    throw ShmException(shmid_, ShmException::INVALID_SIZE, sstm.str());
  }
  addr_ = shmat(shmid_, nullptr, 0);
  if (!addr_) {
    std::stringstream sstm;
    sstm << "shmat return nullptr, errno: " << errno;
    throw ShmException(shmid_, ShmException::MEMORY_ASSIGN_ERROR, sstm.str());
  }
}

void SharedMemory::unmount_Shm() {
  auto res = shmdt(addr_);
  if (res) {
    std::stringstream sstm;
    sstm << "shmdt return -1, errno: " << errno;
    throw ShmException(shmid_, ShmException::UNABLE_DETACH, sstm.str());
  }
}

void SharedMemory::send_Msg(const std::string& str) {
  if (str.empty()) {
    return;
  }
  auto msg_size = str.size();
  if (msg_size > shmsize_) {
    std::stringstream sstm;
    sstm << "shm message overflow, input size " << msg_size << " shm size "
         << shmsize_;
    throw ShmException(shmid_, ShmException::MSG_OVERFLOW, sstm.str());
  }
  auto res_ptr = memcpy(addr_, static_cast<const void*>(str.c_str()), msg_size);
  if (!res_ptr) {
    throw ShmException(shmid_, ShmException::UNABLE_TO_WRITE,
                       "memcpy returns a nullptr");
  }
}

void SharedMemory::read_Msg(std::string& str, size_t size) {
  if (size > shmsize_) {
    std::stringstream sstm;
    sstm << "shm message overflow, required size " << size << " shm size "
         << shmsize_;
    throw ShmException(shmid_, ShmException::MSG_OVERFLOW, sstm.str());
  }
  str.clear();
  char* ptr_char;
  auto res_ptr = memcpy(ptr_char, addr_, size);
  if (!res_ptr) {
    throw ShmException(shmid_, ShmException::UNABLE_TO_READ,
                       "memcpy returns a nullptr");
  }
  str.copy(ptr_char, size);
}

SharedMemory::SharedMemory(int id, size_t size) : shmid_(id), shmsize_(size) {
  mount_Shm();
}

SharedMemory::SharedMemory(std::pair<int, size_t> info)
    : shmid_(info.first), shmsize_(info.second) {
  mount_Shm();
}

std::pair<int, size_t> SharedMemory::try_get_ShmID(::key_t key, size_t size) {
  int shmid = shmget(key, size, 0666);
  if (shmid == -1) {
    std::stringstream error_msg;
    error_msg << "semget error, errno: " << errno
              << " waiting DISPATCHER to be created, DISPATCHER key: "
              << key;
    throw ShmException(0, ShmException::DISPATCHER_DENIED, error_msg.str());
  }
  return std::make_pair(shmid, size);
}

}  // namespace utility
}  // namespace common
}  // namespace atd