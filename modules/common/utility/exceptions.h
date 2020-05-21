/* created by liangle */

#pragma once

#include <exception>
#include <string>

namespace atd {
namespace common {
namespace utility {

class CommonException : std::exception {
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

}  // namespace utility
}  // namespace common
}  // namespace atd