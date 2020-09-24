#pragma once

#include <deque>
#include <iostream>
#include <map>
#include <set>
#include <vector>

#include "imgui_basic_component.h"
#include "modules/neo_adp/data_service/data_dispatcher.h"

class DataObserver : public ImGui_Components {
 public:
  /**
   * @brief main loop for rendering every signal
   */
  virtual void Render() override;

  const int get_ID();

  /**
   * @brief stop update date from DataMonitor
   */
  void Disable();

  /**
   * @brief resume update date from DataMonitor
   */
  void Enable();

 protected:
  /**
   * @brief add a signal
   */
  bool register_signal(const string&);

  /**
   * @brief remove a signal
   */
  bool unregister_signal(const string&);

 private:
  /**
   * @brief register all references from DataMonitor, including all signal
   * names and all signal observation data
   */
  const int id_;
  const std::map<std::string, line_frame>& data_list_;

  /**
   * @brief user interested signal list, are selected by user
   */
  std::set<std::string> ob_list_;
  /**
   * @brief the color signal curve used
   */
  std::map<std::string, ImVec4> color_dispatcher_;

  bool enable_ = true;
  int signal_counter_ = 0;

 private:
  static const int default_color_set_num = 5;
  static const float default_color_list[default_color_set_num][4] = {
      {230.0f / 255.0f, 155.0f / 255.0f, 3.0f / 255.0f, 0.7f},
      {131.0f / 255.0f, 175.0f / 255.0f, 155.0f / 255.0f, 0.7f},
      {252.0f / 255.0f, 157.0f / 255.0f, 154.0f / 255.0f, 0.7f},
      {92.0f / 255.0f, 167.0f / 255.0f, 186.0f / 255.0f, 0.7f},
      {175.0f / 255.0f, 215.0f / 255.0f, 237.0f / 255.0f, 0.7f}};

 public:
  DataObserver(const int id, const std::map<std::string, line_frame>& data);
  ~DataObserver() = default;
};

class DataMonitor : public ImGui_Components {
 public:
  /**
   * @brief main loop, render every registered data observer
   */
  virtual void Render() override;

  /**
   * @brief add a observer
   */
  bool register_observer();

  /**
   * @brief remove a observer
   */
  bool unregister_observer();

 private:
  /**
   * @brief receiving new data
   */
  void update_data_base();

 private:
  struct line_frame {
    std::deque<float> data =
        std::deque<float>(DataMonitor_Max_BufferSize, 0.0f);
    float upper_bound = 0.0f;
    float lower_bound = 0.0f;
  };

  std::set<std::string> signal_list_;
  std::map<std::string, line_frame> data_repository_;

 public:
  DataMonitor() = default;
  ~DataMonitor() = default;
};
