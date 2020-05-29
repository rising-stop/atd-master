#include "shm_data_handler.h"

namespace atd {
namespace common {
namespace utility {

void ShmDataHandler::set_TargetShm(int id, size_t size) {
  uptr_shm_ = Singleton::instance<ShmFactory>()->get_Shm(id, size);
}

ShmDataHandler::ShmDataHandler(int id, size_t size) { set_TargetShm(id, size); }

}  // namespace utility
}  // namespace common
}  // namespace atd