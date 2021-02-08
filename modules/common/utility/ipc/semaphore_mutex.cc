#include "semaphore_mutex.h"

#include <stdexcept>

#include "modules/common/utility/exceptions.h"

namespace Themis {
namespace utility {

int SemDispatcher::register_sem(::key_t key, int sgnl_num) {
  auto semid = semget(key, sgnl_num, 0666 | IPC_CREAT | IPC_EXCL);
  if (semid == -1) {
    semid = semget(key, sgnl_num, 0666 | IPC_CREAT);
  }
  if (semid == -1) {
    // std::stringstream error_msg;
    // error_msg << "semget error, errno: " << errno;
    // throw DispatcherException(key, DispatcherException::KEY_INVALID,
    // error_msg.str());
    CUSTOM_EXCEPTION("semget error, errno: ", errno);
  }

  registered_sems_.insert(std::make_pair(key, std::make_pair(semid, sgnl_num)));
}

void SemDispatcher::release_all_sem() {
  for (auto item4loop : registered_sems_) {
    release_sem(item4loop.first);
  }
}

void SemDispatcher::release_sem(::key_t key) {
  semun sem_union;
  auto itr_sem = registered_sems_.find(key);
  if (itr_sem != registered_sems_.end()) {
    auto res_semctl = semctl(itr_sem->second.first, itr_sem->second.second,
                             IPC_RMID, sem_union);
    if (res_semctl == -1) {
      // std::stringstream error_msg;
      // error_msg << "semctl error, errno: " << errno;
      // throw DispatcherException(key, DispatcherException::UNABLE_RELEASE,
      // error_msg.str());
      CUSTOM_EXCEPTION("semctl error, errno: ", errno);
    }
    registered_sems_.erase(itr_sem);
  }
}

std::pair<int, int> SemDispatcher::get_SemInfo(::key_t key) const {
  if (registered_sems_.find(key) != registered_sems_.end()) {
    return registered_sems_.at(key);
  } else {
    std::stringstream error_msg;
    error_msg << "key " << key << " not found";
    // throw DispatcherException(key, DispatcherException::KEY_NOT_EXIST,
    // error_msg.str());
    CUSTOM_EXCEPTION("key ", key, "not found");
  }
}

int SemMutex::get_SemID() const { return sem_id_; }

SemMutex::SemMutex(int sem_id) : sem_id_(sem_id) {}

SemMutex::~SemMutex() {}

void DualSemMutex::lock() {
  if (is_locked_) {
    // throw SemException(sem_id_, SemException::DUAL_DOUBLE_LOCK, "double
    // lock");
    CUSTOM_EXCEPTION("double lock");
  }
  sembuf sem_b;
  sem_b.sem_num = 0;
  sem_b.sem_op = -1;
  sem_b.sem_flg = SEM_UNDO;

  if (semop(sem_id_, &sem_b, 1) == -1) {
    // std::stringstream error_msg;
    // error_msg << "semop error, errno: " << errno;
    // throw SemException(sem_id_, SemException::SEM_ID_INVALID,
    // error_msg.str());
    CUSTOM_EXCEPTION("semop error, errno: ", errno);
  }
  is_locked_ = true;
}

void DualSemMutex::unlock() {
  if (!is_locked_) {
    // throw SemException(sem_id_, SemException::DUAL_DOUBLE_FREE, "double
    // free");
    CUSTOM_EXCEPTION("double free");
  }
  sembuf sem_b;
  sem_b.sem_num = 0;
  sem_b.sem_op = 1;
  sem_b.sem_flg = SEM_UNDO;

  if (semop(sem_id_, &sem_b, 1) == -1) {
    // std::stringstream error_msg;
    // error_msg << "semop error, errno: " << errno;
    // throw SemException(sem_id_, SemException::SEM_ID_INVALID,
    // error_msg.str());
    CUSTOM_EXCEPTION("semop error, errno: ", errno);
  }
  is_locked_ = false;
}

void DualSemMutex::init() {
  semun sem_union;
  sem_union.val = 1;
  if (semctl(sem_id_, 0, SETVAL, sem_union) == -1) {
    // std::stringstream error_msg;
    // error_msg << "semctl error, errno: " << errno;
    // throw SemException(sem_id_, SemException::SEM_ID_INVALID,
    // error_msg.str());
    CUSTOM_EXCEPTION("semctl error, errno: ", errno);
  }
}

DualSemMutex::DualSemMutex(int id) : SemMutex(id) {}
DualSemMutex::~DualSemMutex() {}

SharedSemMutex::SharedSemMutex(int id) : SemMutex(id) {}

void SharedSemMutex::init() {
  semun sem_union_write;
  sem_union_write.val = MAX_PROCESS_NUM;
  if (semctl(sem_id_, 0, SETVAL, sem_union_write) == -1) {
    // std::stringstream error_msg;
    // error_msg << "semctl error, errno: " << errno;
    // throw SemException(sem_id_, SemException::SEM_ID_INVALID,
    // error_msg.str());
    CUSTOM_EXCEPTION("semctl error, errno: ", errno);
  }
}

SharedSemMutex::~SharedSemMutex() {}

void SharedSemMutex::lock() {
  if (is_locked_) {
    // throw SemException(sem_id_, SemException::SHARED_WRITE_DOUBLE_LOCK,
    //                    "double lock");
    CUSTOM_EXCEPTION("double lock");
  }
  sembuf sem_b;
  sem_b.sem_num = 0;
  sem_b.sem_op = -MAX_PROCESS_NUM;
  sem_b.sem_flg = SEM_UNDO;

  if (semop(sem_id_, &sem_b, 1) == -1) {
    // std::stringstream error_msg;
    // error_msg << "semop error, errno: " << errno;
    // throw SemException(sem_id_, SemException::SEM_ID_INVALID,
    // error_msg.str());
    CUSTOM_EXCEPTION("semop error, errno: ", errno);
  }
  is_locked_ = true;
}

void SharedSemMutex::unlock() {
  if (!is_locked_) {
    // throw SemException(sem_id_, SemException::SHARED_WRITE_DOUBLE_FREE,
    //                    "double free");
    CUSTOM_EXCEPTION("double free");
  }
  sembuf sem_b;
  sem_b.sem_num = 0;
  sem_b.sem_op = MAX_PROCESS_NUM;
  sem_b.sem_flg = SEM_UNDO;

  if (semop(sem_id_, &sem_b, 1) == -1) {
    // std::stringstream error_msg;
    // error_msg << "semop error, errno: " << errno;
    // throw SemException(sem_id_, SemException::SEM_ID_INVALID,
    // error_msg.str());
    CUSTOM_EXCEPTION("semop error, errno: ", errno);
  }
  is_locked_ = false;
}

void SharedSemMutex::shared_lock() {
  if (is_shared_locked_) {
    // throw SemException(sem_id_, SemException::SHARED_READ_DOUBLE_LOCK,
    //  "double lock");
    CUSTOM_EXCEPTION("double lock");
  }
  sembuf sem_b;
  sem_b.sem_num = 0;
  sem_b.sem_op = -1;
  sem_b.sem_flg = SEM_UNDO;

  if (semop(sem_id_, &sem_b, 1) == -1) {
    // std::stringstream error_msg;
    // error_msg << "semop error, errno: " << errno;
    // throw SemException(sem_id_, SemException::SEM_ID_INVALID,
    // error_msg.str());
    CUSTOM_EXCEPTION("semop error, errno: ", errno);
  }
  is_shared_locked_ = true;
}

void SharedSemMutex::shared_unlock() {
  if (is_shared_locked_) {
    // throw SemException(sem_id_, SemException::SHARED_READ_DOUBLE_FREE,
    //                    "double free");
    CUSTOM_EXCEPTION("double free");
  }
  sembuf sem_b;
  sem_b.sem_num = 0;
  sem_b.sem_op = 1;
  sem_b.sem_flg = SEM_UNDO;

  if (semop(sem_id_, &sem_b, 1) == -1) {
    // std::stringstream error_msg;
    // error_msg << "semop error, errno: " << errno;
    // throw SemException(sem_id_, SemException::SEM_ID_INVALID,
    // error_msg.str());
    CUSTOM_EXCEPTION("semop error, errno: ", errno);
  }
  is_shared_locked_ = false;
}

}  // namespace utility
}  // namespace Themis