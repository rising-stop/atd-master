#include "log_monitor.h"

#include "modules/common/common_header.h"

void LogMonitor::render() {
  if (enable_) {
    atd::protocol::FRAME_HEADER header_msg;
    atd::protocol::LOG_CONTENT log_msg;
    if (atd::utility::Singleton::instance<DataDispatcher>()->get_LatestHeader(
            header_msg)) {
      if (atd::utility::Singleton::instance<DataDispatcher>()
              ->get_LatestLogInfo(log_msg)) {
        parse_LogContent(header_msg, log_msg);
      }
    }
  }

  HelpMarker("Log Info from Planning Module");

  ImGui::Checkbox("Enable Output Text", &enable_);
  ImGui::ColorEdit3("clear color", (float*)&text_color_);

  // {
  //   ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar;
  //   ImGui::BeginChild("ChildL",
  //                     ImVec2(ImGui::GetWindowContentRegionWidth() * 0.9f,
  //                            ImGui::GetWindowContentRegionHeight() * 0.9f),
  //                     false, window_flags);
  //   for (const auto& log4loop : log_quene_) {
  //     ImGui::TextColored(text_color_, log4loop.c_str());
  //   }

  ImGui::EndChild();
}

void LogMonitor::parse_LogContent(const atd::protocol::FRAME_HEADER& header,
                                  const atd::protocol::LOG_CONTENT& log_msg) {
  static int last_timestamp = 0;
  std::stringstream sstm;
  sstm << "########### "
       << "FRAME NO. " << header.counter_no() << ", TIME STAMP "
       << header.time_stamp() << ", ELAPSE "
       << header.time_stamp() - last_timestamp << "ms ###########"
       << "\n";
  AddLog(sstm.str().c_str());
  sstm.clear();
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
    AddLog(sstm.str().c_str());
    sstm.clear();
  }
}

void LogMonitor::Clear() {
  Buf_.clear();
  LineOffsets_.clear();
  LineOffsets_.push_back(0);
}

void LogMonitor::AddLog(const char* fmt, ...) {
  int old_size = Buf_.size();
  va_list args;
  va_start(args, fmt);
  Buf_.appendfv(fmt, args);
  va_end(args);
  for (int new_size = Buf_.size(); old_size < new_size; old_size++)
    if (Buf_[old_size] == '\n') LineOffsets_.push_back(old_size + 1);
}

void LogMonitor::Draw(const char* title) {
  if (!ImGui::Begin(title)) {
    ImGui::End();
    return;
  }

  // Options menu
  if (ImGui::BeginPopup("Options")) {
    ImGui::Checkbox("Auto-scroll", &AutoScroll_);
    ImGui::EndPopup();
  }

  // Main window
  if (ImGui::Button("Options")) ImGui::OpenPopup("Options");
  ImGui::SameLine();
  bool clear = ImGui::Button("Clear");
  ImGui::SameLine();
  bool copy = ImGui::Button("Copy");
  ImGui::SameLine();
  Filter_.Draw("Filter", -100.0f);

  ImGui::Separator();
  ImGui::BeginChild("scrolling", ImVec2(0, 0), false,
                    ImGuiWindowFlags_HorizontalScrollbar);

  if (clear) Clear();
  if (copy) ImGui::LogToClipboard();

  ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
  ImGui::PushStyleColor(ImGuiCol_Text, text_color_);
  const char* buf = Buf_.begin();
  const char* buf_end = Buf_.end();
  if (Filter_.IsActive()) {
    // In this example we don't use the clipper when Filter is enabled.
    // This is because we don't have a random access on the result on our
    // filter. A real application processing logs with ten of thousands of
    // entries may want to store the result of search/filter.. especially if the
    // filtering function is not trivial (e.g. reg-exp).
    for (int line_no = 0; line_no < LineOffsets_.Size; line_no++) {
      const char* line_start = buf + LineOffsets_[line_no];
      const char* line_end = (line_no + 1 < LineOffsets_.Size)
                                 ? (buf + LineOffsets_[line_no + 1] - 1)
                                 : buf_end;
      if (Filter_.PassFilter(line_start, line_end))
        ImGui::TextUnformatted(line_start, line_end);
    }
  } else {
    // The simplest and easy way to display the entire buffer:
    //   ImGui::TextUnformatted(buf_begin, buf_end);
    // And it'll just work. TextUnformatted() has specialization for large blob
    // of text and will fast-forward to skip non-visible lines. Here we instead
    // demonstrate using the clipper to only process lines that are within the
    // visible area. If you have tens of thousands of items and their processing
    // cost is non-negligible, coarse clipping them on your side is recommended.
    // Using ImGuiListClipper requires
    // - A) random access into your data
    // - B) items all being the  same height,
    // both of which we can handle since we an array pointing to the beginning
    // of each line of text. When using the filter (in the block of code above)
    // we don't have random access into the data to display anymore, which is
    // why we don't use the clipper. Storing or skimming through the search
    // result would make it possible (and would be recommended if you want to
    // search through tens of thousands of entries).
    ImGuiListClipper clipper;
    clipper.Begin(LineOffsets_.Size);
    while (clipper.Step()) {
      for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd;
           line_no++) {
        const char* line_start = buf + LineOffsets_[line_no];
        const char* line_end = (line_no + 1 < LineOffsets_.Size)
                                   ? (buf + LineOffsets_[line_no + 1] - 1)
                                   : buf_end;
        ImGui::TextUnformatted(line_start, line_end);
      }
    }
    clipper.End();
  }

  ImGui::PopStyleColor();
  ImGui::PopStyleVar();

  if (AutoScroll_ && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
    ImGui::SetScrollHereY(1.0f);

  ImGui::EndChild();
  ImGui::End();
}
