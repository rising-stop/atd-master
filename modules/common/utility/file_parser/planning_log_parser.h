#pragma once

#include "file_handler.h"
#include "modules/common/utility/utility.h"
#include "planning_log.pb.h"

namespace atd {
namespace utility {

class PlanningLog : public ReadWriteableFile {
 public:
  PlanningLog() = default;
  explicit PlanningLog(FILE_MODE mode, const char* name,
                       const char* path = "~/.config/ATD/default/");
  virtual ~PlanningLog() = default;
};

}  // namespace utility
}  // namespace atd