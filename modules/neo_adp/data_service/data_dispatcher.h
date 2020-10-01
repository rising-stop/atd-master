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
  void updata_Database();

  void send_CalibrationAlternation();

  void set_AlteredCalibration(const std::string&, float);
  void set_AlteredCalibration(const std::string&, int);
  void set_AlteredCalibration(const std::string&, uint);

  bool get_LatestFrame(MONITOR_MSG& frame) const;

  bool get_LatestHeader(FRAME_HEADER& frame) const;

  bool get_LatestLogInfo(LOG_CONTENT& frame) const;

  bool get_LatestGLElement(OPENGL_ELEMENT& frame) const;

  bool get_LatestDisplayContent(DISPLAY_CONTENT& frame) const;

  bool get_LatestDisplayCalib(DISPLAY_CALIBRATION& frame) const;

  bool get_DataMonitor_LatestFrame(std::map<std::string, float>& umap) const;

 private:
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

 private:
  mutable WfirstRWLock rwlock_frame_msg_;
  mutable WfirstRWLock rwlock_cal_var_;

 private:
  DataDispatcher() = default;
  ~DataDispatcher() = default;
};
