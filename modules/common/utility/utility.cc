#include "utility.h"

namespace atd {
namespace utility {

std::mutex Singleton::register_lock_;
std::unordered_map<Singleton::SGLTN_ID, Singleton *> Singleton::registry_;

void CString::cstring_cat(char *str, const char *str_cat) {
  strcat(str, str_cat);
}

}  // namespace utility
}  // namespace atd