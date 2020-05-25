#include "semaphore_mutex.h"

#include <iostream>
#include <stdexcept>

#include "modules/common/utility/exceptions.h"

namespace atd {
namespace common {
namespace utility {

SINGLETON_MEMBER_REGISTER(SemDispather)

int SemDispather::register_sem(::key_t key, int sgnl_num) {
  auto semid = semget(key, sgnl_num, 0666 | IPC_CREAT | IPC_EXCL);
  if (semid == -1) {
    semid = semget(key, sgnl_num, 0666 | IPC_CREAT);
  }
  if (semid == -1) {
    std::stringstream error_msg;
    error_msg << "semget error, errno: " << errno;
    throw SemDispatcherException(key, SemDispatcherException::KEY_INVALID,
                                 error_msg.str());
  }

  registered_sems_.insert(std::make_pair(key, std::make_pair(semid, sgnl_num)));
}

void SemDispather::release_all_sem() {
  for (auto item4loop : registered_sems_) {
    release_sem(item4loop.first);
  }
}

void SemDispather::release_sem(::key_t key) {
  semun sem_union;
  auto itr_sem = registered_sems_.find(key);
  if (itr_sem != registered_sems_.end()) {
    auto res_semctl = semctl(itr_sem->second.first, itr_sem->second.second,
                             IPC_RMID, sem_union);
    if (res_semctl == -1) {
      std::stringstream error_msg;
      error_msg << "semctl error, errno: " << errno;
      throw SemDispatcherException(
          key, SemDispatcherException::SEM_RELEASE_ERROR, error_msg.str());
    }
    registered_sems_.erase(itr_sem);
  }
}

std::pair<int, int> SemDispather::get_SemInfo(::key_t key) const {
  if (registered_sems_.find(key) != registered_sems_.end()) {
    return registered_sems_.at(key);
  } else {
    std::stringstream error_msg;
    error_msg << "key " << key << " not found";
    throw SemDispatcherException(key, SemDispatcherException::KEY_NOT_EXIST,
                                 error_msg.str());
  }
}

int SemMutex::get_SemID() const { return sem_id_; }

SemMutex::SemMutex(int sem_id) : sem_id_(sem_id) {}

SemMutex::~SemMutex() {}

void Dual_SemMutex::lock() {
  if (is_locked_) {
    throw SemException(sem_id_, SemException::DUAL_DOUBLE_LOCK, "double lock");
  }
  sembuf sem_b;
  sem_b.sem_num = 0;
  sem_b.sem_op = -1;
  sem_b.sem_flg = SEM_UNDO;

  if (semop(sem_id_, &sem_b, 1) == -1) {
    std::stringstream error_msg;
    error_msg << "semop error, errno: " << errno;
    throw SemException(sem_id_, SemException::SEM_ID_INVALID, error_msg.str());
  }
  is_locked_ = true;
}

void Dual_SemMutex::unlock() {
  if (!is_locked_) {
    throw SemException(sem_id_, SemException::DUAL_DOUBLE_FREE, "double free");
  }
  sembuf sem_b;
  sem_b.sem_num = 0;
  sem_b.sem_op = 1;
  sem_b.sem_flg = SEM_UNDO;

  if (semop(sem_id_, &sem_b, 1) == -1) {
    std::stringstream error_msg;
    error_msg << "semop error, errno: " << errno;
    throw SemException(sem_id_, SemException::SEM_ID_INVALID, error_msg.str());
  }
  is_locked_ = false;
}

void Dual_SemMutex::init() {
  semun sem_union;
  sem_union.val = 1;
  if (semctl(sem_id_, 0, SETVAL, sem_union) == -1) {
    std::stringstream error_msg;
    error_msg << "semctl error, errno: " << errno;
    throw SemException(sem_id_, SemException::SEM_ID_INVALID, error_msg.str());
  }
}

Dual_SemMutex::Dual_SemMutex(int id) : SemMutex(id) {}
Dual_SemMutex::~Dual_SemMutex() {}

Shared_SemMutex::Shared_SemMutex(int id) : SemMutex(id) {}

void Shared_SemMutex::init() {
  semun sem_union_write;
  sem_union_write.val = MAX_PROCESS_NUM;
  if (semctl(sem_id_, 0, SETVAL, sem_union_write) == -1) {
    std::stringstream error_msg;
    error_msg << "semctl error, errno: " << errno;
    throw SemException(sem_id_, SemException::SEM_ID_INVALID, error_msg.str());
  }
}

Shared_SemMutex::~Shared_SemMutex() {}

void Shared_SemMutex::lock() {
  if (is_locked_) {
    throw SemException(sem_id_, SemException::SHARED_WRITE_DOUBLE_LOCK,
                       "double lock");
  }
  sembuf sem_b;
  sem_b.sem_num = 0;
  sem_b.sem_op = -MAX_PROCESS_NUM;
  sem_b.sem_flg = SEM_UNDO;

  if (semop(sem_id_, &sem_b, 1) == -1) {
    std::stringstream error_msg;
    error_msg << "semop error, errno: " << errno;
    throw SemException(sem_id_, SemException::SEM_ID_INVALID, error_msg.str());
  }
  is_locked_ = true;
}

void Shared_SemMutex::unlock() {
  if (!is_locked_) {
    throw SemException(sem_id_, SemException::SHARED_WRITE_DOUBLE_FREE,
                       "double free");
  }
  sembuf sem_b;
  sem_b.sem_num = 0;
  sem_b.sem_op = MAX_PROCESS_NUM;
  sem_b.sem_flg = SEM_UNDO;

  if (semop(sem_id_, &sem_b, 1) == -1) {
    std::stringstream error_msg;
    error_msg << "semop error, errno: " << errno;
    throw SemException(sem_id_, SemException::SEM_ID_INVALID, error_msg.str());
  }
  is_locked_ = false;
}

void Shared_SemMutex::shared_lock() {
  if (is_shared_locked_) {
    throw SemException(sem_id_, SemException::SHARED_READ_DOUBLE_LOCK,
                       "double lock");
  }
  sembuf sem_b;
  sem_b.sem_num = 0;
  sem_b.sem_op = -1;
  sem_b.sem_flg = SEM_UNDO;

  if (semop(sem_id_, &sem_b, 1) == -1) {
    std::stringstream error_msg;
    error_msg << "semop error, errno: " << errno;
    throw SemException(sem_id_, SemException::SEM_ID_INVALID, error_msg.str());
  }
  is_shared_locked_ = true;
}

void Shared_SemMutex::shared_unlock() {
  if (is_shared_locked_) {
    throw SemException(sem_id_, SemException::SHARED_READ_DOUBLE_FREE,
                       "double free");
  }
  sembuf sem_b;
  sem_b.sem_num = 0;
  sem_b.sem_op = 1;
  sem_b.sem_flg = SEM_UNDO;

  if (semop(sem_id_, &sem_b, 1) == -1) {
    std::stringstream error_msg;
    error_msg << "semop error, errno: " << errno;
    throw SemException(sem_id_, SemException::SEM_ID_INVALID, error_msg.str());
  }
  is_shared_locked_ = false;
}

}  // namespace utility
}  // namespace common
}  // namespace atd