#include "csv_parser.h"

#include <iostream>

namespace atd {
namespace common {
namespace utility {

void CSVFile::check_TitleRegistered(const std::string& title) const {
  auto itr_desired_title = title_hash_.find(title);
  if (itr_desired_title == title_hash_.end()) {
    throw CSVException(CSVException::TITLE_NOT_FOUND, "title not found");
  }
}

void CSVFile::check_RequestColRange(uint64_t col) const {
  if (col >= col_size_) {
    throw CSVException(CSVException::COL_OVERFLOW,
                       "required col is out of range");
  }
}
void CSVFile::check_RequestRowRange(uint64_t row) const {
  if (row >= row_size_) {
    throw CSVException(CSVException::ROW_OVERFLOW,
                       "required row is out of range");
  }
}

double CSVFile::get_Double(const std::string& title, uint64_t col) const {
  return atof(get_String(title, col).c_str());
}

int CSVFile::get_Int(const std::string& title, uint64_t col) const {
  return atoi(get_String(title, col).c_str());
}

std::string CSVFile::get_String(const std::string& title, uint64_t col) const {
  check_RequestColRange(col);
  check_TitleRegistered(title);
  return container_.at(col).at(title_hash_.at(title));
}

std::string* CSVFile::get_MutableElement(const std::string& title,
                                         uint64_t col) {
  check_TitleRegistered(title);
  if (col >= col_size_) {
    for (auto index4loop = col_size_; index4loop < col + 1; index4loop++) {
      container_.insert({index4loop, std::vector<std::string>(row_size_, "0")});
    }
    col_size_ = col + 1;
  }
  return &container_[col][title_hash_.at(title)];
}

void CSVFile::register_title(const std::string& title) {
  auto res_insert = title_hash_.insert({title, row_size_});
  if (res_insert.second) {
    row_map_.insert({row_size_, title});
    row_size_++;
  } else {
    std::stringstream sstm;
    sstm << "the title " << title << " has registered.";
    throw CSVException(CSVException::REGISTER_FAIL, sstm.str());
  }
  if (container_.empty()) {
    container_.insert({0, std::vector<std::string>(row_size_, "0")});
  } else {
    for (auto index_pair : container_) {
      index_pair.second.push_back("0");
    }
  }
}
void CSVFile::set_TitleContent(const std::string& title,
                               const std::vector<std::string>& content) {
  check_TitleRegistered(title);
  auto title_index = title_hash_.at(title);
  auto content_value = content.begin();
  if (col_size_ == content.size()) {
    for (auto index_pair : container_) {
      index_pair.second[title_index] = *content_value;
      content_value++;
    }
  } else if (col_size_ > content.size()) {
    for (auto index_pair : container_) {
      if (content_value != content.end()) {
        index_pair.second[title_index] = *content_value;
        content_value++;
      } else {
        index_pair.second[title_index] = "0";
      }
    }
  } else {
    for (uint64_t index4loop = 0; index4loop < content.size(); index4loop++) {
      if (index4loop >= col_size_) {
        container_.insert(
            {index4loop, std::vector<std::string>(row_size_, "0")});
      }
      container_[index4loop][title_index] = content.at(index4loop);
    }
    col_size_ = content.size();
  }
}

void CSVFile::push_Content(const std::string& title,
                           const std::string& content) {
  check_TitleRegistered(title);
  container_.insert({col_size_, std::vector<std::string>(row_size_, "0")});
  container_[col_size_][title_hash_.at(title)] = content;
  col_size_++;
}

void CSVFile::parse_file() {
  auto file_stm = get_FileStream();

  title_hash_.clear();
  row_map_.clear();
  container_.clear();

  std::string content_in_line;
  std::getline(*file_stm, content_in_line);

  CString::cstring_split(content_in_line, ',',
                         [&](const std::string& str) { register_title(str); });

  int row_index = 0;
  int col_index = 0;
  while (std::getline(*file_stm, content_in_line)) {
    if (content_in_line.empty()) {
      break;
    }
    row_index = 0;
    CString::cstring_split(
        content_in_line, ',',
        [&](const std::string& str) {
          *get_MutableElement(row_map_.at(row_index), col_index) = str;
          row_index++;
        },
        row_size_);
    col_index++;
  }
}

void CSVFile::refresh_file() {
  auto file_stm = get_FileStream();
  if (title_hash_.empty()) {
    return;
  }
  file_stm->clear();

  for (auto itr4loop = row_map_.begin(); itr4loop != row_map_.end();
       itr4loop++) {
    if (itr4loop->first == (row_size_ - 1)) {
      *file_stm << itr4loop->second << '\n';
      break;
    } else {
      *file_stm << itr4loop->second << ',';
    }
  }

  for (auto data_line : container_) {
    for (auto itr4loop = data_line.second.begin();
         itr4loop != data_line.second.end(); itr4loop++) {
      if (itr4loop != (data_line.second.end() - 1)) {
        *file_stm << *itr4loop << ',';
      } else {
        *file_stm << *itr4loop << '\n';
      }
    }
  }

  file_stm->flush();
}

CSVFile::CSVFile(const char* name, const char* path)
    : ReadWriteableFile(name, path) {}

void CSV_Observer::push_Item(const std::string& id, const std::string& data) {
  if (try_Register(id)) {
    csv_.register_title(id);
  }
  try {
    *csv_.get_MutableElement(id, col_index_) = data;
  } catch (const CSVException& e) {
    std::cout << e.what() << '\n' << std::endl;
  }
}

bool CSV_Observer::try_Register(const std::string& id) {
  return registry_.insert(id).second;
}

void CSV_Observer::col_Dispathcer(const std::string& id) {
  auto ins_res = dynamic_registry_.insert(id);
  if (!ins_res.second) {
    col_index_++;
    dynamic_registry_.clear();
  }
}

CSV_Observer::CSV_Observer(const std::string& name, const std::string& path)
    : csv_(name.c_str(), path.c_str()) {}

}  // namespace utility
}  // namespace common
}  // namespace atd