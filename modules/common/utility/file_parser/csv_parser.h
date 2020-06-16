#pragma once

#include <map>
#include <unordered_set>

#include "common/utility/file_parser/file_handler.h"
#include "common/utility/utility.h"

namespace atd {
namespace common {
namespace utility {

class CSVFile : public ReadWriteableFile {
 private:
  inline void check_TitleRegistered(const std::string&) const;
  inline void check_RequestColRange(uint64_t) const;
  inline void check_RequestRowRange(uint64_t) const;

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
  void push_Content(const std::string&, const std::string&);

  virtual void parse_file() override;
  virtual void refresh_file() override;

 public:
  CSVFile() = default;
  CSVFile(const char* name, const char* path = "~/.config/ATD/default/");
  virtual ~CSVFile() = default;
};

class CSV_Observer : public Singleton {
 public:
  void push_Item(const std::string&, const std::string&);

 private:
  bool try_Register(const std::string&);
  void col_Dispathcer(const std::string&);

  std::unordered_set<std::string> dynamic_registry_;
  std::unordered_set<std::string> registry_;
  uint64_t col_index_ = 0;

  CSVFile csv_;

 private:
  CSV_Observer(const std::string&, const std::string&);
  virtual ~CSV_Observer() = default;

  friend class atd::common::utility::Singleton;
};

}  // namespace utility
}  // namespace common
}  // namespace atd