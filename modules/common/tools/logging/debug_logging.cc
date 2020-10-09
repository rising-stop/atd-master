#include "debug_logging.h"

namespace atd {
namespace utility {

DebugLogging::DebugLogging() { TIMER->set_BeginStick("mile_stone"); }

void DebugLogging::reset_Frame() {
  TIMER->wait_and_set_EndStick("mile_stone");
  log_frame_.Clear();
  log_frame_.mutable_title()->set_time_stamp(
      TIMER->get_TimeGap("mile_stone").first);
  log_frame_.mutable_title()->set_counter_no(COUNTER("planning_log"));

  calib_listener_.subscribe(cal_var_);
  for (const auto& calib4loop : cal_var_.calib_float()) {
    calib_repository_.set_calib(calib4loop.name(), calib4loop.data());
  }
  for (const auto& calib4loop : cal_var_.calib_int()) {
    calib_repository_.set_calib(calib4loop.name(), calib4loop.data());
  }
  for (const auto& calib4loop : cal_var_.calib_uint()) {
    calib_repository_.set_calib(calib4loop.name(), calib4loop.data());
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

void DebugLogging::publish_Frame() {
  std::vector<std::pair<std::string, std::string>> calib_list;
  calib_repository_.get_RegisteredCalibSet(calib_list);
  for (auto name4loop : calib_list) {
    if (name4loop.second == typeid(float).name()) {
      auto ptr_var = log_frame_.mutable_calibrations()->add_calib_float();
      auto find_res =
          calib_repository_.get_RegisteredCalib<float>(name4loop.first);
      if (!find_res.second) {
        continue;
      }
      ptr_var->set_name(name4loop.first);
      ptr_var->set_data(find_res.first->get_Var());
      ptr_var->set_data_upper_bound(find_res.first->get_Max());
      ptr_var->set_data_lower_bound(find_res.first->get_Min());
      ptr_var->set_data_init(find_res.first->get_Init());
    } else if (name4loop.second == typeid(int).name()) {
      auto ptr_var = log_frame_.mutable_calibrations()->add_calib_int();
      auto find_res =
          calib_repository_.get_RegisteredCalib<int>(name4loop.first);
      if (!find_res.second) {
        continue;
      }
      ptr_var->set_name(name4loop.first);
      ptr_var->set_data(find_res.first->get_Var());
      ptr_var->set_data_upper_bound(find_res.first->get_Max());
      ptr_var->set_data_lower_bound(find_res.first->get_Min());
      ptr_var->set_data_init(find_res.first->get_Init());
    } else if (name4loop.second == typeid(uint32_t).name()) {
      auto ptr_var = log_frame_.mutable_calibrations()->add_calib_uint();
      auto find_res =
          calib_repository_.get_RegisteredCalib<uint32_t>(name4loop.first);
      if (!find_res.second) {
        continue;
      }
      ptr_var->set_name(name4loop.first);
      ptr_var->set_data(find_res.first->get_Var());
      ptr_var->set_data_upper_bound(find_res.first->get_Max());
      ptr_var->set_data_lower_bound(find_res.first->get_Min());
      ptr_var->set_data_init(find_res.first->get_Init());
    } else {
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
      atd::utility::Singleton::instance<DebugLogging>()->get_PtrLogFrame();
}

}  // namespace utility
}  // namespace atd