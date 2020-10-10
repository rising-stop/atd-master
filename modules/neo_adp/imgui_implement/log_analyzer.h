#pragma once

#include "imgui_basic_component.h"
#include "modules/neo_adp/data_service/data_dispatcher.h"

class LogAnalyzer : public ImGui_Components {
 public:
  virtual void render() override;

 private:
 public:
  LogAnalyzer() = default;
  ~LogAnalyzer() = default;
};