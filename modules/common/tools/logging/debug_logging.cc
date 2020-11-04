#include "debug_logging.h"

namespace atd {
namespace utility {

void DebugLogging::DebugInit() {
  if (!is_init_) {
    atd::utility::Singleton::try_register<atd::utility::Runtime_Calculator<>>();
    atd::utility::Singleton::try_register<atd::utility::Runtime_Counter>();
    atd::utility::Singleton::try_register<atd::utility::DebugLogging>();
    is_init_ = true;
  }
}

bool DebugLogging::is_init_ = false;

DebugLogging::DebugLogging() { TIMER->set_BeginStick("mile_stone"); }

void DebugLogging::reset_Frame() {
  TIMER->wait_and_set_EndStick("mile_stone");
  log_frame_.Clear();
  log_frame_.mutable_title()->set_time_stamp(
      TIMER->get_TimeGap("mile_stone").first);
  log_frame_.mutable_title()->set_counter_no(COUNTER("planning_log"));

  calib_listener_.subscribe(cal_var_);
  for (auto& var4loop : cal_var_.calib_float()) {
    auto res = calib_repository_.try_RegisterVar<CalibrationVariable<float>>(
        var4loop.name(), var4loop.data(), var4loop.data_upper_bound(),
        var4loop.data_lower_bound(), var4loop.data_init());
    if (!res.second) {
      res.first->set_Init(var4loop.data_init());
      res.first->set_Max(var4loop.data_upper_bound());
      res.first->set_Min(var4loop.data_lower_bound());
      res.first->set_Var(var4loop.data());
    }
  }
  for (auto& var4loop : cal_var_.calib_int()) {
    auto res = calib_repository_.try_RegisterVar<CalibrationVariable<int>>(
        var4loop.name(), var4loop.data(), var4loop.data_upper_bound(),
        var4loop.data_lower_bound(), var4loop.data_init());
    if (!res.second) {
      res.first->set_Init(var4loop.data_init());
      res.first->set_Max(var4loop.data_upper_bound());
      res.first->set_Min(var4loop.data_lower_bound());
      res.first->set_Var(var4loop.data());
    }
  }
  for (auto& var4loop : cal_var_.calib_uint()) {
    auto res = calib_repository_.try_RegisterVar<CalibrationVariable<int32_t>>(
        var4loop.name(), var4loop.data(), var4loop.data_upper_bound(),
        var4loop.data_lower_bound(), var4loop.data_init());
    if (!res.second) {
      res.first->set_Init(var4loop.data_init());
      res.first->set_Max(var4loop.data_upper_bound());
      res.first->set_Min(var4loop.data_lower_bound());
      res.first->set_Var(var4loop.data());
    }
  }
}

atd::protocol::FRAME_CONTENT* DebugLogging::get_PtrFrame() {
  return log_frame_.mutable_log()->add_content();
}
atd::protocol::DISPLAY_BOX* DebugLogging::get_PtrElementBox() {
  return log_frame_.mutable_gl_element()->add_box_set();
}
atd::protocol::DISPLAY_LINE* DebugLogging::get_PtrElementLine() {
  return log_frame_.mutable_gl_element()->add_line_set();
}
atd::protocol::DISPLAY_PLOYNOMIAL* DebugLogging::get_PtrElementPoly() {
  return log_frame_.mutable_gl_element()->add_ploy_set();
}

atd::protocol::FLOAT_VAR* DebugLogging::get_PtrElementDisplay_As_Float() {
  return log_frame_.mutable_display_element()->add_float_vars();
}
atd::protocol::INT_VAR* DebugLogging::get_PtrElementDisplay_As_Int() {
  return log_frame_.mutable_display_element()->add_int_vars();
}
atd::protocol::UINT_VAR* DebugLogging::get_PtrElementDisplay_As_UInt() {
  return log_frame_.mutable_display_element()->add_uint_vars();
}
atd::protocol::NORMAL_VAR* DebugLogging::get_PtrElementDisplay_As_String() {
  return log_frame_.mutable_display_element()->add_normal_vars();
}

std::pair<float, bool> DebugLogging::get_FloatCalibVariables(
    const std::string& name) {
  auto res =
      calib_repository_.get_RegisteredVar<CalibrationVariable<float>>(name);
  if (!res.second) {
    return {0.0, false};
  }
  return {res.first->get_Var(), true};
}
std::pair<int, bool> DebugLogging::get_IntCalibVariables(
    const std::string& name) {
  auto res =
      calib_repository_.get_RegisteredVar<CalibrationVariable<int>>(name);
  if (!res.second) {
    return {0.0, false};
  }
  return {res.first->get_Var(), true};
}
std::pair<uint32_t, bool> DebugLogging::get_UintCalibVariables(
    const std::string& name) {
  auto res =
      calib_repository_.get_RegisteredVar<CalibrationVariable<uint32_t>>(name);
  if (!res.second) {
    return {0.0, false};
  }
  return {res.first->get_Var(), true};
}

void DebugLogging::publish_Frame() {
  for (auto pair4loop : calib_repository_) {
    auto ptr_var = log_frame_.mutable_calibrations();
    if (pair4loop.second.type_name_ ==
        typeid(CalibrationVariable<float>).name()) {
      auto ptr_float = ptr_var->add_calib_float();
      auto calib_float =
          calib_repository_.get_RegisteredVar<CalibrationVariable<float>>(
              pair4loop.first);
      ptr_float->set_name(pair4loop.first);
      ptr_float->set_data_init(calib_float.first->get_Init());
      ptr_float->set_data(calib_float.first->get_Var());
      ptr_float->set_data_lower_bound(calib_float.first->get_Min());
      ptr_float->set_data_upper_bound(calib_float.first->get_Max());
    }
    if (pair4loop.second.type_name_ ==
        typeid(CalibrationVariable<int>).name()) {
      auto ptr_int = ptr_var->add_calib_int();
      auto calib_int =
          calib_repository_.get_RegisteredVar<CalibrationVariable<int>>(
              pair4loop.first);
      ptr_int->set_name(pair4loop.first);
      ptr_int->set_data_init(calib_int.first->get_Init());
      ptr_int->set_data(calib_int.first->get_Var());
      ptr_int->set_data_lower_bound(calib_int.first->get_Min());
      ptr_int->set_data_upper_bound(calib_int.first->get_Max());
    }
    if (pair4loop.second.type_name_ ==
        typeid(CalibrationVariable<uint32_t>).name()) {
      auto ptr_uint = ptr_var->add_calib_uint();
      auto calib_uint =
          calib_repository_.get_RegisteredVar<CalibrationVariable<uint32_t>>(
              pair4loop.first);
      ptr_uint->set_name(pair4loop.first);
      ptr_uint->set_data_init(calib_uint.first->get_Init());
      ptr_uint->set_data(calib_uint.first->get_Var());
      ptr_uint->set_data_lower_bound(calib_uint.first->get_Min());
      ptr_uint->set_data_upper_bound(calib_uint.first->get_Max());
    }
  }
  log_publisher_.publish(log_frame_);
}

Writer::Writer(const char* file, uint32_t line_no, SECURITY_INFO level) {
  init();
  uint32_t name_length = strlen(file);
  uint32_t index = name_length - 1;
  for (; index > 0; index--) {
    if ((file[index] == '/') && (index != (name_length - 1))) {
      break;
    }
  }
  std::string file_name(file, index + 1, strlen(file) - index);
  ptr_content_->set_file_name(file_name);
  ptr_content_->set_line_no(line_no);
  switch (level) {
    case INFO:
      ptr_content_->set_level(atd::protocol::INFO);
      break;
    case WARNING:
      ptr_content_->set_level(atd::protocol::WARNING);
      break;
    case ERROR:
      ptr_content_->set_level(atd::protocol::ERROR);
      break;
    default:
      break;
  }
}

Writer& Writer::construct() { return *this; }

void Writer::init() {
  ptr_content_ =
      atd::utility::Singleton::instance<DebugLogging>()->get_PtrFrame();
}

}  // namespace utility
}  // namespace atd