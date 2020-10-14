#pragma once

#include "data_monitor.h"
#include "file_interface.h"
#include "imgui_basic_component.h"
#include "modules/neo_adp/data_service/log_file_handler.h"

class LogAnalyzer : public ImGui_Components {
 public:
  virtual void render() override;

 private:

 public:
  LogAnalyzer() = default;
  ~LogAnalyzer() = default;
};