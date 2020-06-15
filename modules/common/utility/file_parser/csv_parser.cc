#include "csv_parser.h"

namespace atd {
namespace common {
namespace utility {

double CSVFile::get_Double(const std::string& title, uint64_t col) const {
  return atof(get_String(title, col).c_str());
}

int CSVFile::get_Int(const std::string& title, uint64_t col) const {
  return atoi(get_String(title, col).c_str());
}

std::string CSVFile::get_String(const std::string& title, uint64_t col) const {
  auto itr_desired_title = title_hash_.find(title);
  if (itr_desired_title == title_hash_.end()) {
    throw CSVException(CSVException::TITLE_NOT_FOUND,
                       "get_String: title not found");
  }
  if (col > col_size_) {
    throw CSVException(CSVException::COL_OVERFLOW,
                       "get_String: required col is out of range");
  }
  return container_.at(col).at(title_hash_.at(title));
}

std::string* CSVFile::get_MutableElement(const std::string& title,
                                         uint64_t col) {
  auto itr_desired_title = title_hash_.find(title);
  if (itr_desired_title == title_hash_.end()) {
    throw CSVException(CSVException::TITLE_NOT_FOUND, "title not found");
  }
  if (col > col_size_) {
    throw CSVException(CSVException::COL_OVERFLOW,
                       "required col is out of range");
  }
  return &container_[col][title_hash_.at(title)];
}

void CSVFile::register_title(const std::string& title) {
  auto res_insert = title_hash_.insert({title, row_size_});
  if (res_insert.second) {
    row_map_.insert({row_size_, title});
    row_size_++;
  }
  if (container_.empty()) {
    container_.insert(
        {0, std::vector<std::string>(row_size_, std::to_string(0))});
  } else {
    for (auto index_pair : container_) {
      index_pair.second.push_back(std::to_string(0));
    }
  }
}
void CSVFile::set_TitleContent(const std::string& title,
                               const std::vector<std::string>& content) {
  auto itr_desired_title = title_hash_.find(title);
  if (itr_desired_title == title_hash_.end()) {
    throw CSVException(CSVException::TITLE_NOT_FOUND, "title not found");
  }
  itr_desired_title->second = content;
}

void CSVFile::parse_file() {
  auto file_stm = get_FileStream();
  std::string content_in_line;
  std::getline(*file_stm, content_in_line);

  title_set_.clear();
  container_.clear();

  while (std::getline(*file_stm, content_in_line)) {
    if (content_in_line.empty()) {
      break;
    }
    auto title_end_index = content_in_line.find(',');
    std::string title_name = content_in_line.substr(0, title_end_index);
    register_title(title_name);
    content_in_line.erase(0, title_end_index + 1);

    auto& data_list = container_[title_name];
    CString::cstring_split(content_in_line, ',', [&](const std::string& str) {
      data_list.push_back(str);
    });
  }
}

void CSVFile::refresh_file() {
  auto file_stm = get_FileStream();
  file_stm->clear();

  for (auto title : title_set_) {
    auto data_list = container_.at(title);
    if (data_list.empty()) {
      *file_stm << title << '\n';
      continue;
    } else {
      *file_stm << title << ',';
    }
    for (auto itr4loop = data_list.begin(); itr4loop != data_list.end();
         itr4loop++) {
      if (itr4loop != (data_list.end() - 1)) {
        *file_stm << *itr4loop << ',';
      } else {
        *file_stm << *itr4loop << '\n';
      }
    }
  }

  file_stm->flush();
}

CSVFile::CSVFile(const char* path, const char* name)
    : ReadWriteableFile(path, name) {}

}  // namespace utility
}  // namespace common
}  // namespace atd