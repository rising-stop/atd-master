#include "shm_data_handler.h"

namespace Themis {
namespace utility {

void ShmDataHandler::set_TargetShm(int id, size_t size) {
  uptr_shm_ = Singleton::instance<ShmFactory>()->get_Shm(id, size);
}

ShmDataHandler::ShmDataHandler(int id, size_t size) { set_TargetShm(id, size); }

}  // namespace common
}  // namespace Themis