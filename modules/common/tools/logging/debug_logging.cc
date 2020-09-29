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
  for (auto& var4loop : cal_var_.variables()) {
    auto itr = calib_container_.find(var4loop.name());
    if (itr == calib_container_.end()) {
      continue;
    }
    itr->second.var_ = std::stof(var4loop.data().c_str());
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
atd::protocol::VARIABLE* DebugLogging::get_PtrElementDisplay() {
  return log_frame_.mutable_display_element()->add_content();
}

std::pair<float, bool> DebugLogging::get_CalibVariables(
    const std::string& name) {
  auto itr_target = calib_container_.find(name);
  if (itr_target == calib_container_.end()) {
    return {0.0f, false};
  }
  return {itr_target->second.var_, true};
}

void DebugLogging::try_register_Calibration(const std::string& name, float& var,
                                            float max, float min,
                                            float init_var) {
  if (max < min) {
    CUSTOM_EXCEPTION("calibration variable max is lower than min");
  }
  auto ins_res =
      calib_container_.insert({name, {name, var, max, min, init_var}});
  if (!ins_res.second) {
    CUSTOM_EXCEPTION("calibration variable ", name, " has been registered");
  }
  ins_res.first->second.var_ = std::min(std::max(min, init_var), max);
  ins_res.first->second.max_ = max;
  ins_res.first->second.min_ = min;
  ins_res.first->second.init_ = init_var;
}

void DebugLogging::publish_Frame() {
  for (auto pair4loop : calib_container_) {
    auto ptr_var = log_frame_.mutable_variables()->add_variables();
    ptr_var->set_name(pair4loop.second.name_);
    ptr_var->set_data(std::to_string(pair4loop.second.var_));
    ptr_var->set_data_upper_bound(std::to_string(pair4loop.second.max_));
    ptr_var->set_data_lower_bound(std::to_string(pair4loop.second.min_));
    ptr_var->set_data_init(std::to_string(pair4loop.second.init_));
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