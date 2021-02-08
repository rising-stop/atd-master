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
  const std::map<std::string, line_frame>* get_ConstDataRef4Observer();

  uint32_t get_MaxBufferSize() const;
  void set_MaxBufferSize(uint32_t);
  void set_DataSource(
      std::function<std::shared_ptr<atd::protocol::MONITOR_MSG>()>);

 protected:
  void push_SingleFrame(const std::string&, float);
  bool parse_ProtocolMessage(const atd::protocol::MONITOR_MSG&);

 private:
  std::map<std::string, line_frame> data_monitor_summary_;
  uint32_t max_buffer_size_ = DataMonitor_Min_BufferSize;

  std::function<std::shared_ptr<atd::protocol::MONITOR_MSG>()> data_source_;

 public:
  DataSeg4DataMonitor(
      uint32_t, std::function<std::shared_ptr<atd::protocol::MONITOR_MSG>()>);
  DataSeg4DataMonitor() = default;
  ~DataSeg4DataMonitor() = default;
};

#define MONITOR_DATA_POINTER                          \
  atd::utility::Singleton::instance<DataRepository>() \
      ->get_DataConstPointer<DataSeg4DataMonitor>(Data_Seg_Name_DataMonitor)
