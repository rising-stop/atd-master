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

 private:
  /**
   * @brief add a signal
   */
  bool register_signal(const string&);

  /**
   * @brief remove a signal
   */
  bool unregister_signal(const string&);

  /**
   * @brief handle selected menu
   */
  void menu_handler();

  /**
   * @brief updating plot data according to ob_list_
   */
  void plot_render();

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
  struct menu_info {
    bool flag_activate_ = false;
    std::string info_ = "";
  };
  std::map<std::string, menu_info> ob_list_;

  /**
   * @brief the color signal curve used
   */
  std::map<std::string, ImVec4> color_dispatcher_;

  int signal_counter_ = 0;

  /**
   * @brief adjustable slider parameter calculation, decide the range of
   * data focus
   */
  int sample_range_ = DataMonitor_Max_BufferSize;
  int sample_focus_ = DataMonitor_Max_BufferSize;

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
  bool update_data_base();

 private:
  struct line_frame {
    std::deque<float> data =
        std::deque<float>(DataMonitor_Max_BufferSize, 0.0f);
    float upper_bound = 0.0f;
    float lower_bound = 0.0f;
  };

  std::map<std::string, line_frame> data_repository_;

  std::vector<DataObserver*> observer_set_;

  uint32_t monitor_counter_ = 0;
  bool enable_all_ = true;

 public:
  DataMonitor() = default;
  ~DataMonitor() = default;
};
