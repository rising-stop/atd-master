#include "data_monitor.h"

#include "modules/common/common_header.h"

void DataObserver::Render() override {
  std::string range_slider_name{"Range: "};
  range_slider_name.append(id_);
  ImGui::SliderInt(range_slider_name.c_str(), &sample_range_,
                   DataMonitor_Min_BufferSize, DataMonitor_Max_BufferSize);
  std::string focus_slider_name{"Focus: "};
  focus_slider_name.append(id_);
  ImGui::SliderInt(focus_slider_name.c_str(), &sample_focus_, sample_range_,
                   DataMonitor_Max_BufferSize);

  std::string botton_name{"Sheet: "};
  botton_name.append(id_);

  if (ImGui::Button(botton_name.c_str())) ImGui::OpenPopup(id_.c_str());

  if (ImGui::BeginPopup(name_.c_str())) {
    menu_handler();
    ImGui::EndPopup();
  }
  plot_render();
  ImGui::Separator();
}

void DataObserver::menu_handler() {
  for (const auto& signal : data_list_) {
    if (ob_list_.insert(std::make_pair(signal.first, menu_info())).second) {
      register_signal(signal.first);
    }
  }

  for (auto& menu_item : ob_list_) {
    bool detach_detect = menu_item.second.flag_activate_;
    ImGui::MenuItem(menu_item.first.c_str(), "",
                    &(menu_item.second.flag_activate_));
    if (detach_detect && !menu_item.second.flag_activate_) {
      unregister_signal(signal.first);
    }
    if (data_list_.find(menu_item.first) == data_list_.cend()) {
      menu_item.second.info_ = "offline";
    } else {
      menu_item.second.info_ =
          std::to_string(data_list_.at(menu_item.first).data.back());
    }
  }
}

void DataObserver::plot_render() {
  std::vector<ImU32> line_colors;
  std::vector<std::string> overlay_text;

  float v_min = FLT_MAX;
  float v_max = -FLT_MAX;

  for (auto& signal : ob_list_) {
    if (!signal.second.flag_activate_) {
      continue;
    }
    ImGui::ColorEdit4(signal.first.c_str(),
                      (float*)&color_dispatcher_[signal.second]);
    ImGui::SameLine();
    ImGui::Text(signal.second.info_);
    line_colors.push_back(
        ImGui::ColorConvertFloat4ToU32(color_dispatcher_.at(signal.first)));
    overlay_text.push_back(signal.first);
    v_min = std::fmin(v_min, data_list_[signal.first].lower_bound);
    v_max = std::fmax(v_max, data_list_[signal.first].upper_bound);
  }
  ImGui::MulitPlot(
      name_.c_str(),
      [&](const std::string name, int idx) -> float {
        return data_list_[name].data.at(DataMonitor_Max_BufferSize -
                                        sample_range_ + idx);
      },
      sample_range_, 0, line_colors, overlay_text, v_min, v_max,
      ImVec2(0, 160));
}

const int DataObserver::get_ID() { return id_; }

bool DataObserver::register_signal(const string& id) {
  if (signal_counter_ < default_color_set_num) {
    ImVec4 line_color = default_color_list[signal_counter_];
    if (!color_dispatcher_.insert(std::make_pair(id, line_color)).second) {
      return false;
    }
  } else {
    ImVec4 line_color = {TIMER->get_Rand(), TIMER->get_Rand(),
                         TIMER->get_Rand(), 0.7f};
    if (!color_dispatcher_.insert(std::make_pair(id, line_color)).second) {
      return false;
    }
  }
  signal_counter_++;
  return true;
}

bool DataObserver::unregister_signal(const string& id) {
  auto color_id = color_dispatcher_.find(id);
  if (color_id == color_dispatcher_.cend()) {
    return false;
  }
  color_dispatcher_.erase(color_id);
  return true;
}

DataObserver::DataObserver(const int id,
                           const std::map<std::string, line_frame>& data)
    : id_(id), data_list_(data) {}

/**
 * ********************************************
 * @def  Class DataMonitor
 * ********************************************
 */

void DataMonitor::Render() {
  if (ImGui::Button("Monitor - Add on")) {
    observer_set_.push_back(
        new DataObserver(++monitor_counter_, data_repository_));
  }
  ImGui::SameLine();
  ImGui::Checkbox("Enable All", &enable_all_);

  ImGui::Separator();

  if (enable_all) {
    update_data_base();
  }

  for (auto single_monitor : observer_set_) {
    single_monitor->Render();
  }

  if (ImGui::Button("Monitor - Remove")) {
    if (observer_set_.size()) {
      delete observer_set_.back();
      observer_set_.pop_back();
    }
  }
}

bool DataMonitor::update_data_base() {
  std::map<std::string, float> frame_data;
  if (!atd::utility::Singleton::instance<DataDispatcher>()
           ->get_DataMonitor_LatestFrame(frame_data)) {
    return false;
  }
  for (auto signal : frame_data) {
    auto res_ins =
        data_repository_.insert(std::make_pair(signal.first, line_frame()));
    data_repository[signal.first].data.pop_front();
    data_repository[signal.first].data.push_back(signal.second);
    data_repository[signal.first].upper_bound =
        std::fmax(data_repository[signal.first].upper_bound, signal.second);
    data_repository[signal.first].lower_bound =
        std::fmin(data_repository[signal.first].lower_bound, signal.second);
  }
  return true;
}
