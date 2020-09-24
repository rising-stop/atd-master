#pragma once

#include <string>

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
#define DataMonitor_Min_BufferSize = 10;

template <typename T>
class CalibrationVariable {
 public:
  CalibrationVariable(const std::string& str, T var, T max, T min, T init)
      : name_(str), var_(var), max_(max), min_(min), init_(init) {}
  std::string name_;
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