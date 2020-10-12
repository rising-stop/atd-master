#include "data_monitor.h"

#include "modules/common/common_header.h"

const float
    DataObserver::default_color_list[DataObserver::default_color_set_num][4] = {
        {230.0f / 255.0f, 155.0f / 255.0f, 3.0f / 255.0f, 0.7f},
        {131.0f / 255.0f, 175.0f / 255.0f, 155.0f / 255.0f, 0.7f},
        {252.0f / 255.0f, 157.0f / 255.0f, 154.0f / 255.0f, 0.7f},
        {92.0f / 255.0f, 167.0f / 255.0f, 186.0f / 255.0f, 0.7f},
        {175.0f / 255.0f, 215.0f / 255.0f, 237.0f / 255.0f, 0.7f}};

void DataObserver::render() {
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

  if (ImGui::BeginPopup(id_.c_str())) {
    menu_handler();
    ImGui::EndPopup();
  }
  plot_render();
  ImGui::Separator();
}

void DataObserver::set_DisplayHeader(uint32_t header) {
  if (header < 0) {
    CUSTOM_EXCEPTION(
        "data monitor setting exception detected, setting header is lower than "
        "zero");
  }
  header_ = header;
}

void DataObserver::set_DisplayTailer(uint32_t tailer) {
  if (tailer < DataMonitor_Min_BufferSize) {
    CUSTOM_EXCEPTION(
        "data monitor setting exception detected, setting tailer is lower than "
        "minimal buffer size");
  }
  tailer_ = tailer;
}

void DataObserver::menu_handler() {
  for (auto signal : data_list_) {
    if (ob_list_.insert(std::make_pair(std::string(signal.first), menu_info()))
            .second) {
      register_signal(signal.first);
    }
  }

  for (auto& menu_item : ob_list_) {
    bool detach_detect = menu_item.second.flag_activate_;
    ImGui::MenuItem(menu_item.first.c_str(), "",
                    &(menu_item.second.flag_activate_));
    if (detach_detect && !menu_item.second.flag_activate_) {
      unregister_signal(menu_item.first);
    }
    if (data_list_.find(menu_item.first) == data_list_.cend()) {
      menu_item.second.info_ = "offline";
    } else {
      menu_item.second.info_ = atd::utility::CString::cstring_cat(
          "%3.f", data_list_.at(menu_item.first).data_.back());
    }
  }
}

void DataObserver::plot_render() {
  std::vector<ImU32> line_colors(ob_list_.size(), ImU32());
  std::vector<std::string> overlay_text(ob_list_.size(), std::string());

  float v_max = -FLT_MAX;
  float v_min = FLT_MAX;

  for (auto& signal : ob_list_) {
    if (!signal.second.flag_activate_) {
      continue;
    }
    ImGui::ColorEdit4(signal.first.c_str(),
                      (float*)&color_dispatcher_[signal.first]);
    ImGui::SameLine();
    ImGui::Text(signal.second.info_.c_str());
    line_colors.push_back(
        ImGui::ColorConvertFloat4ToU32(color_dispatcher_.at(signal.first)));
    overlay_text.push_back(signal.first);
    v_max = std::fmax(v_max, data_list_.at(signal.first).upper_bound_);
    v_min = std::fmin(v_min, data_list_.at(signal.first).lower_bound_);
  }

  uint32_t sample_range = DataMonitor_Min_BufferSize;
  if (header_ > tailer_) {
    CUSTOM_EXCEPTION("error setting of header and tailer");
  } else {
    if (data_list_.empty()) {
      header_ = 0;
      tailer_ = DataMonitor_Max_BufferSize;
    } else {
      if (tailer_ > data_list_.begin()->second.data_.size()) {
        tailer_ = data_list_.size();
      }
    }
    sample_range = tailer_ - header_;
    if (sample_range < DataMonitor_Min_BufferSize) {
      header_ = tailer_ - DataMonitor_Min_BufferSize;
      sample_range = DataMonitor_Min_BufferSize;
    }
  }

  ImGui::MulitPlot(
      id_,
      [&](const std::string name, int idx) -> float {
        return data_list_.at(name).data_.at(idx);
      },
      sample_range, header_, line_colors, overlay_text, v_min, v_max,
      ImVec2(0, 160));
}

const std::string& DataObserver::get_ID() { return id_; }

bool DataObserver::register_signal(const std::string& id) {
  if (signal_counter_ < default_color_set_num) {
    ImVec4 line_color(default_color_list[signal_counter_][0],
                      default_color_list[signal_counter_][1],
                      default_color_list[signal_counter_][2],
                      default_color_list[signal_counter_][3]);

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

bool DataObserver::unregister_signal(const std::string& id) {
  auto color_id = color_dispatcher_.find(id);
  if (color_id == color_dispatcher_.cend()) {
    return false;
  }
  color_dispatcher_.erase(color_id);
  return true;
}

DataObserver::DataObserver(const std::string& id,
                           const std::map<std::string, line_frame>& data)
    : id_(id), data_list_(data) {}

/**
 * ********************************************
 * @def  Class DataMonitor
 * ********************************************
 */

void DataMonitor::render() {
  std::string range_slider_name{"Range: "};
  range_slider_name.append(id_);
  ImGui::SliderInt(range_slider_name.c_str(), &sample_range_,
                   DataMonitor_Min_BufferSize, DataMonitor_Max_BufferSize);
  std::string focus_slider_name{"Focus: "};
  focus_slider_name.append(id_);
  ImGui::SliderInt(focus_slider_name.c_str(), &sample_focus_, sample_range_,
                   DataMonitor_Max_BufferSize);

  if (ImGui::Button("Monitor - Add on")) {
    observer_set_.push_back(new DataObserver(
        atd::utility::CString::cstring_cat("%d", ++monitor_counter_),
        data_repository_));
  }
  ImGui::SameLine();
  ImGui::Checkbox("Enable All", &enable_all_);

  ImGui::Separator();

  if (enable_all_) {
    update_data_base();
  }

  for (auto single_monitor : observer_set_) {
    single_monitor->render();
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
    data_repository_[signal.first].data_.pop_front();
    data_repository_[signal.first].data_.push_back(signal.second);
    data_repository_[signal.first].upper_bound_ =
        std::fmax(data_repository_[signal.first].upper_bound_, signal.second);
    data_repository_[signal.first].lower_bound_ =
        std::fmin(data_repository_[signal.first].lower_bound_, signal.second);
  }
  return true;
}

void DataMonitor::set_name(const std::string& name) { name_ = name; }
