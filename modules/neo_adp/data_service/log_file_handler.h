#pragma once

#include "data_repository.h"
#include "data_seg4data_monitor.h"
#include "lcm/lcm-cpp.hpp"
#include "modules/common/common_header.h"

using namespace atd::utility;
using namespace atd::protocol;

enum LCMFILE_MODE : int { DEFAULT = 0, WRITE = 1, READ = 2 };

class LCM_File_Handler {
 public:
  virtual bool open_LogFile(const std::string&) {}
  virtual bool create_LogFile(const std::string&) {}
  LCMFILE_MODE get_Mode() const;
  const std::string& get_Name() const;
  bool good() const;

 protected:
  bool file_init(const std::string&, LCMFILE_MODE);

  bool flag_is_initalized_ = false;
  LCMFILE_MODE mode_ = DEFAULT;
  std::string name_;
  std::shared_ptr<lcm::LogFile> ptr_file_;

 public:
  LCM_File_Handler() = default;
  LCM_File_Handler(const std::string&, LCMFILE_MODE);
  ~LCM_File_Handler() = default;
};

class PlanningLog_Reader : public LCM_File_Handler, public DataSeg4DataMonitor {
 public:
  using const_iterator =
      std::vector<atd::protocol::MONITOR_MSG>::const_iterator;

 public:
  virtual bool open_LogFile(const std::string&) override;
  virtual bool update() override;
  bool is_Done() const;

 protected:
  bool init();
  void preprocess_LogFile();

 private:
  std::thread* thread_read_ = nullptr;
  volatile bool flag_is_file_read_ = false;

 public:
  PlanningLog_Reader() = default;
  PlanningLog_Reader(const std::string& name);
  ~PlanningLog_Reader() = default;
};

class PlanningLog_Writer : public LCM_File_Handler {
 public:
  virtual bool create_LogFile(const std::string&) override;
  void listening(const atd::protocol::MONITOR_MSG&);

 protected:
  std::string name_;
  bool init();

 public:
  PlanningLog_Writer() = default;
  PlanningLog_Writer(const std::string& name);
};

#define LOG_READER_POINTER                            \
  atd::utility::Singleton::instance<DataRepository>() \
      ->get_DataPointer<PlanningLog_Reader>(Data_Seg_Name_LogMonitor)
