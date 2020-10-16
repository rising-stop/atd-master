#include "data_seg4data_monitor.h"

#include "data_seg4lcm_protocol.h"

line_frame::line_frame(uint32_t size, float init_value) {
  data_ = std::deque<float>(size, 0.0f);
}

std::map<std::string, line_frame>& DataSeg4DataMonitor::get_DataRef4Observer() {
  return data_monitor_summary_;
};

const std::map<std::string, line_frame>&
DataSeg4DataMonitor::get_ConstDataRef4Observer() {
  return data_monitor_summary_;
}

bool DataSeg4DataMonitor::update() {
  atd::protocol::MONITOR_MSG frame_msg;
  if (!atd::utility::Singleton::instance<DataRepository>()
           ->get_DataConstPointer<RealTimeDataDispatcher>(
               Data_Seg_Name_LCMProtocol)
           ->get_LatestFrame(frame_msg)) {
  }

  /* for updating data monitor data base */
  for (const auto& log_item : frame_msg.log().content()) {
    for (const auto& var : log_item.float_vars()) {
      push_SingleFrame(var.name(), var.data());
    }
    for (const auto& var : log_item.int_vars()) {
      push_SingleFrame(var.name(), static_cast<float>(var.data()));
    }
    for (const auto& var : log_item.uint_vars()) {
      push_SingleFrame(var.name(), static_cast<float>(var.data()));
    }
  }
  for (const auto& var : frame_msg.display_element().float_vars()) {
    push_SingleFrame(var.name(), var.data());
  }
  for (const auto& var : frame_msg.display_element().int_vars()) {
    push_SingleFrame(var.name(), static_cast<float>(var.data()));
  }
  for (const auto& var : frame_msg.display_element().uint_vars()) {
    push_SingleFrame(var.name(), static_cast<float>(var.data()));
  }
  for (const auto& var : frame_msg.calibrations().calib_float()) {
    push_SingleFrame(var.name(), var.data());
  }
  for (const auto& var : frame_msg.calibrations().calib_int()) {
    push_SingleFrame(var.name(), static_cast<float>(var.data()));
  }
  for (const auto& var : frame_msg.calibrations().calib_uint()) {
    push_SingleFrame(var.name(), static_cast<float>(var.data()));
  }

  return true;
}

void DataSeg4DataMonitor::push_SingleFrame(const std::string& name,
                                           float value) {
  auto itr_find = data_monitor_summary_.find(name);
  if (itr_find != data_monitor_summary_.end()) {
    data_monitor_summary_.insert(
        std::make_pair(name, line_frame(max_buffer_size_)));
  } else {
    data_monitor_summary_[name].data_.pop_front();
    data_monitor_summary_[name].data_.push_back(value);
    data_monitor_summary_[name].upper_bound_ =
        std::fmax(data_monitor_summary_[name].upper_bound_, value);
    data_monitor_summary_[name].lower_bound_ =
        std::fmin(data_monitor_summary_[name].lower_bound_, value);
  }
}
