#pragma once

#include <cstring>
#include <fstream>

#include "common/utility/exceptions.h"
#include "common/utility/utility.h"

namespace atd {
namespace common {
namespace utility {

class ReadWriteableFile {
 private:
  void file_init();
  char* name_;
  char* path_;
  char* full_path_file_name_;
  std::fstream file_stm_;

 public:
  void redirect(const char*, const char*);
  inline std::fstream& get_FileStream();

 public:
  explicit ReadWriteableFile(const char* file_name,
                             const char* full_path = "~/.config/ATD/default/");
  virtual ~ReadWriteableFile();
};

}  // namespace utility
}  // namespace common
}  // namespace atd