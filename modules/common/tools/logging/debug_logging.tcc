#include "debug_logging.h"

namespace atd {
namespace utility {

template <typename T>
bool DebugLogging::try_register_Calibration(const std::string& name,
                                            const T& max, const T& min,
                                            const T& init) {
  if (max < min) {
    CUSTOM_EXCEPTION("calibration variable max is lower than min");
  }
  return calib_repository_.register_Calibration(name, init, max, min, init)
      .second;
}

template <typename T>
std::pair<T, bool> DebugLogging::try_fetch_Calibration(
    const std::string& name) const {
  auto fetch_res = calib_repository_.get_RegisteredCalib<T>(name);
  if (fetch_res.second) {
    return {fetch_res.first, true};
  }
  return {T(), false};
}

}  // namespace utility
}  // namespace atd