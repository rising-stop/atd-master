#pragma once

#include <map>
#include <unordered_set>

#include "modules/common/utility/file_parser/file_handler.h"
#include "modules/common/utility/utility.h"

namespace atd {
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

  uint64_t print_head_ = 0;

 public:
  double get_Double(const std::string&, uint64_t) const;
  int get_Int(const std::string&, uint64_t) const;
  std::string get_String(const std::string&, uint64_t) const;

  void register_title(const std::string&);
  std::string* get_MutableElement(const std::string&, uint64_t);
  void set_TitleContent(const std::string&, const std::vector<std::string>&);
  void push_Content(const std::string&, const std::string&);

  uint64_t get_RowSize() const;
  uint64_t get_ColSize() const;

  virtual void parse_file() override;
  virtual void refresh_file() override;
  void fresh_file();

 public:
  CSVFile() = default;
  CSVFile(FILE_MODE mode, const char* name,
          const char* path = "~/.config/ATD/default/");
  virtual ~CSVFile() = default;
};

class CSV_Observer : public Singleton {
  friend class atd::utility::Singleton;

 public:
  void redirect(ReadWriteableFile::FILE_MODE, const std::string&,
                const std::string&);
  void push_Item(const std::string&, const std::string&);

 private:
  bool try_Register(const std::string&);
  void col_Dispathcer(const std::string&);

  std::unordered_set<std::string> dynamic_registry_;
  std::unordered_set<std::string> registry_;
  uint64_t col_index_ = 0;

  CSVFile csv_;

 private:
  CSV_Observer() = default;
  CSV_Observer(ReadWriteableFile::FILE_MODE, const std::string&,
               const std::string&);
  virtual ~CSV_Observer() = default;
};

}  // namespace utility
}  // namespace atd