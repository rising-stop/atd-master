#pragma once

#include "lcm/lcm-cpp.hpp"
#include "modules/common/common_header.h"

using namespace atd::utility;
using namespace atd::protocol;

class LCM_File_Handler {
 public:
  bool is_good() { return file_.good(); }

 private:
  lcm::LogFile file_;

 public:
  LCM_File_Handler(const std::string& name) : file_(name, "r") {}
  ~LCM_File_Handler() = default;
};