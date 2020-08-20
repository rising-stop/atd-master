/* created by liangle */

#pragma once

#include <exception>
#include <sstream>
#include <string>

namespace atd {
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
  template <typename T, typename... ARGS>
  void set_Message(std::stringstream& sstm, const T& msg, ARGS&&... args) {
    sstm << msg;
    set_Message(sstm, std::forward<ARGS>(args)...);
  }

  template <typename T>
  void set_Message(std::stringstream& sstm, const T& msg) {
    sstm << msg;
  }

 private:
  std::string error_message_;

 public:
  CommonException() = default;
  template <typename... ARGS>
  CommonException(const char* file_name, uint32_t line_no, ARGS&&... args) {
    error_message_.append(file_name);
    error_message_.append(" no.");
    error_message_.append(std::to_string(line_no));
    error_message_.append(" : ");
    std::stringstream msg_load_;
    set_Message(msg_load_, std::forward<ARGS>(args)...);
    error_message_.append(msg_load_.str());
  }
  ~CommonException() = default;
};

}  // namespace utility
}  // namespace atd

#define CUSTOM_EXCEPTION(args...) \
  throw atd::utility::CommonException(__FILE__, __LINE__, args)
