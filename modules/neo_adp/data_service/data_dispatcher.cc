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

void DataDispatcher::send_CalibrationAlternation() {
  unique_writeguard<WfirstRWLock> rwguard(rwlock_cal_var_);
  calib_publisher_.publish(cal_var_);
  cal_var_.Clear();
}

void DataDispatcher::set_AlteredCalibration(const std::string& name,
                                            float var) {
  unique_writeguard<WfirstRWLock> rwguard(rwlock_cal_var_);

  auto ptr_float = cal_var_.add_calib_float();
  ptr_float->set_name(name);
  ptr_float->set_data(var);
}

void DataDispatcher::set_AlteredCalibration(const std::string& name, int var) {
  unique_writeguard<WfirstRWLock> rwguard(rwlock_cal_var_);
  auto ptr_int = cal_var_.add_calib_int();
  ptr_int->set_name(name);
  ptr_int->set_data(var);
}

void DataDispatcher::set_AlteredCalibration(const std::string& name, uint var) {
  unique_writeguard<WfirstRWLock> rwguard(rwlock_cal_var_);
  auto ptr_uint = cal_var_.add_calib_uint();
  ptr_uint->set_name(name);
  ptr_uint->set_data(var);
}

bool DataDispatcher::get_LatestFrame(MONITOR_MSG& frame) const {
  unique_readguard<WfirstRWLock> rwguard(rwlock_frame_msg_);
  if (!msg_validity_checking()) {
    return false;
  }
  frame = frame_msg_;
  return true;
}

bool DataDispatcher::get_BasicDisplayInfo(std::string& info) const {
  unique_readguard<WfirstRWLock> rwguard(rwlock_frame_msg_);
  if (!msg_validity_checking()) {
    return false;
  }
  // "AutoDriving\nSpeed(kph): 11.0\nSteer(deg): 11.0"
  std::string auto_info{"Offline"};
  std::string display_speed{"Speed(kph): "};
  std::string display_steer{"Steer(deg): "};
  std::string display_des_acc{"DeAcc(ms2): "};
  std::string display_act_acc{"AtAcc(ms2): "};
  std::string display_DTC{"DTC code: "};
  std::string display_setting_spd{"DeSpd(kph): "};
  for (auto single_content : frame_msg_.display_element().content()) {
    auto content_name = single_content.name();
    if (content_name == atd::utility::DISPLAY_FLAG_AUTO) {
      int mode_code = std::stoi(single_content.data());
      if (mode_code == 0) {
        auto_info = "Acc";
      } else if (mode_code == 1) {
        auto_info = "Manual";
      } else if (mode_code == 2) {
        auto_info = "Auto";
      } else {
        auto_info = "Abnormal";
      }
    } else if (content_name == atd::utility::DISPLAY_SPEED) {
      float spd = std::stof(single_content.data().c_str());
      display_speed.append(std::to_string(spd * 3.6));
    } else if (content_name == atd::utility::DISPLAY_STEER) {
      display_steer.append(single_content.data());
    } else if (content_name == atd::utility::DISPLAY_DESIRED_ACC) {
      display_des_acc.append(single_content.data());
    } else if (content_name == atd::utility::DISPLAY_ACTUAL_ACC) {
      display_act_acc.append(single_content.data());
    } else if (content_name == atd::utility::DISPLAY_DTC) {
      display_DTC.append(single_content.data());
    } else if (content_name == atd::utility::DISPLAY_SETTING_SPEED) {
      display_setting_spd.append(single_content.data());
    }
  }
  info.clear();
  info.append(auto_info);
  info.append("\n");
  info.append(display_setting_spd);
  info.append("\n");
  info.append(display_speed);
  info.append("\n");
  info.append(display_steer);
  info.append("\n");
  info.append(display_des_acc);
  info.append("\n");
  info.append(display_act_acc);
  info.append("\n");
  info.append(display_DTC);
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
