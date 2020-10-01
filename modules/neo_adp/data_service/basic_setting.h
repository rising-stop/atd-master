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

 public:
  inline void set_Var(const T& var) { var_ = var; }
  inline void set_Max(const T& max) { max_ = max; }
  inline void set_Min(const T& min) { min_ = min; }
  inline void set_Init(const T& init) { init_ = init; }

  inline const T& get_Var() const { return var_; }
  inline const T& get_Max() const { return max_; }
  inline const T& get_Min() const { return min_; }
  inline const T& get_Init() const { return init_; }

 private:
  T var_;
  T max_;
  T min_;
  T init_;
};

class Any_CalibrationRepository {
 public:
  template <typename T>
  std::pair<const CalibrationVariable<T>*, bool> get_RegisteredCalib(
      const std::string& name) {
    std::string type_name = typeid(T).name();
    auto itr_type = calib_repository_.find(type_name);
    if (itr_type == calib_repository_.end()) {
      return {nullptr, false};
    }
    auto itr_name = calib_repository_[type_name].find(name);
    if (itr_name == calib_repository_[type_name].end()) {
      return {nullptr, false};
    }
    return {dynamic_cast<CalibrationVariable<T>*>(itr_name->second), true};
  }

  template <typename T>
  bool set_calib(const std::string& name, const T& var) {
    std::string type_name = typeid(T).name();
    auto itr_type = calib_repository_.find(type_name);
    if (itr_type == calib_repository_.end()) {
      return false;
    }
    auto itr_name = calib_repository_[type_name].find(name);
    if (itr_name == calib_repository_[type_name].end()) {
      return false;
    }
    dynamic_cast<CalibrationVariable<T>*>(itr_name->second)->set_Var(var);
    return true;
  }

  template <typename T>
  std::pair<CalibrationVariable<T>*, bool> register_Calibration(
      const std::string& name, T var, T max, T min, T init) {
    return insert(name, create_CalibrationVariable(var, max, min, init));
  }

 private:
  template <typename T>
  std::pair<CalibrationVariable<T>*, bool> insert(
      const std::string& name, CalibrationVariable<T>* ptr_calib) {
    std::string type_name = typeid(T).name();
    auto itr_type = calib_repository_.find(type_name);
    if (itr_type == calib_repository_.end()) {
      calib_repository_.insert(std::make_pair(
          type_name, std::map<std::string, void*>({std::make_pair(
                         name, static_cast<void*>(ptr_calib))})));
      return {ptr_calib, true};
    }
    auto ins_res = calib_repository_[type_name].insert(
        std::make_pair(name, static_cast<void*>(ptr_calib)));
    if (ins_res.second) {
      return {ptr_calib, true};
    }
    delete ptr_calib;
    return {dynamic_cast<CalibrationVariable<T>*>(ins_res.first->second),
            false};
  }

  template <typename T>
  CalibrationVariable<T>* create_CalibrationVariable(const std::string& str,
                                                     T var, T max, T min,
                                                     T init) const {
    return new CalibrationVariable<T>(str, var, max, min, init);
  }

 private:
  std::map<std::string, std::map<std::string, void*>> calib_repository_;

 public:
  Any_CalibrationRepository() = default;
  ~Any_CalibrationRepository() {
    for (auto& type_set : calib_repository_) {
      for (auto& var : type_set.second) {
        delete var.second;
      }
    }
  }
};