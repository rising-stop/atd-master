#pragma once

#include <map>

#include "basic_setting.h"
#include "modules/common/common_header.h"
#include "protobuf_msg/planning_log.pb.h"

using namespace atd::utility;
using namespace atd::protocol;

class DataDispatcher final : public Singleton {
  friend class Singleton;

 public:
  /**
   * @brief main loop for updating data base
   */
  void spin();

  bool get_LogFrame(std::string& log) const;

  bool get_TotalLogFrame(std::string& log) const;

  bool get_BasicDisplayInfo(std::string& info) const;

  bool get_DataMonitor_LatestFrame(
      std::map<std::string, std::string>& umap) const;

  bool get_CalibInfo_As_Float(
      std::map<std::string, CalibrationVariable<float>>& umap) const;
  bool get_CalibInfo_As_Int(
      std::map<std::string, CalibrationVariable<int>>& umap) const;
  bool get_CalibInfo_As_UInt(
      std::map<std::string, CalibrationVariable<uint32_t>>& umap) const;

  bool set_CalibrationInfo_As_Float(const std::string name, float var);
  bool set_CalibrationInfo_As_Int(const std::string name, int var);
  bool set_CalibrationInfo_As_UInt(const std::string name, uint32_t var);

 private:
  void parse_LogContent(const MONITOR_MSG& msg, std::string& log) const;

  bool msg_validity_checking() const;

 private:
  /**
   * @brief data receiver and data publisher, lcm channel "PlanningLog" is
   * used for receiving messages from planning, channel "PlanningCalib" sends
   * calibrations to planning
   */
  Proto_Messages<MONITOR_MSG> frame_msg_;
  LCM_Proxy<Proto_Messages<MONITOR_MSG>> msg_reciver_{LCM_MODE::READER,
                                                      "PlanningLog"};
  Proto_Messages<atd::protocol::DISPLAY_CALIBRATION> cal_var_;
  LCM_Proxy<Proto_Messages<atd::protocol::DISPLAY_CALIBRATION>>
      calib_publisher_{LCM_MODE::SENDER, "PlanningCalib"};

  /**
   * @brief data base for data-monitor
   */
  std::map<std::string, float> data_monitor_summary_;

  /**
   * @brief data base for log monitor
   */
  std::string log_summary_;

  /**
   * @brief real time calibrations
   */
  std::map<std::string, CalibrationVariable<int>> calib_container_int_;
  std::map<std::string, CalibrationVariable<uint32_t>> calib_container_uint_;
  std::map<std::string, CalibrationVariable<float>> calib_container_float_;

 private:
  mutable WfirstRWLock rwlock_;

 private:
  DataDispatcher() = default;
  ~DataDispatcher() = default;
};
