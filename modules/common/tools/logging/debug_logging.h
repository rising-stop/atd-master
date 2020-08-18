#pragma once

#include <iostream>

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

  template <typename T1, typename... Ts>
  void LogInfo(const T1& arg, Ts&&... args) {
    LogInfo(arg);
    LogInfo(std::forward<Ts>(args)...);
  }

  template <typename T>
  void LogInfo(const T& arg) {
  }

  void LogInfo(const std::string& str) {}

 private:
 private:
  Proto_Messages<atd::protocol::LOG_CONTENT> log_frame_;
  LCM_Proxy<Proto_Messages<atd::protocol::LOG_CONTENT>> log_publisher_{
      LCM_MODE::SENDER, "PlanningLog"};

 private:
  DebugLogging() = default;
  virtual ~DebugLogging() = default;
};

}  // namespace utility
}  // namespace atd