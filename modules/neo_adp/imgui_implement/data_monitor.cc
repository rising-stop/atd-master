#include "data_monitor.h"

#include "modules/common/common_header.h"

void DataObserver::Render() override {}

const int DataObserver::get_ID() { return id_; }

void DataObserver::Disable() { enable_ = false; }

void DataObserver::Enable() { enable_ = true; }

bool DataObserver::register_signal(const string& id) {
  auto citr_id = data_list_.find(id);
  if (citr_id == data_list_.cend()) {
    return false;
  }
  ob_list_.insert(id);
  if (signal_counter_ < default_color_set_num) {
    ImVec4 line_color = default_color_list[signal_counter_];
    color_dispatcher_.insert{std::make_pair(id, line_color)};
  } else {
    ImVec4 line_color = {TIMER->get_Rand(), TIMER->get_Rand(),
                         TIMER->get_Rand(), 0.7f};
    color_dispatcher_.insert{std::make_pair(id, line_color)};
  }
  signal_counter_++;
}

bool DataObserver::unregister_signal(const string& id) {
  auto ob_id = ob_list_.find(id);
  if (ob_id == ob_list_.cend()) {
    return false;
  }
  ob_list_.erase(ob_id);

  auto color_id = color_dispatcher_.find(id);
  if (color_id == color_dispatcher_.cend()) {
    return false;
  }
  color_dispatcher_.erase(color_id);
}

DataObserver::DataObserver(const int id,
                           const std::map<std::string, line_frame>& data)
    : id_(id), data_list_(data) {}

void DataMonitor::Render() {
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
      ImGui::MenuItem(menu_item.first.c_str(), "", &(menu_item.second.enable));
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
    ImGui::SameLine();
    ImGui::Text(std::to_string(line.second.data.back()).c_str());
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
      sample_range, 0, line_colors, overlay_text, -1.0f, 1.0f, ImVec2(0, 160));
  ImGui::Separator();
}

void DataMonitor::update_frame() {
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
      std::cerr << "update_frame(): stof throw a exception, error input: "
                << frame_[line_data.first] << ", name: " << line_data.first
                << std::endl;
    }
  }
}

const std::string DataMonitor::get_Name() const { return name_; }
void DataMonitor::Disable() { enable_ = false; }
void DataMonitor::Enable() { enable_ = true; }