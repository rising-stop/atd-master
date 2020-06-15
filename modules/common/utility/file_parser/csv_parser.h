#pragma once

#include <map>

#include "common/utility/file_parser/file_handler.h"
#include "common/utility/utility.h"

namespace atd {
namespace common {
namespace utility {

class CSVFile : public ReadWriteableFile {
 private:
  uint64_t row_size_ = 0;
  uint64_t col_size_ = 0;

  std::unordered_map<std::string, uint64_t> title_hash_;
  std::map<uint64_t, std::string> row_map_;
  std::map<uint64_t, std::vector<std::string>> container_;

 public:
  double get_Double(const std::string&, uint64_t) const;
  int get_Int(const std::string&, uint64_t) const;
  std::string get_String(const std::string&, uint64_t) const;

  void register_title(const std::string&);
  std::string* get_MutableElement(const std::string&, uint64_t);
  void set_TitleContent(const std::string&, const std::vector<std::string>&);

  virtual void parse_file() override;
  virtual void refresh_file() override;

 public:
  CSVFile() = default;
  CSVFile(const char*, const char*);
  virtual ~CSVFile() = default;
};  // namespace utility

}  // namespace utility
}  // namespace common
}  // namespace atd