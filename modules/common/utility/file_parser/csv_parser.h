#pragma once

#include "common/utility/file_parser/file_handler.h"
#include "common/utility/utility.h"

namespace atd {
namespace common {
namespace utility {

class CSVFile : public ReadWriteableFile {
 private:
  enum BASIC_DATA_TYPE : int { STRING = 0, DOUBLE = 1, INT = 2 };
  struct CSV_CELL {
    BASIC_DATA_TYPE type_;
    std::string content_;
  };

  unsigned int row_ = 0, col_ = 0;
  std::vector<std::string> title_list_;
  std::unordered_map<std::string, std::vector<CSV_CELL>> container_;

 public:
  double get_Double(uint32_t, uint32_t) const;
  double get_Double(const std::string&, uint32_t) const;
  int get_Int(uint32_t, uint32_t) const;
  int get_Int(const std::string&, uint32_t) const;
  std::string get_String(uint32_t, uint32_t) const;
  std::string get_String(const std::string&, uint32_t) const;

  void set_Double(uint32_t, uint32_t, double);
  void set_Double(const std::string&, uint32_t, double);
  void set_Int(uint32_t, uint32_t, int);
  void set_Int(const std::string&, uint32_t, int);
  void set_String(uint32_t, uint32_t, const std::string&);
  void set_String(const std::string&, uint32_t, const std::string&);

  void set_TitleDouble(const std::string&, const std::vector<double>&);
  void set_TitleInt(const std::string&, const std::vector<int>&);
  void set_TitleString(const std::string&, const std::vector<std::string>&);

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