#include "planning_log_parser.h"

#include <iostream>

namespace atd {
namespace utility {

PlanningLog::PlanningLog(FILE_MODE mode, const char* name, const char* path)
    : ReadWriteableFile(mode, name, path) {}

void PlanningLog::Log_Parse(const std::string& log_name) {
  log_name_ = log_name;
  parse_file();
  std::cout << "Parse Done" << std::endl;
}

void PlanningLog::parse_file() {
  uint64_t file_length = 0;
  atd::protocol::LOG_CONTENT single_frame;

  lcm::LogFile lcm_log(log_name_, "r");
  if (!lcm_log.good()) {
    CUSTOM_EXCEPTION("can not open file log %s", log_name_.c_str());
  }
  while (1) {
    const lcm::LogEvent* event = lcm_log.readNextEvent();

    if (!event) break;

    if (event->channel != channel_name_) continue;

    if (single_frame.ParseFromArray(event->data, event->datalen)) {
      file_length += event->datalen;
      log_record(single_frame);
    } else {
      continue;
    }
  }
  if (!file_length) {
    // throw LCMException(LCMException::MSG_NOT_FOUND, "");
    CUSTOM_EXCEPTION("can not found LCM channel %s", channel_name_.c_str());
  }
}

void PlanningLog::log_record(const atd::protocol::LOG_CONTENT& frame) {
  auto& ptr_file_stream = get_FileStream();

  ptr_file_stream << "########### "
                  << "FRAME NO. " << frame.title().counter_no()
                  << ", TIME STAMP " << frame.title().time_stamp()
                  << " ###########"
                  << "\n";
  for (uint32_t index = 0; index < frame.content_size(); index++) {
    switch (frame.content(index).level()) {
      case atd::protocol::SECURITY_INFO::INFO:
        ptr_file_stream << "> level: INFO, ";
        break;
      case atd::protocol::SECURITY_INFO::WARNING:
        ptr_file_stream << "> level: WARNING, ";
        break;
      case atd::protocol::SECURITY_INFO::ERROR:
        ptr_file_stream << "> level: ERROR, ";
        break;
      default:
        ptr_file_stream << "> level: UNKNOW, ";
        break;
    }
    ptr_file_stream << frame.content(index).file_name() << " no. "
                    << frame.content(index).line_no() << ":"
                    << "\n";
    for (uint32_t str_index = 0;
         str_index < frame.content(index).str_msg_size(); str_index++) {
      ptr_file_stream << "   > " << frame.content(index).str_msg(str_index)
                      << "\n";
    }
    for (uint32_t var_index = 0;
         var_index < frame.content(index).variables_size(); var_index++) {
      ptr_file_stream << "   > "
                      << frame.content(index).variables(var_index).name()
                      << " = "
                      << frame.content(index).variables(var_index).data()
                      << "\n";
    }
  }
  return;
}

void PlanningLog::refresh_file() {}

}  // namespace utility
}  // namespace atd