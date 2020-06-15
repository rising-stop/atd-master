#include "utility.h"

namespace atd {
namespace common {
namespace utility {

std::unordered_map<Singleton::SGLTN_ID, std::shared_ptr<std::once_flag>>
    Singleton::once_flags_;
std::unordered_map<Singleton::SGLTN_ID, Singleton *> Singleton::registry_;

template <typename... ARG>
void CString::cstring_cat(char *str, const char *str_cat, ARG &&... args) {
  strcat(str, str_cat);
  string_cat(str, std::forward<ARG>(args)...);
}

void CString::cstring_cat(char *str, const char *str_cat) {
  strcat(str, str_cat);
}

template <typename METHOD>
void CString::cstring_split(const std::string &split_line, const char comma,
                            METHOD substring_handler) {
  uint64_t sub_head_index = 0, sub_tail_index = 1;
  if (split_line.empty()) {
    return;
  }
  uint64_t index4loop = 0;
  while (index4loop < split_line.size()) {
    if (split_line.at(index4loop) == comma) {
      sub_tail_index = index4loop;
      substring_handler(
          split_line.substr(sub_head_index, sub_tail_index - sub_head_index));
      sub_head_index = index4loop + 1;
    }
    if (index4loop == (split_line.size() - 1)) {
      if (split_line.at(index4loop) != comma) {
        substring_handler(
            split_line.substr(sub_head_index, index4loop - sub_head_index) + 1);
      }
    }
  }
}

}  // namespace utility
}  // namespace common
}  // namespace atd