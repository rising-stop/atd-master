#pragma once

#include "data_monitor.h"
#include "file_interface.h"
#include "imgui_basic_component.h"
#include "modules/common/common_header.h"

class DataAnalyzer : public ImGui_Components, public atd::utility::Singleton {
  friend class atd::utility::Singleton;

 public:
  template <typename T>
  void add_MonitorTab(
      const std::string& name, uint32_t size,
      std::function<const std::map<std::string, line_frame>*()> source) {
    auto itr_man = monitor_manager_.find(name);
    if (itr_man != monitor_manager_.end()) {
      return;
    }
    monitor_manager_.insert({name, std::make_shared<T>(size, source)});
  }

  virtual void render() override;

 private:
  std::unordered_map<std::string, std::shared_ptr<DataObserver_Manager>>
      monitor_manager_;

 private:
  DataAnalyzer() = default;
  ~DataAnalyzer() = default;
};