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
  void get_RegisteredCalibSet(std::vector<std::string>& list) const {
    list.clear();
    size_t capacity = calib_repository_.size();
    list.resize(capacity);
    size_t counter = 0u;
    for (const auto& item : calib_repository_) {
      list[counter++] = item.first;
    }
  }

  std::pair<const std::pair<std::string, void*>&, bool> get_RegisteredCalib(
      const std::string& name) const {
    auto itr_name = calib_repository_.find(name);
    if (itr_name == calib_repository_.end()) {
      return {{"", nullptr}, false};
    }
    return {itr_name->second, true};
  }

  template <typename T>
  bool set_calib(const std::string& name, const T& var) {
    auto itr_name = calib_repository_.find(name);
    if (itr_name == calib_repository_.end()) {
      return false;
    }
    std::string type_name = typeid(T).name();
    dynamic_cast<CalibrationVariable<T>*>(itr_name->second)->set_Var(var);
    return true;
  }

  template <typename T>
  std::pair<CalibrationVariable<T>*, bool> register_Calibration(
      const std::string& name, T var, T max, T min, T init) {
    return insert(name, create_CalibrationVariable(var, max, min, init));
  }

  void clear() { calib_repository_.clear(); }

  void remove_Calib(const std::string& name) {
    auto res_find = calib_repository_.find(name);
    if (res_find != calib_repository_.end()) {
      calib_repository_.erase(res_find);
    }
  }

 private:
  template <typename T>
  std::pair<CalibrationVariable<T>*, bool> insert(
      const std::string& name, CalibrationVariable<T>* ptr_calib) {
    std::string type_name = typeid(T).name();
    auto ins_res = calib_repository_.insert(std::make_pair(
        name, std::make_pair(type_name, static_cast<void*>(ptr_calib))));
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
  std::map<std::string, std::pair<std::string, void*>> calib_repository_;

 public:
  Any_CalibrationRepository() = default;
  ~Any_CalibrationRepository() {
    for (auto& type_set : calib_repository_) {
      delete type_set.second.second;
    }
  }
};