#include "data_seg4lcm_protocol.h"

void RealTimeDataDispatcher::updata_Database() {
  unique_writeguard<WfirstRWLock> rwguard(rwlock_frame_msg_);
  if (!msg_reciver_.subscribe(frame_msg_)) {
    return;
  }
}

bool RealTimeDataDispatcher::get_LatestFrame(MONITOR_MSG& frame) const {
  unique_readguard<WfirstRWLock> rwguard(rwlock_frame_msg_);
  if (!msg_validity_checking()) {
    return false;
  }
  frame = frame_msg_;
  return true;
}

bool RealTimeDataDispatcher::get_LatestHeader(FRAME_HEADER& frame) const {
  unique_readguard<WfirstRWLock> rwguard(rwlock_frame_msg_);
  if (!msg_validity_checking()) {
    return false;
  }
  frame = frame_msg_.title();
  return true;
}

bool RealTimeDataDispatcher::get_LatestLogInfo(LOG_CONTENT& frame) const {
  unique_readguard<WfirstRWLock> rwguard(rwlock_frame_msg_);
  if (!msg_validity_checking()) {
    return false;
  }
  frame = frame_msg_.log();
  return true;
}

bool RealTimeDataDispatcher::get_LatestGLElement(OPENGL_ELEMENT& frame) const {
  unique_readguard<WfirstRWLock> rwguard(rwlock_frame_msg_);
  if (!msg_validity_checking()) {
    return false;
  }
  frame = frame_msg_.gl_element();
  return true;
}

bool RealTimeDataDispatcher::get_LatestDisplayContent(DISPLAY_CONTENT& frame) const {
  unique_readguard<WfirstRWLock> rwguard(rwlock_frame_msg_);
  if (!msg_validity_checking()) {
    return false;
  }
  frame = frame_msg_.display_element();
  return true;
}

bool RealTimeDataDispatcher::get_LatestDisplayCalib(DISPLAY_CALIBRATION& frame) const {
  unique_readguard<WfirstRWLock> rwguard(rwlock_frame_msg_);
  if (!msg_validity_checking()) {
    return false;
  }
  frame = frame_msg_.calibrations();
  return true;
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
