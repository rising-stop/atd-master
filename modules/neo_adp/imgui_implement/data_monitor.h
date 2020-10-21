#pragma once

#include <deque>
#include <map>
#include <set>
#include <vector>

#include "imgui_basic_component.h"
#include "modules/neo_adp/data_service/data_repository.h"
#include "modules/neo_adp/data_service/data_seg4data_monitor.h"
#include "modules/neo_adp/data_service/data_seg4lcm_protocol.h"
#include "modules/neo_adp/data_service/log_file_handler.h"

class DataObserver : public ImGui_Components {
 public:
  /**
   * @brief main loop for rendering every signal
   */
  virtual void render() override;

  const std::string& get_ID();

  void set_DisplayHeader(uint32_t header);
  void set_DisplayTailer(uint32_t tailer);

  void active();
  void pause();

 private:
  /**
   * @brief add a signal
   */
  bool register_signal(const std::string&);

  /**
   * @brief remove a signal
   */
  bool unregister_signal(const std::string&);

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
  const std::string id_;
  const std::map<std::string, line_frame>* data_list_;
  std::map<std::string, line_frame> backup_data_list_;

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

  uint32_t signal_counter_ = 0;

  // /**
  //  * @brief adjustable slider parameter calculation, decide the range of
  //  * data focus
  //  */
  // int sample_range_ = DataMonitor_Max_BufferSize;
  // int sample_focus_ = DataMonitor_Max_BufferSize;
  uint32_t header_ = 0;
  uint32_t tailer_ = DataMonitor_Min_BufferSize;

  bool flag_is_active_ = true;
  bool flag_auto_zoom_ = true;
  float plot_limit_[2] = {0.1, -0.1};

 private:
  static const int default_color_set_num = 5;
  static const float default_color_list[default_color_set_num][4];

 public:
  DataObserver(const std::string& id,
               const std::map<std::string, line_frame>* data);
  ~DataObserver() = default;
};

class DataObserver_Manager : public ImGui_Components {
 public:
  /**
   * @brief main loop, render every registered data observer
   */
  virtual void render() override;

  void set_MaxBufferSize(uint32_t);
  void set_ObserverDataSource(
      std::function<const std::map<std::string, line_frame>*()>);

 protected:
  void draw();

 private:
  uint32_t max_buffer_size_ = DataMonitor_Max_BufferSize;

  int sample_range_ = DataMonitor_Max_BufferSize;
  int sample_focus_ = DataMonitor_Max_BufferSize;

  std::string name_;

  std::vector<std::shared_ptr<DataObserver>> observer_set_;

  std::function<const std::map<std::string, line_frame>*()> data_source_;

  uint32_t monitor_counter_ = 0;
  bool enable_all_ = true;

 public:
  DataObserver_Manager() = default;
  DataObserver_Manager(
      uint32_t, std::function<const std::map<std::string, line_frame>*()>);
  ~DataObserver_Manager() = default;
};

/**
 * @brief class LogDataObserver
 */
class LogDataObserver : public DataObserver_Manager {
 public:
  virtual void render() override;

 protected:
  void draw();

 public:
  LogDataObserver() = default;
  LogDataObserver(uint32_t,
                  std::function<const std::map<std::string, line_frame>*()>);
  ~LogDataObserver() = default;
};
