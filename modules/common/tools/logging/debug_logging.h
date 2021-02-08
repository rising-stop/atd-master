#pragma once

#include "modules/common/utility/any_repository.h"
#include "modules/common/utility/ipc/lcm_dispatcher/lcm_dispatcher.h"
#include "modules/common/utility/timer.h"
#include "modules/common/utility/utility.h"
#include "modules/neo_adp/data_service/basic_setting.h"
#include "protobuf_msg/planning_log.pb.h"

#define DISPLAY_SPEED "spd_ms"
#define DISPLAY_STEER "steer_deg"
#define DISPLAY_FLAG_AUTO "flag_auto"
#define DISPLAY_DESIRED_ACC "des_acc_ms"
#define DISPLAY_ACTUAL_ACC "act_acc_ms"
#define DISPLAY_DTC "DTC_code"
#define DISPLAY_SETTING_SPEED "setting_spd_ms"

namespace Themis {
namespace utility {

enum SECURITY_INFO : int { INFO = 0, WARNING = 1, ERROR = 2 };

template <typename T>
class CalibrationVariable {
 public:
  CalibrationVariable(T var, T max, T min, T init)
      : var_(var), max_(max), min_(min), init_(init) {}

 public:
  inline void set_Var(const T& var) { var_ = var; }
  inline void set_Max(const T& max) { max_ = max; }
  inline void set_Min(const T& min) { min_ = min; }
  inline void set_Init(const T& init) { init_ = init; }

  inline const T& get_Var() const { return var_; }
  inline const T& get_Max() const { return max_; }
  inline const T& get_Min() const { return min_; }
  inline const T& get_Init() const { return init_; }

 private:
  T var_;
  T max_;
  T min_;
  T init_;
};

class DebugLogging : public Singleton {
  friend class Singleton;

 public:
  static void DebugInit();

 private:
  static bool is_init_;

 public:
  void reset_Frame();
  void publish_Frame();
  atd::protocol::FRAME_CONTENT* get_PtrFrame();
  atd::protocol::DISPLAY_BOX* get_PtrElementBox();
  atd::protocol::DISPLAY_LINE* get_PtrElementLine();
  atd::protocol::DISPLAY_PLOYNOMIAL* get_PtrElementPoly();

  atd::protocol::FLOAT_VAR* get_PtrElementDisplay_As_Float();
  atd::protocol::INT_VAR* get_PtrElementDisplay_As_Int();
  atd::protocol::UINT_VAR* get_PtrElementDisplay_As_UInt();
  atd::protocol::NORMAL_VAR* get_PtrElementDisplay_As_String();

  std::pair<float, bool> get_FloatCalibVariables(const std::string&);
  std::pair<int, bool> get_IntCalibVariables(const std::string&);
  std::pair<uint32_t, bool> get_UintCalibVariables(const std::string&);

  atd::protocol::FRAME_CONTENT* get_PtrLogFrame();

  template <typename T>
  std::pair<std::shared_ptr<CalibrationVariable<T>>, bool>
  try_register_Calibration(const std::string&, const T&, const T&, const T&);
  template <typename T>
  const std::pair<std::shared_ptr<CalibrationVariable<T>>, bool>
  try_fetch_Calibration(const std::string&) const;

  template <typename T>
  std::pair<std::shared_ptr<CalibrationVariable<T>>, bool>
  try_fetch_MutableCalibration(const std::string&);

 private:
  Proto_Messages<atd::protocol::MONITOR_MSG> log_frame_;
  LCM_Proxy<Proto_Messages<atd::protocol::MONITOR_MSG>> log_publisher_{
      LCM_MODE::SENDER, "PlanningLog"};

  Proto_Messages<atd::protocol::DISPLAY_CALIBRATION> cal_var_;
  LCM_Proxy<Proto_Messages<atd::protocol::DISPLAY_CALIBRATION>> calib_listener_{
      LCM_MODE::READER, "PlanningCalib"};

  Any_Repository calib_repository_;

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
  Writer& LogNormal(const std::string& arg_name, const T& arg,
                    TO_STRING func = StringConverter()) {
    auto ptr_var = ptr_content_->add_normal_vars();
    ptr_var->set_name(arg_name);
    ptr_var->set_data(func(arg));
    return *this;
  }

  Writer& LogFloat(const std::string& arg_name, const float& arg) {
    auto ptr_var = ptr_content_->add_float_vars();
    ptr_var->set_name(arg_name);
    ptr_var->set_data(arg);
    return *this;
  }

  Writer& LogInt(const std::string& arg_name, const int& arg) {
    auto ptr_var = ptr_content_->add_int_vars();
    ptr_var->set_name(arg_name);
    ptr_var->set_data(arg);
    return *this;
  }

  Writer& LogUInt(const std::string& arg_name, const uint32_t& arg) {
    auto ptr_var = ptr_content_->add_uint_vars();
    ptr_var->set_name(arg_name);
    ptr_var->set_data(arg);
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
}  // namespace Themis

#include "debug_logging.tcc"

#define DEBUG_LOGGING_INIT atd::utility::DebugLogging::DebugInit();

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
#define GET_FLOAT_DISPLAY_POINTER                                 \
  atd::utility::Singleton::instance<atd::utility::DebugLogging>() \
      ->get_PtrElementDisplay_As_Float()
#define GET_INT_DISPLAY_POINTER                                   \
  atd::utility::Singleton::instance<atd::utility::DebugLogging>() \
      ->get_PtrElementDisplay_As_Int()
#define GET_UINT_DISPLAY_POINTER                                  \
  atd::utility::Singleton::instance<atd::utility::DebugLogging>() \
      ->get_PtrElementDisplay_As_UInt()

#define REGISTER_CALIBRATION(type, variable, upper, lower, init)          \
  type variable = init;                                                   \
  auto res##variable =                                                    \
      atd::utility::Singleton::instance<atd::utility::DebugLogging>()     \
          ->try_register_Calibration(#variable, static_cast<type>(upper), \
                                     static_cast<type>(lower),            \
                                     static_cast<type>(init));            \
  if (!res##variable.second) variable = res##variable.first->get_Var();

#define REGISTER_FLOAT_AS_CALIBRATION(variable, upper, lower, init) \
  REGISTER_CALIBRATION(float, variable, upper, lower, init)

#define REGISTER_INT_AS_CALIBRATION(variable, upper, lower, init) \
  REGISTER_CALIBRATION(int, variable, upper, lower, init)

#define REGISTER_UINT_AS_CALIBRATION(variable, upper, lower, init) \
  REGISTER_CALIBRATION(uint32_t, variable, upper, lower, init)
