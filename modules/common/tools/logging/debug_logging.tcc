#include "debug_logging.h"

namespace Themis {
namespace utility {

template <typename T>
std::pair<std::shared_ptr<CalibrationVariable<T>>, bool>
DebugLogging::try_register_Calibration(const std::string &name, const T &max,
                                       const T &min, const T &init) {
  if (max < min) {
    CUSTOM_EXCEPTION("calibration variable max is lower than min");
  }
  return calib_repository_.try_RegisterVar<CalibrationVariable<T>>(
      name, init, max, min, init);
}

template <typename T>
const std::pair<std::shared_ptr<CalibrationVariable<T>>, bool>
DebugLogging::try_fetch_Calibration(const std::string &name) const {
  auto fetch_res =
      calib_repository_.get_RegisteredVar<CalibrationVariable<T>>(name);
  if (fetch_res.second) {
    return {std::static_pointer_cast<CalibrationVariable<T>>(
                fetch_res.first->pointer_),
            true};
  }
  return {nullptr, false};
}

template <typename T>
std::pair<std::shared_ptr<CalibrationVariable<T>>, bool>
DebugLogging::try_fetch_MutableCalibration(const std::string &name) {
  auto fetch_res =
      calib_repository_.get_MutableRegisteredVar<CalibrationVariable<T>>(name);
  if (fetch_res.second) {
    return {std::static_pointer_cast<CalibrationVariable<T>>(
                fetch_res.first->pointer_),
            true};
  }
  return {nullptr, false};
}

}  // namespace utility
}  // namespace Themis