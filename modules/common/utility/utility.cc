#include "utility.h"

namespace atd {
namespace common {
namespace utility {

std::unordered_map<Singleton::SGLTN_ID, std::shared_ptr<std::once_flag>>
    Singleton::once_flags_;
std::unordered_map<Singleton::SGLTN_ID, Singleton *> Singleton::registry_;

}  // namespace utility
}  // namespace common
}  // namespace atd