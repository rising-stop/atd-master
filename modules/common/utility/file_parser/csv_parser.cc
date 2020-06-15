#include "csv_parser.h"

namespace atd {
namespace common {
namespace utility {

double CSVFile::get_Double(const std::string& title, uint32_t col) const {
  auto itr_desired_title = container_.find(title);
  if (itr_desired_title == container_.end()) {
    throw CSVException(CSVException::TITLE_NOT_FOUND,
                       "get_Double: title not found");
  }
  if (col >= itr_desired_title->second.size()) {
    throw CSVException(CSVException::COL_OVERFLOW,
                       "get_Double required col is out of range");
  }
  return atof(itr_desired_title->second.at(col).c_str());
}

int CSVFile::get_Int(const std::string& title, uint32_t col) const {
  auto itr_desired_title = container_.find(title);
  if (itr_desired_title == container_.end()) {
    throw CSVException(CSVException::TITLE_NOT_FOUND,
                       "get_Int: title not found");
  }
  if (col >= itr_desired_title->second.size()) {
    throw CSVException(CSVException::COL_OVERFLOW,
                       "get_Int required col is out of range");
  }
  return atoi(itr_desired_title->second.at(col).c_str());
}

std::string CSVFile::get_String(const std::string& title, uint32_t col) const {
  auto itr_desired_title = container_.find(title);
  if (itr_desired_title == container_.end()) {
    throw CSVException(CSVException::TITLE_NOT_FOUND,
                       "get_String: title not found");
  }
  if (col >= itr_desired_title->second.size()) {
    throw CSVException(CSVException::COL_OVERFLOW,
                       "get_String required col is out of range");
  }
  return itr_desired_title->second.at(col);
}

std::string* CSVFile::get_MutableElement(const std::string& title, int col) {
  auto itr_desired_title = container_.find(title);
  if (itr_desired_title == container_.end()) {
    throw CSVException(CSVException::TITLE_NOT_FOUND,
                       "get_MutableElement: title not found");
  }
  while (col >= itr_desired_title->second.size()) {
    itr_desired_title->second.push_back("0");
  }
  return &itr_desired_title->second[col];
}

void CSVFile::register_title(const std::string& title) {
  title_set_.insert(title);
  container_.insert({title, std::vector<std::string>()});
}
void CSVFile::set_TitleContent(const std::string& title,
                               const std::vector<std::string>& content) {
  auto itr_desired_title = container_.find(title);
  if (itr_desired_title == container_.end()) {
    throw CSVException(CSVException::TITLE_NOT_FOUND,
                       "get_MutableElement: title not found");
  }
  itr_desired_title->second = content;
}

void CSVFile::parse_csv();
void CSVFile::refresh_csv();

CSVFile::CSVFile(const char* path, const char* name)
    : ReadWriteableFile(path, name) {}

}  // namespace utility
}  // namespace common
}  // namespace atd