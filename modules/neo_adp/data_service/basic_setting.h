#pragma once

#include "modules/common/common_header.h"

/**
 * @brief max buffer size for log monitor
 */
#define LogMonitor_BufferSize 11

/**
 * @brief max time (frame) waiting for reconnection
 */
#define DateDispatcher_TimeOut 100

/**
 * @brief max buffer size for data monitor
 */
#define DataMonitor_Max_BufferSize 1800

/**
 * @brief max buffer size for data monitor
 */
#define DataMonitor_Min_BufferSize 10

/**
 * @brief time out for waiting planning responsing the calibration variable's
 * alternation
 */
#define Calibrator_TimeOut 10

struct line_frame {
  std::deque<float> data = std::deque<float>(DataMonitor_Max_BufferSize, 0.0f);
  float upper_bound = 0.0f;
  float lower_bound = 0.0f;
};

template <typename T>
class CalibrationVariable {
 public:
  CalibrationVariable(T var, T max, T min, T init)
      : var_(var), max_(max), min_(min), init_(init) {}
  T var_;
  T max_;
  T min_;
  T init_;
};

template <typename T>
static CalibrationVariable<T>* create_CalibrationVariable(
    const std::string& str, T var, T max, T min, T init) {
  return new CalibrationVariable<T>(str, var, max, min, init);
}
