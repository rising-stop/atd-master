#include "data_analyzer.h"


void DataAnalyzer::render() {
  ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
  if (ImGui::BeginTabBar("MonitorTabBar", tab_bar_flags)) {
    for (auto& monitor : monitor_manager_) {
      if (ImGui::BeginTabItem(monitor.first.c_str())) {
        monitor.second->render();
        ImGui::EndTabItem();
      }
    }
    ImGui::EndTabBar();
  }
}
