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
  void set_Message(const std::string& msg) { error_message_ = msg; }

 private:
  std::string error_message_;

 public:
  CommonException() = default;
  template <typename... ARGS>
  CommonException(const char* file_name, uint32_t line_no, const char* format,
                  ARGS&&... args) {
    error_message_.append(file_name);
    error_message_.append(" no.");
    error_message_.append(std::to_string(line_no));
    error_message_.append(" : ");
    char* excp_msg = new char();
    sprintf(excp_msg, format, std::forward<ARGS>(args)...);
    error_message_.append(excp_msg);
    delete excp_msg;
  }
  CommonException(const std::string& msg) { set_Message(msg); }
  ~CommonException() = default;
};

}  // namespace utility
}  // namespace atd

#define CUSTOM_EXCEPTION(format, args...) \
  throw atd::utility::CommonException(__FILE__, __LINE__, format, ##args)
