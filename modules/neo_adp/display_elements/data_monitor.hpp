#pragma once

#include <deque>
#include <iostream>
#include <map>
#include <set>
#include <vector>

#include "data_dispatcher.hpp"
#include "modules/neo_adp/imgui-opengl3/imgui_impl_glfw.h"
#include "modules/neo_adp/imgui-opengl3/imgui_impl_opengl3.h"
#include "modules/neo_adp/imgui_module/imgui.h"

constexpr int monitor_max_buffer = 600; /* time_buffer  (s) */
constexpr int monitor_min_buffer = 10;

class DataMonitor {
 public:
  void Render() {
    std::string slider_name{"Range: "};
    slider_name.append(name_);
    ImGui::SliderInt(slider_name.c_str(), &sample_range, monitor_min_buffer,
                     monitor_max_buffer);
    std::string botton_name{"Sheet: "};
    botton_name.append(name_);

    if (ImGui::Button(botton_name.c_str())) ImGui::OpenPopup(name_.c_str());

    ImGui::SameLine();
    std::string check_box_name{"Enable Sheet "};
    check_box_name.append(name_);
    ImGui::Checkbox(check_box_name.c_str(), &enable_);
    if (enable_) {
      update_frame();
    }
    if (ImGui::BeginPopup(name_.c_str())) {
      for (auto& menu_item : menu_content_)
        ImGui::MenuItem(menu_item.first.c_str(), "",
                        &(menu_item.second.enable));
      ImGui::EndPopup();
    }

    std::vector<ImU32> line_colors;
    std::vector<std::string> overlay_text;

    int line_index = 0;
    for (auto& line : menu_content_) {
      if (!line.second.enable) {
        continue;
      }
      ImGui::ColorEdit4(line.first.c_str(), (float*)&line.second.line_color);
      line_colors.push_back(
          ImGui::ColorConvertFloat4ToU32(line.second.line_color));
      overlay_text.push_back(line.first);
    }
    ImGui::MulitPlot(
        name_.c_str(),
        [&](const std::string name, int idx) -> float {
          int data_size = menu_content_[name].data.size();
          return menu_content_[name].data.at(data_size - sample_range + idx);
        },
        sample_range, 0, line_colors, overlay_text, -1.0f, 1.0f,
        ImVec2(0, 160));
    ImGui::Separator();
  }

 private:
  void update_frame() {
    if (!atd::utility::Singleton::instance<DataDispatcher>()->get_LatestFrame(
            frame_)) {
    }

    for (auto item_pos : frame_) {
      menu_content_.insert({item_pos.first, line_frame()});
    }

    std::set<std::string> selected_set;
    for (auto& menu_item : menu_content_) {
      if (menu_item.second.enable) {
        selected_set.insert(menu_item.first);
      }
    }

    auto itr_data = menu_content_.begin();
    while (itr_data != menu_content_.end()) {
      if (frame_.find(itr_data->first) == frame_.end()) {
        itr_data = menu_content_.erase(itr_data);
      } else {
        itr_data++;
      }
    }

    for (auto& line_data : menu_content_) {
      line_data.second.data.pop_front();
      try {
        line_data.second.data.push_back(std::stof(frame_[line_data.first]));
      } catch (...) {
        line_data.second.data.push_back(0.0f);
        std::cerr << "update_frame(): stof throw a exception" << std::endl;
      }
    }
  }

 public:
  const std::string name_;

 private:
  bool enable_ = true;
  int sample_range = monitor_max_buffer;
  struct line_frame {
    bool enable = false;
    std::deque<float> data = std::deque<float>(monitor_max_buffer, 0.0f);
    ImVec4 line_color{1.0f, 1.0f, 1.0f, 1.00f};
  };
  std::map<std::string, std::string> frame_;
  std::map<std::string, line_frame> menu_content_;

 public:
  DataMonitor(const std::string& id) : name_(id) {}
  ~DataMonitor() {}
};
