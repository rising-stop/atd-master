#include "csv_parser.h"

namespace atd {
namespace common {
namespace utility {

double CSVFile::get_Double(uint32_t row, uint32_t col) const{
  if (row >= title_list_.size())
}

double CSVFile::get_Double(const std::string&, uint32_t) const;
int CSVFile::get_Int(uint32_t, uint32_t) const;
int CSVFile::get_Int(const std::string&, uint32_t) const;
std::string CSVFile::get_String(uint32_t, uint32_t) const;
std::string CSVFile::get_String(const std::string&, uint32_t) const;

void CSVFile::set_Double(uint32_t, uint32_t, double);
void CSVFile::set_Double(const std::string&, uint32_t, double);
void CSVFile::set_Int(uint32_t, uint32_t, int);
void CSVFile::set_Int(const std::string&, uint32_t, int);
void CSVFile::set_String(uint32_t, uint32_t, const std::string&);
void CSVFile::set_String(const std::string&, uint32_t, const std::string&);

void CSVFile::set_TitleDouble(const std::string&, const std::vector<double>&);
void CSVFile::set_TitleInt(const std::string&, const std::vector<int>&);
void CSVFile::set_TitleString(const std::string&,
                              const std::vector<std::string>&);

void CSVFile::parse_csv();
void CSVFile::refresh_csv();

CSVFile::CSVFile(const char* path, const char* name)
    : ReadWriteableFile(path, name) {}

}  // namespace utility
}  // namespace common
}  // namespace atd