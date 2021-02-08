#pragma once

#include "imgui_basic_component.h"
#include "modules/neo_adp/data_service/data_seg4lcm_protocol.h"

class LogMonitor : public ImGui_Components {
 public:
  virtual void render() override;

 private:
  void parse_LogContent(const atd::protocol::FRAME_HEADER& header,
                        const atd::protocol::LOG_CONTENT& log_msg);

  bool enable_ = true;
  ImVec4 text_color_ =
      ImVec4(24.0f / 255.0f, 240.0f / 255.0f, 60.0f / 255.0f, 1.00f);

 private:
  void Clear();

  void AddLog(const char* fmt, ...);

  void Draw();

  ImGuiTextBuffer Buf_;
  ImGuiTextFilter Filter_;
  ImVector<int> LineOffsets_;  // Index to lines offset. We maintain this with
                               // AddLog() calls.
  bool AutoScroll_ = true;     // Keep scrolling if already at the bottom.
  uint64_t last_frame_time_ = 0;

 public:
  LogMonitor() = default;
  ~LogMonitor() = default;
};
