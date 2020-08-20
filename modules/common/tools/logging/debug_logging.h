#pragma once

#include "modules/common/utility/ipc/lcm_dispatcher/lcm_dispatcher.h"
#include "modules/common/utility/timer.h"
#include "modules/common/utility/utility.h"
#include "protobuf_msg/planning_log.pb.h"

namespace atd {
namespace utility {

enum SECURITY_INFO : int { INFO = 0, WARNING = 1, ERROR = 2 };

class DebugLogging : public Singleton {
  friend class Singleton;

 public:
  void reset_Frame();
  void publish_Frame();
  atd::protocol::FRAME_CONTENT* get_PtrFrame();

 private:
  Proto_Messages<atd::protocol::LOG_CONTENT> log_frame_;
  LCM_Proxy<Proto_Messages<atd::protocol::LOG_CONTENT>> log_publisher_{
      LCM_MODE::SENDER, "PlanningLog"};

 private:
  DebugLogging();
  virtual ~DebugLogging() = default;
};

class StringConverter {
 public:
  template <typename T>
  std::string operator()(const T& var) {
    return std::to_string(var);
  }

  StringConverter() = default;
  ~StringConverter() = default;
};

class Writer {
 public:
  Writer& construct();

  template <typename T, typename TO_STRING = StringConverter>
  Writer& LogVar(const std::string& arg_name, const T& arg,
                 TO_STRING func = StringConverter()) {
    auto ptr_var = ptr_content_->add_variables();
    ptr_var->set_name(arg_name);
    ptr_var->set_data(func(arg));
    return *this;
  }

  Writer& LogInfo(const std::string& info) {
    ptr_content_->add_str_msg(info);
    return *this;
  }

 private:
  void init();
  atd::protocol::FRAME_CONTENT* ptr_content_;

 public:
  Writer(const char* file, uint32_t line_no, SECURITY_INFO level);
  ~Writer() = default;
};

}  // namespace utility
}  // namespace atd

#define LOG_DEBUG_INFO(file, line, level) Writer(file, line, level).construct()
#define LCM_LOG(level) LOG_DEBUG_INFO(__FILE__, __LINE__, level)
