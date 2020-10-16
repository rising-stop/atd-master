#pragma once

#include "data_seg4data_monitor.h"
#include "lcm/lcm-cpp.hpp"
#include "modules/common/common_header.h"

using namespace atd::utility;
using namespace atd::protocol;

enum LCMFILE_MODE : int { DEFAULT = 0, WRITE = 1, READ = 2 };

class LCM_File_Handler {
 public:
  bool open_LogFile(const std::string&);
  bool create_LogFile(const std::string&);
  LCMFILE_MODE get_Mode() const;
  const std::string& get_Name() const;
  bool good() const;

 protected:
  virtual bool init() {}
  bool file_init(const std::string&, LCMFILE_MODE);

  bool flag_is_initalized_ = false;
  LCMFILE_MODE mode_ = DEFAULT;
  std::string name_;
  std::shared_ptr<lcm::LogFile> ptr_file_;

 public:
  LCM_File_Handler(const std::string&, LCMFILE_MODE);
  ~LCM_File_Handler() = default;
};

class PlanningLog_Reader : public LCM_File_Handler {
 public:
  using const_iterator =
      std::vector<atd::protocol::MONITOR_MSG>::const_iterator;

 public:
  uint32_t get_TotalSize();
  std::pair<const_iterator, const_iterator> get_PlanningMessage(uint32_t,
                                                                uint32_t) const;
  bool is_Done() const;

 protected:
  virtual bool init() override;
  void preprocess_LogFile();

 private:
  std::thread* thread_read_ = nullptr;
  mutable std::mutex content_mutex_;
  std::vector<atd::protocol::MONITOR_MSG> log_content_;
  volatile bool flag_is_file_read_ = false;

 public:
  PlanningLog_Reader() = default;
  PlanningLog_Reader(const std::string& name);
  ~PlanningLog_Reader() = default;
};

class PlanningLog_Writer : public LCM_File_Handler {
 public:
  void listening(const atd::protocol::MONITOR_MSG&);

 protected:
  std::string name_;
  virtual bool init() override;

 public:
  PlanningLog_Writer() = default;
  PlanningLog_Writer(const std::string& name);
};

/**
 * @brief declearation of DataSeg4LCMLogger
 */
class DataSeg4LCMLogger : public DataSeg4DataMonitor {
 public:
  virtual bool update() override;
  void active(const std::string&);
  std::pair<PlanningLog_Reader::const_iterator,
            PlanningLog_Reader::const_iterator>
      get_PlanningMessage(uint32_t, uint32_t) const;

 private:
  bool inner_update();

  std::string file_name_;
  PlanningLog_Reader file_reader_;

 public:
  DataSeg4LCMLogger() = default;
  ~DataSeg4LCMLogger() = default;
};
