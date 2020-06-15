#pragma once

#include <unordered_set>

#include "common/utility/file_parser/file_handler.h"
#include "common/utility/utility.h"
namespace atd {
namespace common {
namespace utility {

class CSVFile : public ReadWriteableFile {
 private:
  std::unordered_set<std::string> title_set_;
  std::unordered_map<std::string, std::vector<std::string>> container_;

 public:
  double get_Double(const std::string&, uint32_t) const;
  int get_Int(const std::string&, uint32_t) const;
  std::string get_String(const std::string&, uint32_t) const;

  std::string* get_MutableElement(const std::string&, int);
  void register_title(const std::string&);
  void set_TitleContent(const std::string&, const std::vector<std::string>&);

  void parse_csv();
  void refresh_csv();

 public:
  CSVFile() = default;
  CSVFile(const char*, const char*);
  virtual ~CSVFile() = default;
};  // namespace utility

class CSVParser {
 private:
 public:
  explicit CSVParser(const char*);
};

}  // namespace utility
}  // namespace common
}  // namespace atd