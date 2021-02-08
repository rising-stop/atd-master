#include <cstring>

#include "utility.h"

namespace Themis {
namespace utility {

template <typename... ARG>
int CString::cstring_length_estimate(const char *str, ARG &&... args) {
  return cstring_length_estimate(str) +
         cstring_length_estimate(std::forward<ARG>(args)...);
}

template <typename T>
int CString::cstring_length_estimate(const T &num) {
  return 25;
}

template <typename T, typename... ARG>
int CString::cstring_length_estimate(const T &num, ARG &&... args) {
  return cstring_length_estimate(num) +
         cstring_length_estimate(std::forward<ARG>(args)...);
}

template <typename... ARG>
std::string CString::cstring_cat(const char *str_cat, ARG &&... args) {
  int char_num =
      strlen(str_cat) + cstring_length_estimate(std::forward<ARG>(args)...);
  std::shared_ptr<char> str_container;
  str_container.reset(new char[char_num]);
  sprintf(str_container.get(), str_cat, std::forward<ARG>(args)...);
  return std::string(str_container.get());
}

template <typename METHOD>
void CString::cstring_split(const std::string &split_line, const char comma,
                            METHOD substring_handler, int op_times) {
  uint64_t sub_head_index = 0, sub_tail_index = 1;
  if (split_line.empty()) {
    return;
  }
  uint64_t index4loop = 0;
  int operate_times = 0;
  while (index4loop < split_line.size()) {
    if (operate_times == op_times) {
      break;
    }
    if (split_line.at(index4loop) == comma) {
      sub_tail_index = index4loop;
      substring_handler(
          split_line.substr(sub_head_index, sub_tail_index - sub_head_index));
      sub_head_index = index4loop + 1;
      operate_times++;
    }
    if (index4loop == (split_line.size() - 1)) {
      if (split_line.at(index4loop) != comma) {
        substring_handler(
            split_line.substr(sub_head_index, index4loop - sub_head_index));
        operate_times++;
      }
    }
    index4loop++;
  }
}

}  // namespace utility
}  // namespace Themis
