#pragma once

#include <deque>
#include <map>
#include <set>
#include <vector>

#include "data_dispatcher.hpp"
#include "modules/neo_adp/imgui-opengl3/imgui_impl_glfw.h"
#include "modules/neo_adp/imgui-opengl3/imgui_impl_opengl3.h"
#include "modules/neo_adp/imgui_module/imgui.h"

constexpr int monitor_max_buffer = 600; /* time_buffer  (s) */

class DataMonitor {
 public:
  void Render() {
    ImGui::SliderInt("Sample range", &sample_range, 10, 600);
    if (ImGui::Button("Data Set")) ImGui::OpenPopup("toggle_popup");
    if (ImGui::BeginPopup("my_toggle_popup")) {
      update_frame();
      ImGui::EndPopup();
    }

    if (!line_data_.size()) {
      return;
    }

    std::vector<ImU32> line_colors;
    std::vector<std::string> overlay_text;

    int line_index = 0;
    for (auto line : line_data_) {
      ImVec4 line_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
      ImGui::ColorEdit4(line.first.c_str(), (float*)&line_color);
      line_colors.push_back(ImGui::ColorConvertFloat4ToU32(line_color));
      overlay_text.push_back(line.first);
    }
    ImGui::MulitPlot(
        "",
        [&](const std::string name, int idx) -> float {
          return line_data_[name].at(idx);
        },
        monitor_max_buffer, 0, line_colors, overlay_text, -1.0f, 1.0f,
        ImVec2(0, 80));
  }

 private:
  void update_frame() {
    std::map<std::string, std::string> frame;
    if (!atd::utility::Singleton::instance<DataDispatcher>()->get_LatestFrame(
            frame)) {
      return;
    }

    menu_content_.clear();
    for (auto item_pos : frame) {
      menu_content_.insert({item_pos.first, false});
    }

    std::set<std::string> selected_set;
    for (auto& menu_item : menu_content_) {
      ImGui::MenuItem(menu_item.first.c_str(), "", &(menu_item.second));
      if (menu_item.second) {
        selected_set.insert(menu_item.first);
      }
    }

    auto itr_data = line_data_.begin();
    while (itr_data != line_data_.end()) {
      if (selected_set.find(itr_data->first) == selected_set.end()) {
        itr_data = line_data_.erase(itr_data);
      } else {
        itr_data++;
      }
    }

    for (auto list_name : selected_set) {
      auto itr_name = line_data_.find(list_name);
      if (itr_name == line_data_.end()) {
        line_data_.insert(
            {list_name, std::deque<float>(monitor_max_buffer, 0.0f)});
      } else {
        line_data_[list_name].pop_front();
        line_data_[list_name].push_back(std::stof(frame[list_name]));
      }
    }
  }

 private:
  int sample_range = 0;
  std::map<std::string, bool> menu_content_;
  std::map<std::string, std::deque<float>> line_data_;

 public:
  DataMonitor() {}
  ~DataMonitor() {}
};
