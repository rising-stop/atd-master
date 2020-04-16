#ifndef STEP_SUPPORT_H_
#define STEP_SUPPORT_H_

#include "modules/common/common_support/observer/ObservingLogging.h"
#include "modules/common/common_support/timer/timer.h"

namespace atd {
namespace planning {
namespace common {

#ifndef STEP_HEADER
#define STEP_HEADER(logger_id, logger_conf_file_path, ver_num, log_period)  \
  static uint64_t last_period_time = 0, period_time = 0;                    \
  TIMER->set_BeginStick(atd::planning::common::NODE_STEP_STICK_ID);         \
  TIMER->set_Counter(atd::planning::common::NODE_LAUNCH_STICK_ID);          \
  TIMER->set_EndStick(atd::planning::common::NODE_LAUNCH_STICK_ID);         \
  period_time =                                                             \
      TIMER->get_TimeGap(atd::planning::common::NODE_LAUNCH_STICK_ID);      \
  REGISTER_LOGGER(logger_id, logger_conf_file_path)                         \
  static bool flag_version_once = true;                                     \
  if (flag_version_once) {                                                  \
    CLOG(INFO, logger_id) << ">>>>>>>  "                                    \
                          << "module version : " << ver_num << "  <<<<<<<"; \
    TIMER->set_LogPeriod(log_period);                                       \
    flag_version_once = false;                                              \
  }                                                                         \
  CLOG_IF(TIMER->is_LogPeriod(), INFO, logger_id)                           \
      << "----------- STEP : "                                              \
      << TIMER->get_Counter(atd::planning::common::NODE_LAUNCH_STICK_ID)    \
      << ",  MILESTONE : " << period_time                                   \
      << " ms,  LAST ELAPSE : " << period_time - last_period_time           \
      << "ms -----------";                                                  \
  last_period_time = period_time;
#endif

#ifndef STEP_END
#define STEP_END(logger_id)                                            \
  TIMER->set_EndStick(atd::planning::common::NODE_STEP_STICK_ID);      \
  CLOG_IF(TIMER->is_LogPeriod(), INFO, logger_id)                      \
      << "step complete, elapse "                                      \
      << TIMER->get_TimeGap(atd::planning::common::NODE_STEP_STICK_ID) \
      << "ms."                                                         \
      << "\n";
#endif

}  // namespace common
}  // namespace planning
}  // namespace atd

#endif