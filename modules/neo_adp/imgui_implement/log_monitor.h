#pragma once

#include "imgui_basic_component.h"
#include "modules/neo_adp/data_service/data_dispatcher.h"

class LogMonitor : public ImGui_Components {
 public:
  virtual void render() override;

 private:
  void push_LogMessage(const std::string& log);

  void parse_LogContent(const atd::protocol::FRAME_HEADER& header,
                        const atd::protocol::LOG_CONTENT& log_msg,
                        std::string& log) const;

  bool enable_ = true;
  ImVec4 text_color_ =
      ImVec4(24.0f / 255.0f, 240.0f / 255.0f, 60.0f / 255.0f, 1.00f);

  std::deque<std::string> log_quene_;

 public:
  LogMonitor() = default;
  ~LogMonitor() = default;
};
