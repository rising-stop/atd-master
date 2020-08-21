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
}

void DebugLogging::publish_Frame() { log_publisher_.publish(log_frame_); }

atd::protocol::FRAME_CONTENT* DebugLogging::get_PtrFrame() {
  return log_frame_.add_content();
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