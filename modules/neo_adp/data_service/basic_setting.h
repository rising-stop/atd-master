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

/**
 * @brief times new value of some calibration sended
 */
#define Calibrator_AlterCommand_SendTime 3

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

class Any_Repository {
 public:
  template <typename T, typename... ARGS>
  std::pair<std::shared_ptr<T>, bool> try_RegisterVar(const std::string& name,
                                                      ARGS&&... args) {
    return insert(name,
                  create_CalibrationVariable(std::forward<ARGS>(args)...));
  }

  void get_RegisteredSet(
      std::vector<std::pair<std::string, std::string>>& list) const {
    list.clear();
    size_t capacity = var_repository_.size();
    list.resize(capacity);
    size_t counter = 0u;
    for (const auto& item : var_repository_) {
      list[counter++] = std::make_pair(item.first, item.second.first);
    }
  }

  template <typename T>
  std::pair<const std::shared_ptr<T>, bool> get_RegisteredVar(
      const std::string& name) const {
    auto itr_name = var_repository_.find(name);
    if (itr_name == var_repository_.end()) {
      return {nullptr, false};
    }
    if (typeid(T).name() != itr_name->first) {
      return {nullptr, false};
    }
    return {static_cast<std::shared_ptr<T>>(itr_name->second.second), true};
  }

  template <typename T>
  std::pair<std::shared_ptr<T>, bool> get_MutableRegisteredVar(
      const std::string& name) {
    auto itr_name = var_repository_.find(name);
    if (itr_name == var_repository_.end()) {
      return {nullptr, false};
    }
    if (typeid(T).name() != itr_name->first) {
      return {nullptr, false};
    }
    return {static_cast<std::shared_ptr<T>>(itr_name->second.second), true};
  }

  void clear() { var_repository_.clear(); }

  void remove_Var(const std::string& name) {
    auto res_find = var_repository_.find(name);
    if (res_find != var_repository_.end()) {
      var_repository_.erase(res_find);
    }
  }

 private:
  template <typename T>
  std::pair<std::shared_ptr<T>, bool> insert(const std::string& name,
                                             std::shared_ptr<T> ptr_calib) {
    std::string type_name = typeid(T).name();
    auto ins_res = var_repository_.insert(std::make_pair(
        name, std::make_pair(type_name,
                             static_cast<std::shared_ptr<void>>(ptr_calib))));
    if (ins_res.second) {
      return {ptr_calib, true};
    }
    delete ptr_calib;
    return {static_cast<std::shared_ptr<T>>(ins_res.first->second), false};
  }

  template <typename T, typename... ARGS>
  std::shared_ptr<T> create_CalibrationVariable(ARGS&&... args) const {
    return std::make_shared<T>(std::forward<ARGS>(args)...);
  }

 private:
  std::map<std::string, std::pair<std::string, std::shared_ptr<void*>>>
      var_repository_;

 public:
  Any_Repository() = default;
  ~Any_Repository() = default;
};