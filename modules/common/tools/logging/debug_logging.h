#pragma once

#include "modules/common/utility/ipc/lcm_dispatcher/lcm_dispatcher.h"
#include "modules/common/utility/timer.h"
#include "modules/common/utility/utility.h"
#include "protobuf_msg/planning_log.pb.h"

namespace atd {
namespace utility {

static const std::string DISPLAY_SPEED{"spd_kph"};
static const std::string DISPLAY_STEER{"steer_deg"};
static const std::string DISPLAY_FLAG_AUTO{"flag_auto"};
static const std::string DISPLAY_DESIRED_ACC{"des_acc_ms"};
static const std::string DISPLAY_ACTUAL_ACC{"act_acc_ms"};
static const std::string DISPLAY_DTC{"DTC_code"};
static const std::string DISPLAY_SETTING_SPEED{"setting_spd_ms"};

enum SECURITY_INFO : int { INFO = 0, WARNING = 1, ERROR = 2 };

class DebugLogging : public Singleton {
  friend class Singleton;

 public:
  void reset_Frame();
  void publish_Frame();
  atd::protocol::FRAME_CONTENT* get_PtrFrame();
  atd::protocol::DISPLAY_BOX* get_PtrElementBox();
  atd::protocol::DISPLAY_LINE* get_PtrElementLine();
  atd::protocol::DISPLAY_PLOYNOMIAL* get_PtrElementPoly();
  atd::protocol::DISPLAY_CONTENT* get_PtrDisplayElement();

 private:
  Proto_Messages<atd::protocol::MONITOR_MSG> log_frame_;
  LCM_Proxy<Proto_Messages<atd::protocol::MONITOR_MSG>> log_publisher_{
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

#define LOG_DEBUG_INFO(file, line, level) \
  atd::utility::Writer(file, line, level).construct()
#define LCM_LOG_INFO LOG_DEBUG_INFO(__FILE__, __LINE__, atd::utility::INFO)
#define LCM_LOG_WARNING \
  LOG_DEBUG_INFO(__FILE__, __LINE__, atd::utility::WARNING)
#define LCM_LOG_ERROR LOG_DEBUG_INFO(__FILE__, __LINE__, atd::utility::ERROR)
#define GET_OPENGL_BOX                                            \
  atd::utility::Singleton::instance<atd::utility::DebugLogging>() \
      ->get_PtrElementBox()
#define GET_OPENGL_LINE                                           \
  atd::utility::Singleton::instance<atd::utility::DebugLogging>() \
      ->get_PtrElementLine()
#define GET_OPENGL_POLY                                           \
  atd::utility::Singleton::instance<atd::utility::DebugLogging>() \
      ->get_PtrElementPoly()
#define GET_DISPLAY_POINTER                                       \
  atd::utility::Singleton::instance<atd::utility::DebugLogging>() \
      ->get_PtrDisplayElement()

