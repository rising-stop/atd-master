#include "data_dispatcher.h"

void DataDispatcher::spin() {
  unique_writeguard<WfirstRWLock> rwguard(rwlock_);
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

  /* for updating calibrator data base */

  calib_container_float_.clear();
  for (const auto& var : frame_msg_.calibrations().calib_float()) {
    calib_container_float_.insert(
        {var.name(),
         CalibrationVariable<float>(var.data(), var.data_upper_bound(),
                                    var.data_lower_bound(), var.data_init())});
  }
  calib_container_int_.clear();
  for (const auto& var : frame_msg_.calibrations().calib_int()) {
    calib_container_int_.insert(
        {var.name(),
         CalibrationVariable<int>(var.data(), var.data_upper_bound(),
                                  var.data_lower_bound(), var.data_init())});
  }
  calib_container_uint_.clear();
  for (const auto& var : frame_msg_.calibrations().calib_uint()) {
    calib_container_uint_.insert(
        {var.name(),
         CalibrationVariable<uint>(var.data(), var.data_upper_bound(),
                                   var.data_lower_bound(), var.data_init())});
  }

  cal_var_.Clear();
  for (const auto& calib_item : calib_dynamic_float_) {
    auto ptr_float = cal_var_.add_calib_float();
    ptr_float->set_name(calib_item.first);
    ptr_float->set_data(calib_item.second.second);
  }
  for (const auto& calib_item : calib_dynamic_int_) {
    auto ptr_int = cal_var_.add_calib_int();
    ptr_int->set_name(calib_item.first);
    ptr_int->set_data(calib_item.second.second);
  }
  for (const auto& calib_item : calib_dynamic_uint_) {
    auto ptr_uint = cal_var_.add_calib_uint();
    ptr_uint->set_name(calib_item.first);
    ptr_uint->set_data(calib_item.second.second);
  }

  calib_publisher_.publish(cal_var_);
}

bool DataDispatcher::get_LatestFrame(MONITOR_MSG& frame) const {
  unique_readguard<WfirstRWLock> rwguard(rwlock_);
  if (!msg_validity_checking()) {
    return false;
  }
  frame = frame_msg_;
  return true;
}

bool DataDispatcher::get_BasicDisplayInfo(std::string& info) const {
  unique_readguard<WfirstRWLock> rwguard(rwlock_);
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
  unique_readguard<WfirstRWLock> rwguard(rwlock_);
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

void DataDispatcher::set_CalibrationInfo_As_Float(const std::string name,
                                                  float var) {
  // calib_dynamic_float_
  unique_writeguard<WfirstRWLock> rwguard(rwlock_);
  auto itr_name = calib_container_float_.find(name);
  if (itr_name == calib_container_float_.end()) {
    return;
  }
  auto itr_dyn_name = calib_dynamic_float_.find(name);
  if (itr_dyn_name != calib_dynamic_float_.end()) {
    if (
        /* difference exists */
        fabs(itr_name->second.var_ - itr_dyn_name->second.second) > 1e-4f) {
      if (/* time limit not reached */
          itr_dyn_name->second.first > 0) {
        itr_dyn_name->second.first--;
      } else {
        calib_dynamic_float_.erase(itr_dyn_name);
      }
    } else {
      calib_dynamic_float_.erase(itr_dyn_name);
    }
  } else {
    if (
        /* difference exists */
        fabs(itr_name->second.var_ - var) > 1e-4f) {
      calib_dynamic_float_.insert({name, {Calibrator_TimeOut, var}});
    } else {
      /* none */
    }
  }
}

void DataDispatcher::set_CalibrationInfo_As_Int(const std::string name,
                                                int var) {
  unique_writeguard<WfirstRWLock> rwguard(rwlock_);
  auto itr_name = calib_container_int_.find(name);
  if (itr_name == calib_container_int_.end()) {
    return;
  }
  auto itr_dyn_name = calib_dynamic_int_.find(name);
  if (itr_dyn_name != calib_dynamic_int_.end()) {
    if (
        /* difference exists */
        itr_name->second.var_ == itr_dyn_name->second.second) {
      if (/* time limit not reached */
          itr_dyn_name->second.first > 0) {
        itr_dyn_name->second.first--;
      } else {
        calib_dynamic_int_.erase(itr_dyn_name);
      }
    } else {
      calib_dynamic_int_.erase(itr_dyn_name);
    }
  } else {
    if (
        /* difference exists */
        itr_name->second.var_ == var) {
      calib_dynamic_int_.insert({name, {Calibrator_TimeOut, var}});
    } else {
      /* none */
    }
  }
}

void DataDispatcher::set_CalibrationInfo_As_UInt(const std::string name,
                                                 uint32_t var) {
  unique_writeguard<WfirstRWLock> rwguard(rwlock_);
  auto itr_name = calib_container_uint_.find(name);
  if (itr_name == calib_container_uint_.end()) {
    return;
  }
  auto itr_dyn_name = calib_dynamic_uint_.find(name);
  if (itr_dyn_name != calib_dynamic_uint_.end()) {
    if (
        /* difference exists */
        itr_name->second.var_ == itr_dyn_name->second.second) {
      if (/* time limit not reached */
          itr_dyn_name->second.first > 0) {
        itr_dyn_name->second.first--;
      } else {
        calib_dynamic_uint_.erase(itr_dyn_name);
      }
    } else {
      calib_dynamic_uint_.erase(itr_dyn_name);
    }
  } else {
    if (
        /* difference exists */
        itr_name->second.var_ == var) {
      calib_dynamic_uint_.insert({name, {Calibrator_TimeOut, var}});
    } else {
      /* none */
    }
  }
}

bool DataDispatcher::get_CalibInfo_As_Float(
    std::map<std::string, CalibrationVariable<float>>& umap) const {
  unique_readguard<WfirstRWLock> rwguard(rwlock_);
  if (!msg_validity_checking()) {
    return false;
  }
  umap = calib_container_float_;
  return true;
}

bool DataDispatcher::get_CalibInfo_As_Int(
    std::map<std::string, CalibrationVariable<int>>& umap) const {
  unique_readguard<WfirstRWLock> rwguard(rwlock_);
  if (!msg_validity_checking()) {
    return false;
  }
  umap = calib_container_int_;
  return true;
}

bool DataDispatcher::get_CalibInfo_As_UInt(
    std::map<std::string, CalibrationVariable<uint32_t>>& umap) const {
  unique_readguard<WfirstRWLock> rwguard(rwlock_);
  if (!msg_validity_checking()) {
    return false;
  }
  umap = calib_container_uint_;
  return true;
}
