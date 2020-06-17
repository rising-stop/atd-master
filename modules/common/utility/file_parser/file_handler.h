#pragma once

#include <cstring>
#include <fstream>

#include "modules/common/utility/exceptions.h"
#include "modules/common/utility/utility.h"

namespace atd {
namespace utility {

class ReadWriteableFile {
 public:
  enum FILE_MODE : int { WRITE = 0, READ = 1 };

  void redirect(FILE_MODE, const char*, const char*);
  std::fstream* get_FileStream();

  virtual void parse_file();
  virtual void refresh_file();

 private:
  void file_init();
  char* name_ = new char;
  char* path_ = new char;
  char* full_path_file_name_ = new char;
  FILE_MODE mode_;
  std::fstream file_stm_;

 public:
  ReadWriteableFile() = default;
  explicit ReadWriteableFile(FILE_MODE mode, const char* file_name,
                             const char* full_path = "~/.config/ATD/default/");
  virtual ~ReadWriteableFile();
};

}  // namespace utility
}  // namespace atd