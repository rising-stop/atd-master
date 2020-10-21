#include "data_seg4lcm_protocol.h"

bool RealTimeDataDispatcher::update() {
  unique_writeguard<WfirstRWLock> rwguard(rwlock_frame_msg_);
  if (!msg_reciver_.subscribe(frame_msg_)) {
    return false;
  }
  return true;
}

std::shared_ptr<MONITOR_MSG> RealTimeDataDispatcher::get_LatestFrame() const {
  unique_readguard<WfirstRWLock> rwguard(rwlock_frame_msg_);
  if (!msg_validity_checking()) {
    return nullptr;
  }
  auto res = std::make_shared<MONITOR_MSG>();
  *res = frame_msg_;
  return res;
}

std::shared_ptr<FRAME_HEADER> RealTimeDataDispatcher::get_LatestHeader() const {
  unique_readguard<WfirstRWLock> rwguard(rwlock_frame_msg_);
  if (!msg_validity_checking()) {
    return nullptr;
  }
  auto res = std::make_shared<FRAME_HEADER>();
  *res = frame_msg_.title();
  return res;
}

std::shared_ptr<LOG_CONTENT> RealTimeDataDispatcher::get_LatestLogInfo() const {
  unique_readguard<WfirstRWLock> rwguard(rwlock_frame_msg_);
  if (!msg_validity_checking()) {
    return nullptr;
  }
  auto res = std::make_shared<LOG_CONTENT>();
  *res = frame_msg_.log();
  return res;
}

std::shared_ptr<OPENGL_ELEMENT> RealTimeDataDispatcher::get_LatestGLElement()
    const {
  unique_readguard<WfirstRWLock> rwguard(rwlock_frame_msg_);
  if (!msg_validity_checking()) {
    return nullptr;
  }
  auto res = std::make_shared<OPENGL_ELEMENT>();
  *res = frame_msg_.gl_element();
  return res;
}

std::shared_ptr<DISPLAY_CONTENT>
RealTimeDataDispatcher::get_LatestDisplayContent() const {
  unique_readguard<WfirstRWLock> rwguard(rwlock_frame_msg_);
  if (!msg_validity_checking()) {
    return nullptr;
  }
  auto res = std::make_shared<DISPLAY_CONTENT>();
  *res = frame_msg_.display_element();
  return res;
}

std::shared_ptr<DISPLAY_CALIBRATION>
RealTimeDataDispatcher::get_LatestDisplayCalib() const {
  unique_readguard<WfirstRWLock> rwguard(rwlock_frame_msg_);
  if (!msg_validity_checking()) {
    return nullptr;
  }
  auto res = std::make_shared<DISPLAY_CALIBRATION>();
  *res = frame_msg_.calibrations();
  return res;
}

bool RealTimeDataDispatcher::msg_validity_checking() const {
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
