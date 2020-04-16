#include "timer.h"

namespace atd {
namespace planning {
namespace common {

Runtime_Observer* Runtime_Observer::instance_ = nullptr;
std::mutex Runtime_Observer::instance_mutex_;

Runtime_Observer* Runtime_Observer::get_Instance() {
  if (!instance_) {
    std::lock_guard<std::mutex> lock(instance_mutex_);
    if (!instance_) {
      instance_ = new Runtime_Observer();
    }
  }

  return instance_;
}

void Runtime_Observer::set_LogPeriod(const int& period) {
  NODE_LOG_PERIOD_ = period;
}

bool Runtime_Observer::is_LogPeriod() const {
  return get_Instance()->get_Counter(NODE_LAUNCH_STICK_ID) % NODE_LOG_PERIOD_ ==
                 0
             ? true
             : false;
}

}  // namespace common
}  // namespace planning
}  // namespace atd