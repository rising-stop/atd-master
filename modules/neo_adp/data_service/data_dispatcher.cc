#include "data_dispatcher.h"

void DataDispatcher::updata_Database() {
  unique_writeguard<WfirstRWLock> rwguard(rwlock_frame_msg_);
  if (!msg_reciver_.subscribe(frame_msg_)) {
    return;
  }

  /* for updating data monitor data base */
  data_monitor_summary_.clear();
  for (const auto& log_item : frame_msg_.log().content()) {
    for (const auto& var : log_item.float_vars()) {
      data_monitor_summary_.insert({var.name(), var.data()});
    }
    for (const auto& var : log_item.int_vars()) {
      data_monitor_summary_.insert(
          {var.name(), static_cast<float>(var.data())});
    }
    for (const auto& var : log_item.uint_vars()) {
      data_monitor_summary_.insert(
          {var.name(), static_cast<float>(var.data())});
    }
  }
  for (const auto& var : frame_msg_.display_element().float_vars()) {
    data_monitor_summary_.insert({var.name(), var.data()});
  }
  for (const auto& var : frame_msg_.display_element().int_vars()) {
    data_monitor_summary_.insert({var.name(), static_cast<float>(var.data())});
  }
  for (const auto& var : frame_msg_.display_element().uint_vars()) {
    data_monitor_summary_.insert({var.name(), static_cast<float>(var.data())});
  }
  for (const auto& var : frame_msg_.calibrations().calib_float()) {
    data_monitor_summary_.insert({var.name(), var.data()});
  }
  for (const auto& var : frame_msg_.calibrations().calib_int()) {
    data_monitor_summary_.insert({var.name(), static_cast<float>(var.data())});
  }
  for (const auto& var : frame_msg_.calibrations().calib_uint()) {
    data_monitor_summary_.insert({var.name(), static_cast<float>(var.data())});
  }
}

bool DataDispatcher::get_LatestFrame(MONITOR_MSG& frame) const {
  unique_readguard<WfirstRWLock> rwguard(rwlock_frame_msg_);
  if (!msg_validity_checking()) {
    return false;
  }
  frame = frame_msg_;
  return true;
}

bool DataDispatcher::get_LatestHeader(FRAME_HEADER& frame) const {
  unique_readguard<WfirstRWLock> rwguard(rwlock_frame_msg_);
  if (!msg_validity_checking()) {
    return false;
  }
  frame = frame_msg_.title();
  return true;
}

bool DataDispatcher::get_LatestLogInfo(LOG_CONTENT& frame) const {
  unique_readguard<WfirstRWLock> rwguard(rwlock_frame_msg_);
  if (!msg_validity_checking()) {
    return false;
  }
  frame = frame_msg_.log();
  return true;
}

bool DataDispatcher::get_LatestGLElement(OPENGL_ELEMENT& frame) const {
  unique_readguard<WfirstRWLock> rwguard(rwlock_frame_msg_);
  if (!msg_validity_checking()) {
    return false;
  }
  frame = frame_msg_.gl_element();
  return true;
}

bool DataDispatcher::get_LatestDisplayContent(DISPLAY_CONTENT& frame) const {
  unique_readguard<WfirstRWLock> rwguard(rwlock_frame_msg_);
  if (!msg_validity_checking()) {
    return false;
  }
  frame = frame_msg_.display_element();
  return true;
}

bool DataDispatcher::get_LatestDisplayCalib(DISPLAY_CALIBRATION& frame) const {
  unique_readguard<WfirstRWLock> rwguard(rwlock_frame_msg_);
  if (!msg_validity_checking()) {
    return false;
  }
  frame = frame_msg_.calibrations();
  return true;
}

bool DataDispatcher::get_DataMonitor_LatestFrame(
    std::map<std::string, float>& umap) const {
  unique_readguard<WfirstRWLock> rwguard(rwlock_frame_msg_);
  if (!msg_validity_checking()) {
    return false;
  }
  umap = data_monitor_summary_;
  return true;
}

bool DataDispatcher::msg_validity_checking() const {
  static uint64_t last_time_stamp = 0;
  static int time_out_counter = 0;
  if (last_time_stamp == frame_msg_.title().time_stamp()) {
    if (time_out_counter < DateDispatcher_TimeOut) {
      time_out_counter++;
    } else {
      return false;
    }
  } else {
    time_out_counter = 0;
  }
  last_time_stamp = frame_msg_.title().time_stamp();
  return true;
}
