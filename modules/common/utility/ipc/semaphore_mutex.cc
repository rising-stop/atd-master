#include "semaphore_mutex.h"

#include <iostream>
#include <stdexcept>

namespace atd {
namespace common {
namespace utility {

std::unordered_map<::key_t, std::pair<int, int>> SemMutex::registered_sems_;

std::pair<int, int> SemMutex::register_sem(int sgnl_num) {
  semid_ = semget(key_, sgnl_num, 0666 | IPC_CREAT | IPC_EXCL);
  if (semid_ != -1) {
    init();
  } else {
    semid_ = semget(key_, sgnl_num, 0666 | IPC_CREAT);
  }
  if (semid_ == -1) {
    std::runtime_error("SemMutex::Register_Sem: sem register failed");
  }

  registered_sems_.insert(
      std::make_pair(key_, std::make_pair(semid_, sgnl_num)));
}

void SemMutex::release_all_sem() {
  semun sem_union;
  for (auto sem4loop : registered_sems_) {
    for (int index4loop = 0; index4loop < sem4loop.second.second;
         index4loop++) {
      semctl(sem4loop.second.first, sem4loop.second.second, IPC_RMID,
             sem_union);
    }
  }
  registered_sems_.clear();
}

void SemMutex::release_sem() {
  semun sem_union;
  auto itr_sem = registered_sems_.find(key_);
  if (itr_sem != registered_sems_.end()) {
    semctl(itr_sem->second.first, itr_sem->second.second, IPC_RMID, sem_union);
    registered_sems_.erase(itr_sem);
  }
}

::key_t SemMutex::get_key() const { return key_; }

int SemMutex::get_semID() const { return semid_; }

SemMutex::SemMutex(::key_t key) : key_(key) {}

SemMutex::~SemMutex() {}

void Dual_SemMutex::lock() {
  if (is_locked_) {
    std::logic_error("Dual_SemMutex::lock: Fatal error, double lock");
  }
  sembuf sem_b;
  sem_b.sem_num = 0;
  sem_b.sem_op = -1;
  sem_b.sem_flg = SEM_UNDO;

  if (semop(semid_, &sem_b, 1) == -1) {
    std::string error_msg;
    error_msg.append("semop error, errno: ");
    error_msg.append(std::to_string(errno));
    std::runtime_error(error_msg.c_str());
  }
  is_locked_ = true;
}

void Dual_SemMutex::unlock() {
  if (!is_locked_) {
    std::logic_error("Dual_SemMutex::unlock: Fatal error, double free");
  }
  sembuf sem_b;
  sem_b.sem_num = 0;
  sem_b.sem_op = 1;
  sem_b.sem_flg = SEM_UNDO;

  if (semop(semid_, &sem_b, 1) == -1) {
    std::string error_msg;
    error_msg.append("semop error, errno: ");
    error_msg.append(std::to_string(errno));
    std::runtime_error(error_msg.c_str());
  }
  is_locked_ = false;
}

void Dual_SemMutex::init() {
  semun sem_union;
  sem_union.val = 1;
  semctl(semid_, 0, SETVAL, sem_union);
}

Dual_SemMutex::Dual_SemMutex(::key_t key) : SemMutex(key) {
  auto res = register_sem(1);
}
Dual_SemMutex::~Dual_SemMutex() {}

Shared_SemMutex::Shared_SemMutex(::key_t key) : SemMutex(key) {
  auto res = register_sem(1);
}

void Shared_SemMutex::init() {
  semun sem_union_write;
  sem_union_write.val = MAX_PROCESS_NUM;
  semctl(semid_, 0, SETVAL, sem_union_write);
}

Shared_SemMutex::~Shared_SemMutex() {}

void Shared_SemMutex::lock() {
  if (is_locked_) {
    std::logic_error("Shared_SemMutex::lock: Fatal error, double lock");
  }
  sembuf sem_b;
  sem_b.sem_num = 0;
  sem_b.sem_op = -MAX_PROCESS_NUM;
  sem_b.sem_flg = SEM_UNDO;

  if (semop(semid_, &sem_b, 1) == -1) {
    std::string error_msg;
    error_msg.append("semop error, errno: ");
    error_msg.append(std::to_string(errno));
    std::runtime_error(error_msg.c_str());
  }
  is_locked_ = true;
}

void Shared_SemMutex::unlock() {
  if (!is_locked_) {
    std::logic_error("Shared_SemMutex::unlock: Fatal error, double free");
  }
  sembuf sem_b;
  sem_b.sem_num = 0;
  sem_b.sem_op = MAX_PROCESS_NUM;
  sem_b.sem_flg = SEM_UNDO;

  if (semop(semid_, &sem_b, 1) == -1) {
    std::string error_msg;
    error_msg.append("semop error, errno: ");
    error_msg.append(std::to_string(errno));
    std::runtime_error(error_msg.c_str());
  }
  is_locked_ = false;
}

void Shared_SemMutex::shared_lock() {
  if (is_shared_locked_) {
    std::logic_error("Shared_SemMutex::shared_lock: Fatal error, double lock");
  }
  sembuf sem_b;
  sem_b.sem_num = 0;
  sem_b.sem_op = -1;
  sem_b.sem_flg = SEM_UNDO;

  int res_shared_op = semop(semid_, &sem_b, 1);
  std::cout << "res_shared_op = " << res_shared_op << std::endl;
  if (res_shared_op == -1) {
    // if (errno != EAGAIN) {
    std::string error_msg;
    error_msg.append("semop error, errno: ");
    error_msg.append(std::to_string(errno));
    std::runtime_error(error_msg.c_str());
    // }
  }
  is_shared_locked_ = true;
}

void Shared_SemMutex::shared_unlock() {
  if (is_shared_locked_) {
    std::logic_error(
        "Shared_SemMutex::shared_unlock: Fatal error, double free");
  }
  sembuf sem_b;
  sem_b.sem_num = 0;
  sem_b.sem_op = 1;
  sem_b.sem_flg = SEM_UNDO;

  if (semop(semid_, &sem_b, 1) == -1) {
    if (errno != EAGAIN) {
      std::string error_msg;
      error_msg.append("semop error, errno: ");
      error_msg.append(std::to_string(errno));
      std::runtime_error(error_msg.c_str());
    }
  }
  is_shared_locked_ = false;
}

}  // namespace utility
}  // namespace common
}  // namespace atd