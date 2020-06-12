/* created by liangle */

#pragma once

#include <exception>
#include <sstream>
#include <string>

namespace atd {
namespace common {
namespace utility {

/**
 * @brief Class CommonException
 * derived from class exception, expected to deal the exceptions that happened
 * in common lib, two basic functions are:
 *   1. default construction
 *   2. setting error msg
 */
class CommonException : public std::exception {
 public:
  virtual const char* what() const noexcept override {
    return error_message_.c_str();
  }

 protected:
  void set_Message(const std::string& msg) { error_message_ = msg; }

 private:
  std::string error_message_{"common exception"};

 public:
  CommonException() = default;
  CommonException(const std::string& msg) { set_Message(msg); }
  ~CommonException() = default;
};

/**
 * @brief Class SemException
 * containing messages from class semaphore_mutex
 */
class SemException : public CommonException {
 public:
  enum INVALID_PARTERN : int {
    UNKNOW = 0,
    SEM_ID_INVALID = 1,
    OVER_SUBSCRIBE = 2,
    DUAL_DOUBLE_LOCK = 3,
    DUAL_DOUBLE_FREE = 4,
    SHARED_WRITE_DOUBLE_LOCK = 5,
    SHARED_WRITE_DOUBLE_FREE = 6,
    SHARED_READ_DOUBLE_LOCK = 7,
    SHARED_READ_DOUBLE_FREE = 8
  };

 private:
  int sem_id_;
  INVALID_PARTERN error_code_;

 public:
  /**
   * @brief constructor:
   * @param
   *   1. int key: sem id
   *   2. INVALID_PARTERN error_code:  some invalid partern code
   *   3. const string&: error message
   */
  SemException(int id, INVALID_PARTERN error_code, const std::string& str)
      : sem_id_(id), error_code_(error_code) {
    std::stringstream sstr;
    sstr << "sem id " << sem_id_ << " throw error code "
         << static_cast<int>(error_code_) << ": ";
    set_Message(sstr.str() + str);
  };
};

/**
 * @brief Class SemDispatcherException
 * containing messages from class SemDispatcher
 */
class DispatcherException : public CommonException {
 public:
  enum INVALID_PARTERN : int {
    UNKNOW = 0,
    KEY_INVALID = 1,
    UNABLE_RELEASE = 2,
    KEY_NOT_EXIST = 3
  };

 private:
  int key_;
  INVALID_PARTERN error_code_;

 public:
  /**
   * @brief constructor:
   * @param
   *   1. int key: sem key
   *   2. INVALID_PARTERN error_code:  some invalid partern code
   *   3. const string&: error message
   */
  DispatcherException(int key, INVALID_PARTERN error_code,
                      const std::string& str)
      : key_(key), error_code_(error_code) {
    std::stringstream sstr;
    sstr << "key " << key << " throw error code "
         << static_cast<int>(error_code_) << ": ";
    set_Message(sstr.str() + str);
  };
};

/**
 * @brief Class SemDispatcherException
 * containing messages from class SemDispatcher
 */
class ShmException : public CommonException {
 public:
  enum INVALID_PARTERN : int {
    UNKNOW = 0,
    MSG_OVERFLOW = 1,
    UNABLE_TO_READ = 2,
    UNABLE_TO_WRITE = 3,
    INVALID_ID = 4,
    INVALID_SIZE = 5,
    MEMORY_ASSIGN_ERROR = 6,
    UNABLE_DETACH = 7,
    DISPATCHER_DENIED = 8
  };

 private:
  int shm_id_;
  INVALID_PARTERN error_code_;

 public:
  /**
   * @brief constructor:
   * @param
   *   1. int key: sem id
   *   2. INVALID_PARTERN error_code:  some invalid partern code
   *   3. const string&: error message
   */
  ShmException(int id, INVALID_PARTERN error_code, const std::string& str)
      : shm_id_(id), error_code_(error_code) {
    std::stringstream sstr;
    sstr << "shm id " << shm_id_ << " throw error code "
         << static_cast<int>(error_code_) << ": ";
    set_Message(sstr.str() + str);
  };
};

/**
 * @brief Class FileException
 * containing messages from class ReadWriteableFile and its derives
 */
class FileException : public CommonException {
 public:
  enum INVALID_PARTERN : int {
    UNKNOW = 0,
    UNABLE_TO_OPEN = 1,
    GOOD_CHECK_FAIL = 2
  };

 private:
  INVALID_PARTERN error_code_;

 public:
  /**
   * @brief constructor:
   * @param
   *   1. INVALID_PARTERN error_code:  some invalid partern code
   *   2. const string&: error message
   */
  FileException(INVALID_PARTERN error_code, const std::string& str)
      : error_code_(error_code) {
    std::stringstream sstr;
    sstr << "file throw error code " << static_cast<int>(error_code_) << ": ";
    set_Message(sstr.str() + str);
  };
};

}  // namespace utility
}  // namespace common
}  // namespace atd