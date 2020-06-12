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

}  // namespace utility
}  // namespace common
}  // namespace atd