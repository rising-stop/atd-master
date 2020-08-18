#include "file_handler.h"

namespace atd {
namespace utility {

std::fstream& ReadWriteableFile::get_FileStream() {
  if (file_stm_.good()) {
    return file_stm_;
  } else {
    // throw FileException(FileException::GOOD_CHECK_FAIL, "good checking
    // fail");
    CUSTOM_EXCEPTION("good checking fail");
  }
}

void ReadWriteableFile::file_init() {
  if (file_stm_.is_open()) {
    file_stm_.close();
  }
  if (mode_ == WRITE) {
    file_stm_.open(full_path_file_name_, std::ios::out);
    if (!file_stm_.is_open()) {
      // std::stringstream sstm;
      // sstm << "unable to open file " << full_path_file_name_;
      // throw FileException(FileException::UNABLE_TO_OPEN, sstm.str());
      CUSTOM_EXCEPTION("unable to open file %s", full_path_file_name_.c_str());
    }
  } else {
    file_stm_.open(full_path_file_name_, std::ios::in);
    if (!file_stm_.is_open()) {
      //   std::stringstream sstm;
      //   sstm << "unable to open file " << full_path_file_name_;
      //   throw FileException(FileException::UNABLE_TO_OPEN, sstm.str());
      CUSTOM_EXCEPTION("unable to open file %s", full_path_file_name_.c_str());
    }
  }
}

void ReadWriteableFile::redirect(FILE_MODE mode, const char* name,
                                 const char* path) {
  if (!strlen(path)) {
    path_ = "./";
  } else {
    path_ = path;
  }
  name_ = name;
  mode_ = mode;
  if (path_.at(path_.size() - 1) == '/') {
    full_path_file_name_.append(path_);
    full_path_file_name_.append(name_);
  } else {
    full_path_file_name_.append(path_);
    full_path_file_name_.append("/");
    full_path_file_name_.append(name_);
  }
  file_init();
}

void ReadWriteableFile::parse_file() {}
void ReadWriteableFile::refresh_file() {}

ReadWriteableFile::ReadWriteableFile(FILE_MODE mode, const char* name,
                                     const char* path) {
  redirect(mode, name, path);
}

ReadWriteableFile::~ReadWriteableFile() {
  if (file_stm_.is_open()) {
    file_stm_.close();
  }
}

}  // namespace utility
}  // namespace atd