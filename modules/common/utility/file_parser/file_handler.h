#pragma once

#include <cstring>
#include <fstream>

#include "modules/common/utility/exceptions.h"
#include "modules/common/utility/utility.h"

namespace Themis {
namespace utility {

class ReadWriteableFile {
 public:
  enum FILE_MODE : int { WRITE = 0, READ = 1 };

  void redirect(FILE_MODE, const char*, const char*);
  std::fstream& get_FileStream();

 protected:
  virtual void parse_file();
  virtual void refresh_file();

  void file_init();
  std::string name_;
  std::string path_;
  std::string full_path_file_name_;
  FILE_MODE mode_;
  std::fstream file_stm_;

 public:
  ReadWriteableFile() = default;
  explicit ReadWriteableFile(FILE_MODE mode, const char* file_name,
                             const char* full_path = "~/.config/ATD/default/");
  virtual ~ReadWriteableFile();
};

}  // namespace utility
}  // namespace Themis