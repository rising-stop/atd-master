#pragma once

#include "data_repository.h"

struct line_frame {
  std::deque<float> data_;
  float upper_bound_ = 0.0f;
  float lower_bound_ = 0.0f;

  line_frame() = default;
  line_frame(uint32_t, float init_value = 0.0f);
};

class DataSeg4DataMonitor : public RepositorySegment {
 public:
  virtual bool update() override;
  std::map<std::string, line_frame>& get_DataRef4Observer();
  const std::map<std::string, line_frame>& get_ConstDataRef4Observer();

 private:
  void push_SingleFrame(const std::string&, float);
  std::map<std::string, line_frame> data_monitor_summary_;
  uint32_t max_buffer_size_ = DataMonitor_Max_BufferSize;

 public:
  DataSeg4DataMonitor() = default;
  ~DataSeg4DataMonitor() = default;
};