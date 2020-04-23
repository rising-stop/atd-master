#include "semaphore.h"

#include <iostream>
#include <stdexcept>

namespace atd {
namespace common {
namespace utility {

::key_t SemMutex::get_Key() const { return key_; }

SemMutex::SemMutex(::key_t key) : key_(key) {}

#define SEM_LOCK_INDEX 0

void Dual_SemMutex::lock() {
  if (is_locked_) {
    std::logic_error("Dual_SemMutex::lock: Fatal error, double lock");
  }
  sembuf sem_b;
  sem_b.sem_num = SEM_LOCK_INDEX;
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
  sem_b.sem_num = SEM_LOCK_INDEX;
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
  semctl(semid_, SEM_LOCK_INDEX, SETVAL, sem_union);
}

Dual_SemMutex::Dual_SemMutex(::key_t key) : SemMutex(key) {
  semid_ = semget(key_, 1, 0666 | IPC_CREAT | IPC_EXCL);
  if (semid_ != -1) {
    init();
  } else {
    semid_ = semget(key_, 1, 0666 | IPC_CREAT);
  }
}
Dual_SemMutex::~Dual_SemMutex() {
  semun sem_union;
  semctl(semid_, SEM_LOCK_INDEX, IPC_RMID, sem_union);
}

#define SHARED_SEM_LOCK_INDEX 1

Shared_SemMutex::Shared_SemMutex(::key_t key) : SemMutex(key) {
  semid_ = semget(key, 2, 0666 | IPC_CREAT | IPC_EXCL);
  if (semid_ != -1) {
    init();
  } else {
    semid_ = semget(key, 2, 0666 | IPC_CREAT);
  }
  std::cout << "Shared_SemMutex: semid_ " << semid_ << std::endl;
}
Shared_SemMutex::~Shared_SemMutex() {
  semun sem_union;
  semctl(semid_, SEM_LOCK_INDEX, IPC_RMID, sem_union);
  semctl(semid_, SHARED_SEM_LOCK_INDEX, IPC_RMID, sem_union);
}

void Shared_SemMutex::lock() {
  if (is_locked_) {
    std::logic_error("Shared_SemMutex::lock: Fatal error, double lock");
  }
  sembuf sem_b[2];
  sem_b[0].sem_num = SEM_LOCK_INDEX;
  sem_b[0].sem_op = -1;
  sem_b[0].sem_flg = SEM_UNDO;
  sem_b[1].sem_num = SHARED_SEM_LOCK_INDEX;
  sem_b[1].sem_op = -1;
  sem_b[1].sem_flg = SEM_UNDO;

  if (semop(semid_, sem_b, 2) == -1) {
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
  sembuf sem_b[2];
  sem_b[0].sem_num = SEM_LOCK_INDEX;
  sem_b[0].sem_op = 1;
  sem_b[0].sem_flg = SEM_UNDO;
  sem_b[1].sem_num = SHARED_SEM_LOCK_INDEX;
  sem_b[1].sem_op = 1;
  sem_b[1].sem_flg = SEM_UNDO;

  if (semop(semid_, sem_b, 2) == -1) {
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
  sem_b.sem_num = SHARED_SEM_LOCK_INDEX;
  sem_b.sem_op = -1;
  sem_b.sem_flg = SEM_UNDO;

  if (semop(semid_, &sem_b, 1) == -1) {
    std::string error_msg;
    error_msg.append("semop error, errno: ");
    error_msg.append(std::to_string(errno));
    std::runtime_error(error_msg.c_str());
  }
  is_shared_locked_ = true;
}

void Shared_SemMutex::shared_unlock() {
  if (is_shared_locked_) {
    std::logic_error(
        "Shared_SemMutex::shared_unlock: Fatal error, double free");
  }
  sembuf sem_b;
  sem_b.sem_num = SHARED_SEM_LOCK_INDEX;
  sem_b.sem_op = 1;
  sem_b.sem_flg = SEM_UNDO;

  if (semop(semid_, &sem_b, 1) == -1) {
    std::string error_msg;
    error_msg.append("semop error, errno: ");
    error_msg.append(std::to_string(errno));
    std::runtime_error(error_msg.c_str());
  }
  is_shared_locked_ = false;
}

void Shared_SemMutex::init() {
  semun sem_union;
  sem_union.val = 1;
  semctl(semid_, SEM_LOCK_INDEX, SETVAL, sem_union);
  semctl(semid_, SHARED_SEM_LOCK_INDEX, SETVAL, sem_union);
}

}  // namespace utility
}  // namespace common
}  // namespace atd