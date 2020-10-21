#include "data_monitor.h"

#include "file_interface.h"
#include "modules/common/common_header.h"

const float
    DataObserver::default_color_list[DataObserver::default_color_set_num][4] = {
        {230.0f / 255.0f, 155.0f / 255.0f, 3.0f / 255.0f, 0.7f},
        {131.0f / 255.0f, 175.0f / 255.0f, 155.0f / 255.0f, 0.7f},
        {252.0f / 255.0f, 157.0f / 255.0f, 154.0f / 255.0f, 0.7f},
        {92.0f / 255.0f, 167.0f / 255.0f, 186.0f / 255.0f, 0.7f},
        {175.0f / 255.0f, 215.0f / 255.0f, 237.0f / 255.0f, 0.7f}};

void DataObserver::active() {
  if (!flag_is_active_) {
    flag_is_active_ = true;
  }
}

void DataObserver::pause() {
  if (flag_is_active_) {
    backup_data_list_ = *data_list_;
    flag_is_active_ = false;
  }
}

void DataObserver::render() {
  std::string botton_name{"Sheet: "};
  botton_name.append(id_);

  if (ImGui::Button(botton_name.c_str())) ImGui::OpenPopup(id_.c_str());

  if (ImGui::BeginPopup(id_.c_str())) {
    menu_handler();
    ImGui::EndPopup();
  }
  ImGui::SameLine();
  ImGui::Checkbox("Auto Zoom", &flag_auto_zoom_);
  ImGui::InputFloat2("Plot Bounds", plot_limit_);
  if (plot_limit_[0] < plot_limit_[1]) {
    plot_limit_[0] = plot_limit_[1] + 0.1;
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
  for (auto signal : *data_list_) {
    if (ob_list_.insert(std::make_pair(signal.first, menu_info())).second) {
      register_signal(signal.first);
    }
  }

  for (auto& menu_item : ob_list_) {
    if (data_list_->find(menu_item.first) == data_list_->cend()) {
      menu_item.second.flag_activate_ = false;
    }
    bool click_detect = menu_item.second.flag_activate_;
    ImGui::MenuItem(menu_item.first.c_str(), "",
                    &(menu_item.second.flag_activate_));
    if (click_detect && !menu_item.second.flag_activate_) {
      unregister_signal(menu_item.first);
    }
  }
}

void DataObserver::plot_render() {
  std::vector<ImU32> line_colors;
  std::vector<std::string> overlay_text;

  float v_max = -FLT_MAX;
  float v_min = FLT_MAX;
  bool plot_busy = false;

  for (auto& signal : ob_list_) {
    if (!signal.second.flag_activate_) {
      continue;
    }
    if (data_list_->find(signal.first) == data_list_->cend()) {
      signal.second.info_ = "offline";
    } else {
      signal.second.info_ = atd::utility::CString::cstring_cat(
          "%.3f", data_list_->at(signal.first).data_.back());
    }
    std::string color_edit_name = signal.first;
    color_edit_name.push_back(' ');
    color_edit_name.append(signal.second.info_);
    ImGui::ColorEdit4(color_edit_name.c_str(),
                      (float*)&color_dispatcher_[signal.first]);
    line_colors.push_back(
        ImGui::ColorConvertFloat4ToU32(color_dispatcher_.at(signal.first)));
    overlay_text.push_back(signal.first);
    v_max = std::fmax(v_max, data_list_->at(signal.first).upper_bound_);
    v_min = std::fmin(v_min, data_list_->at(signal.first).lower_bound_);
    plot_busy = true;
  }

  if (flag_auto_zoom_) {
    if (ob_list_.empty()) {
      plot_limit_[0] = 0.1;
      plot_limit_[1] = -0.1;
    } else {
      if (plot_busy) {
        plot_limit_[0] = v_max;
        plot_limit_[1] = v_min;
      }
    }
  } else {
    v_max = plot_limit_[0];
    v_min = plot_limit_[1];
  }

  uint32_t sample_range = DataMonitor_Min_BufferSize;
  if (header_ > tailer_) {
    CUSTOM_EXCEPTION("error setting of header and tailer");
  } else {
    if (data_list_->empty()) {
      header_ = 0;
      tailer_ = DataMonitor_Max_BufferSize;
    } else {
      if (tailer_ > data_list_->begin()->second.data_.size()) {
        tailer_ = data_list_->size();
      }
    }
    sample_range = tailer_ - header_;
    if (sample_range < DataMonitor_Min_BufferSize) {
      header_ = tailer_ - DataMonitor_Min_BufferSize;
      sample_range = DataMonitor_Min_BufferSize;
    }
  }

  if (flag_is_active_) {
    ImGui::MulitPlot(
        id_,
        [&](const std::string name, int idx) -> float {
          return data_list_->at(name).data_.at(idx);
        },
        sample_range, header_, line_colors, overlay_text, v_min, v_max,
        ImVec2(0, 160));
  } else {
    ImGui::MulitPlot(
        id_,
        [&](const std::string name, int idx) -> float {
          return backup_data_list_.at(name).data_.at(idx);
        },
        sample_range, header_, line_colors, overlay_text, v_min, v_max,
        ImVec2(0, 160));
  }
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
                           const std::map<std::string, line_frame>* data)
    : id_(id), data_list_(data) {}

/**
 * ********************************************
 * @def  Class DataMonitor
 * ********************************************
 */
void DataObserver_Manager::render() { draw(); }

void DataObserver_Manager::draw() {
  ImGui::SliderInt("Range: ", &sample_range_, DataMonitor_Min_BufferSize,
                   max_buffer_size_);

  if (sample_focus_ < sample_range_) {
    sample_focus_ = sample_range_;
  }
  ImGui::SliderInt("Focus: ", &sample_focus_, sample_range_, max_buffer_size_);

  if (ImGui::Button("Monitor - Add on")) {
    observer_set_.push_back(std::make_shared<DataObserver>(
        atd::utility::CString::cstring_cat("%d", ++monitor_counter_),
        data_source_()));
  }

  for (auto& ptr_observer : observer_set_) {
    ptr_observer->set_DisplayHeader(sample_focus_ - sample_range_);
    ptr_observer->set_DisplayTailer(sample_focus_);
  }

  ImGui::SameLine();
  ImGui::Checkbox("Enable All", &enable_all_);

  ImGui::Separator();

  for (auto single_monitor : observer_set_) {
    single_monitor->render();
    if (enable_all_) {
      single_monitor->active();
    } else {
      single_monitor->pause();
    }
  }

  if (ImGui::Button("Monitor - Remove")) {
    if (observer_set_.size()) {
      observer_set_.pop_back();
    }
  }
}

void DataObserver_Manager::set_MaxBufferSize(uint32_t size) {
  max_buffer_size_ = size;
  sample_range_ = size;
  sample_focus_ = size;
}

void DataObserver_Manager::set_ObserverDataSource(
    std::function<const std::map<std::string, line_frame>*()> func) {
  data_source_ = func;
}

DataObserver_Manager::DataObserver_Manager(
    uint32_t size,
    std::function<const std::map<std::string, line_frame>*()> func) {
  set_MaxBufferSize(size);
  set_ObserverDataSource(func);
}

void LogDataObserver::draw() {
  std::string file_name;
  if (atd::utility::Singleton::instance<ResourceInterface_Manager>()
          ->set_Button("LogFile4DataMonitor",
                       [&](const std::string& name) -> bool {
                         file_name = name;
                         return true;
                       })) {
    LOG_READER_POINTER->open_LogFile(file_name);
  }
  if (LOG_READER_POINTER->is_Done()) {
    this->DataObserver_Manager::draw();
  } else {
    ImGui::Text("waiting for log file loading ...");
  }
}

void LogDataObserver::render() { draw(); }

LogDataObserver::LogDataObserver(
    uint32_t size,
    std::function<const std::map<std::string, line_frame>*()> source)
    : DataObserver_Manager(size, source) {}
