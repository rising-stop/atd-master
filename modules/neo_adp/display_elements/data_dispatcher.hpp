#pragma once

#include <map>

#include "modules/common/common_header.h"
#include "protobuf_msg/planning_log.pb.h"

using namespace atd::utility;
using namespace atd::protocol;

constexpr int DataDispatcher_MaxBufferSize = 600;

class DataDispatcher final : public Singleton {
  friend class Singleton;

 public:
  void spin() {
    unique_writeguard<WfirstRWLock> rwguard(rwlock_);
    if (msg_reciver_.subscribe(frame_msg_)) {
      if (data_quene_.size() >= DataDispatcher_MaxBufferSize) {
        data_quene_.pop_front();
      }
      data_quene_.push_back(frame_msg_);

      variables_.clear();
      for (auto log_content : frame_msg_.log().content()) {
        for (auto item : log_content.variables()) {
          variables_.insert({item.name(), item.data()});
        }
      }
      for (auto display_content : frame_msg_.display_element().content()) {
        variables_.insert({display_content.name(), display_content.data()});
      }
    }
  }

  bool get_GLElements(OPENGL_ELEMENT& gl_ele) const {
    unique_readguard<WfirstRWLock> rwguard(rwlock_);
    if (!msg_validity_checking()) {
      return false;
    }
    gl_ele.Clear();
    gl_ele = frame_msg_.gl_element();
    return true;
  }

  bool get_LogFrame(std::string& log) const {
    unique_readguard<WfirstRWLock> rwguard(rwlock_);
    if (!msg_validity_checking()) {
      return false;
    }
    log.clear();
    parse_LogContent(frame_msg_, log);
    return true;
  }

  bool get_TotalLogFrame(std::string& log) const {
    unique_readguard<WfirstRWLock> rwguard(rwlock_);
    if (!msg_validity_checking()) {
      return false;
    }
    if (data_quene_.empty()) {
      return false;
    }
    log.clear();
    std::string tmp_str;
    for (int index = std::max(
             0, static_cast<int>(data_quene_.size() - 1 - log_buffer_size));
         index < std::min(DataDispatcher_MaxBufferSize,
                          static_cast<int>(data_quene_.size()));
         index++) {
      parse_LogContent(data_quene_.at(index), tmp_str);
      log.append(tmp_str);
    }
    return true;
  }

  bool get_BasicDisplayInfo(std::string& info) {
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
          auto_info = "Manual";
        } else if (mode_code == 1) {
          auto_info = "Acc";
        } else if (mode_code == 2) {
          auto_info = "Auto";
        } else {
          auto_info = "Abnormal";
        }
      } else if (content_name == atd::utility::DISPLAY_SPEED) {
        display_speed.append(single_content.data());
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

  bool get_LatestFrame(std::map<std::string, std::string>& umap) {
    unique_readguard<WfirstRWLock> rwguard(rwlock_);
    if (!msg_validity_checking()) {
      return false;
    }
    umap = variables_;
    return true;
  }

 private:
  void parse_LogContent(const MONITOR_MSG& msg, std::string& log) const {
    static int last_timestamp = 0;
    std::stringstream sstm;
    sstm << "########### "
         << "FRAME NO. " << msg.title().counter_no() << ", TIME STAMP "
         << msg.title().time_stamp() << ", ELAPSE " << msg.title().time_stamp() - last_timestamp << "ms ###########"
         << "\n";
    for (uint32_t index = 0; index < msg.log().content_size(); index++) {
      switch (msg.log().content(index).level()) {
        case atd::protocol::SECURITY_INFO::INFO:
          sstm << "> [INFO] > ";
          break;
        case atd::protocol::SECURITY_INFO::WARNING:
          sstm << "> [WARNING] > ";
          break;
        case atd::protocol::SECURITY_INFO::ERROR:
          sstm << "> [ERROR] > ";
          break;
        default:
          sstm << "> [UNKNOW] > ";
          break;
      }
      sstm << msg.log().content(index).file_name() << " no. "
           << msg.log().content(index).line_no() << ":"
           << "\n";
      for (uint32_t str_index = 0;
           str_index < msg.log().content(index).str_msg_size(); str_index++) {
        sstm << "   > " << msg.log().content(index).str_msg(str_index) << "\n";
      }
      for (uint32_t var_index = 0;
           var_index < msg.log().content(index).variables_size(); var_index++) {
        sstm << "   > " << msg.log().content(index).variables(var_index).name()
             << " = " << msg.log().content(index).variables(var_index).data()
             << "\n";
      }
      last_timestamp = msg.title().time_stamp();
    }

    log.clear();
    log = sstm.str();
  }

  bool msg_validity_checking() const {
    static uint64_t last_time_stamp = 0;
    static int time_out_counter = 0;
    if (last_time_stamp == frame_msg_.title().time_stamp()) {
      time_out_counter++;
      if (time_out_counter > time_out_) {
        time_out_counter = time_out_ + 1;
        return false;
      }
    } else {
      time_out_counter = 0;
    }
    last_time_stamp = frame_msg_.title().time_stamp();
    return true;
  }

 private:
  const int time_out_ = 60;
  const int log_buffer_size = 11;
  Proto_Messages<MONITOR_MSG> frame_msg_;
  std::map<std::string, std::string> variables_;
  LCM_Proxy<Proto_Messages<MONITOR_MSG>> msg_reciver_{LCM_MODE::READER,
                                                      "PlanningLog"};

  std::deque<atd::protocol::MONITOR_MSG> data_quene_;

  mutable WfirstRWLock rwlock_;

 private:
  DataDispatcher() = default;
  ~DataDispatcher() = default;
};
