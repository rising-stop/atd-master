#include "log_monitor.h"

#include "modules/common/common_header.h"

void LogMonitor::render() {
  if (enable_) {
    std::string new_log_frame;
    atd::protocol::FRAME_HEADER header_msg;
    atd::protocol::LOG_CONTENT log_msg;
    if (atd::utility::Singleton::instance<DataDispatcher>()->get_LatestHeader(
            header_msg)) {
      if (atd::utility::Singleton::instance<DataDispatcher>()
              ->get_LatestLogInfo(log_msg)) {
        parse_LogContent(header_msg, log_msg, new_log_frame);
        push_LogMessage(new_log_frame);
      }
    }
  }

  HelpMarker("Log Info from Planning Module");

  ImGui::Checkbox("Enable Output Text", &enable_);
  ImGui::ColorEdit3("clear color", (float*)&text_color_);

  {
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar;
    ImGui::BeginChild("ChildL",
                      ImVec2(ImGui::GetWindowContentRegionWidth() * 0.9f,
                             ImGui::GetWindowContentRegionHeight() * 0.9f),
                      false, window_flags);
    for (const auto& log4loop : log_quene_) {
      ImGui::TextColored(text_color_, log4loop.c_str());
    }

    ImGui::EndChild();
  }
}

void LogMonitor::push_LogMessage(const std::string& log) {
  while (log_quene_.size() >= LogMonitor_BufferSize) {
    log_quene_.pop_front();
  }
  log_quene_.push_back(log);
}

void LogMonitor::parse_LogContent(const atd::protocol::FRAME_HEADER& header,
                                  const atd::protocol::LOG_CONTENT& log_msg,
                                  std::string& log) const {
  static int last_timestamp = 0;
  std::stringstream sstm;
  sstm << "########### "
       << "FRAME NO. " << header.counter_no() << ", TIME STAMP "
       << header.time_stamp() << ", ELAPSE "
       << header.time_stamp() - last_timestamp << "ms ###########"
       << "\n";
  for (uint32_t index = 0; index < log_msg.content_size(); index++) {
    switch (log_msg.content(index).level()) {
      case atd::protocol::SECURITY_INFO::INFO:
        sstm << "> [INFO] > ";
        break;
      case atd::protocol::SECURITY_INFO::WARNING:
        sstm << "> [WARNING] > ";
        break;
      case atd::protocol::SECURITY_INFO::ERROR:
        sstm << "> [ERROR] > ";
        break;
      default:
        sstm << "> [UNKNOW] > ";
        break;
    }
    sstm << log_msg.content(index).file_name() << " no. "
         << log_msg.content(index).line_no() << ":"
         << "\n";
    for (uint32_t str_index = 0;
         str_index < log_msg.content(index).str_msg_size(); str_index++) {
      sstm << "   > " << log_msg.content(index).str_msg(str_index) << "\n";
    }
    for (uint32_t var_index = 0;
         var_index < log_msg.content(index).float_vars_size(); var_index++) {
      sstm << "   > " << log_msg.content(index).float_vars(var_index).name()
           << " = "
           << atd::utility::CString::cstring_cat(
                  "%.3f", log_msg.content(index).float_vars(var_index).data())
           << "\n";
    }
    for (uint32_t var_index = 0;
         var_index < log_msg.content(index).int_vars_size(); var_index++) {
      sstm << "   > " << log_msg.content(index).int_vars(var_index).name()
           << " = "
           << atd::utility::CString::cstring_cat(
                  "%d", log_msg.content(index).int_vars(var_index).data())
           << "\n";
    }
    for (uint32_t var_index = 0;
         var_index < log_msg.content(index).uint_vars_size(); var_index++) {
      sstm << "   > " << log_msg.content(index).uint_vars(var_index).name()
           << " = "
           << atd::utility::CString::cstring_cat(
                  "%u", log_msg.content(index).uint_vars(var_index).data())
           << "\n";
    }
    for (uint32_t var_index = 0;
         var_index < log_msg.content(index).normal_vars_size(); var_index++) {
      sstm << "   > " << log_msg.content(index).normal_vars(var_index).name()
           << " = " << log_msg.content(index).normal_vars(var_index).data()
           << "\n";
    }
    last_timestamp = header.time_stamp();
  }

  log.clear();
  log = sstm.str();
}