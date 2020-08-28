#pragma once

#include <lcm/lcm-cpp.hpp>

#include "file_handler.h"
#include "modules/common/utility/exceptions.h"
#include "modules/common/utility/utility.h"
#include "planning_log.pb.h"

namespace atd {
namespace utility {

class PlanningLog : public ReadWriteableFile {
 public:
  void Log_Parse(const std::string&);

 protected:
  void log_record(const atd::protocol::MONITOR_MSG&);
  virtual void parse_file();
  virtual void refresh_file();

 private:
  std::string log_name_;
  const std::string channel_name_{"test_log"};

 public:
  PlanningLog() = default;
  explicit PlanningLog(FILE_MODE mode, const char* name,
                       const char* path = "~/.config/ATD/default/");
  virtual ~PlanningLog() = default;
};

}  // namespace utility
}  // namespace atd