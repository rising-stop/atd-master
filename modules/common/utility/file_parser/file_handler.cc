#include "file_handler.h"

namespace atd {
namespace common {
namespace utility {

std::fstream* ReadWriteableFile::get_FileStream() {
  if (file_stm_.good()) {
    return &file_stm_;
  } else {
    throw FileException(FileException::GOOD_CHECK_FAIL, "good checking fail");
  }
}

void ReadWriteableFile::file_init() {
  if (file_stm_.is_open()) {
    file_stm_.close();
  }
  file_stm_.open(full_path_file_name_, std::ios::in | std::ios::out);
  if (!file_stm_.is_open()) {
    std::stringstream sstm;
    sstm << "unable to open file " << full_path_file_name_;
    throw FileException(FileException::UNABLE_TO_OPEN, sstm.str());
  }
}

void ReadWriteableFile::redirect(const char* name, const char* path) {
  strcpy(path_, path);
  strcpy(name_, name);
  if (path[strlen(path) - 1] == '/') {
    CString::cstring_cat(full_path_file_name_, path, name);
  } else {
    CString::cstring_cat(full_path_file_name_, path, "/", name);
  }
  file_init();
}

void ReadWriteableFile::parse_file() {}
void ReadWriteableFile::refresh_file() {}

ReadWriteableFile::ReadWriteableFile(const char* name, const char* path) {
  redirect(name, path);
}

ReadWriteableFile::~ReadWriteableFile() {
  if (file_stm_.is_open()) {
    file_stm_.close();
  }
}

}  // namespace utility
}  // namespace common
}  // namespace atd