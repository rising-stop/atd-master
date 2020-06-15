#include "utility.h"

namespace atd {
namespace common {
namespace utility {

template <typename METHOD>
void CString::cstring_split(const std::string &split_line, const char comma,
                            METHOD substring_handler, int op_times = -1) {
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
            split_line.substr(sub_head_index, index4loop - sub_head_index) + 1);
        operate_times++;
      }
    }
    index4loop++;
  }
}

}  // namespace utility
}  // namespace common
}  // namespace atd
