#pragma once

#include "lcm/lcm-cpp.hpp"
#include "modules/common/common_header.h"

using namespace atd::utility;
using namespace atd::protocol;

enum LCMFILE_MODE : int { DEFAULT = 0, WRITE = 1, READ = 2 };

class LCM_File_Handler {
 public:
  bool open_LogFile(const std::string&);

 public:
  bool create_LogFile(const std::string&);

 protected:
  virtual void init(std::shared_ptr<lcm::LogFile>) = 0;
  LCMFILE_MODE mode_ = DEFAULT;
  std::string name_;

 public:
  LCM_File_Handler(const std::string&, LCMFILE_MODE);
  ~LCM_File_Handler() = default;
};

class PlanningLog_Reader : public LCM_File_Handler {
  using const_iterator =
      std::vector<atd::protocol::MONITOR_MSG>::const_iterator;

 public:
  uint64_t get_TotalSize();
  const atd::protocol::MONITOR_MSG& get_PlanningMessage(uint64_t) const;
  std::pair<const_iterator, const_iterator> get_PlanningMessage(uint64_t,
                                                                uint64_t);

 protected:
  virtual void init(std::shared_ptr<lcm::LogFile>) override;

 private:
  std::vector<atd::protocol::MONITOR_MSG> log_content_;

 public:
  PlanningLog_Reader() = default;
  PlanningLog_Reader(const std::string& name);
};

class PlanningLog_Writer : public LCM_File_Handler {
 public:
  void listening(const atd::protocol::MONITOR_MSG&);

 protected:
 std::string name_;
  virtual void init(std::shared_ptr<lcm::LogFile>) override;

 private:
  std::shared_ptr<lcm::LogFile> ptr_file_;

 public:
  PlanningLog_Writer() = default;
  PlanningLog_Writer(const std::string& name);
};
