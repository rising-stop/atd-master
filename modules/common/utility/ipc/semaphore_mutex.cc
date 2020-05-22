#include "semaphore_mutex.h"

#include <iostream>
#include <stdexcept>

#include "modules/common/utility/exceptions.h"

namespace atd {
namespace common {
namespace utility {

SINGLETON_MEMBER_REGISTER(SemDispather)

std::unordered_map<::key_t, std::pair<int, int>> SemDispather::registered_sems_;

std::pair<int, int> SemDispather::register_sem(int sgnl_num) {
  // semid_ = semget(key_, sgnl_num, 0666 | IPC_CREAT | IPC_EXCL);
  // if (semid_ != -1) {
  //   init();
  // } else {
  //   semid_ = semget(key_, sgnl_num, 0666 | IPC_CREAT);
  // }
  // if (semid_ == -1) {
  //   std::runtime_error("SemMutex::Register_Sem: sem register failed");
  // }

  // registered_sems_.insert(
  //     std::make_pair(key_, std::make_pair(semid_, sgnl_num)));
}

void SemDispather::release_all_sem() {
  // semun sem_union;
  // for (auto sem4loop : registered_sems_) {
  //   for (int index4loop = 0; index4loop < sem4loop.second.second;
  //        index4loop++) {
  //     semctl(sem4loop.second.first, sem4loop.second.second, IPC_RMID,
  //            sem_union);
  //   }
  // }
  // registered_sems_.clear();
}

void SemDispather::release_sem() {
  // semun sem_union;
  // auto itr_sem = registered_sems_.find(key_);
  // if (itr_sem != registered_sems_.end()) {
  //   semctl(itr_sem->second.first, itr_sem->second.second, IPC_RMID,
  //   sem_union); registered_sems_.erase(itr_sem);
  // }
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