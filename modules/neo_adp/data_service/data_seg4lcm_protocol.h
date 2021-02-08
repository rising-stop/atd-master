#pragma once

#include <map>

#include "basic_setting.h"
#include "data_repository.h"
#include "modules/common/common_header.h"
#include "protobuf_msg/planning_log.pb.h"

using namespace Themis::utility;
using namespace Themis::protocol;

class RealTimeDataDispatcher : public RepositorySegment {
 public:
  /**
   * @brief main loop for updating data base
   */
  virtual bool update() override;

  std::shared_ptr<MONITOR_MSG> get_LatestFrame() const;

  std::shared_ptr<FRAME_HEADER> get_LatestHeader() const;

  std::shared_ptr<LOG_CONTENT> get_LatestLogInfo() const;

  std::shared_ptr<OPENGL_ELEMENT> get_LatestGLElement() const;

  std::shared_ptr<DISPLAY_CONTENT> get_LatestDisplayContent() const;

  std::shared_ptr<DISPLAY_CALIBRATION> get_LatestDisplayCalib() const;

  // bool get_DataMonitor_LatestFrame(std::map<std::string, float>& umap) const;

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

  /**
   * @brief data base for data-monitor
   */
  // std::map<std::string, float> data_monitor_summary_;

 private:
  mutable WfirstRWLock rwlock_frame_msg_;
  mutable WfirstRWLock rwlock_cal_var_;

 public:
  RealTimeDataDispatcher() = default;
  ~RealTimeDataDispatcher() = default;
};

#define PROTOCOL_POINTER                              \
  atd::utility::Singleton::instance<DataRepository>() \
      ->get_DataConstPointer<RealTimeDataDispatcher>( \
          Data_Seg_Name_LCMProtocol)
