#pragma once

#include <string>

// #include "protobuf_msg/shm_dispatcher.pb.h"

namespace atd {
namespace common {
namespace utility {

typedef struct {
  bool flag_init_ = false;
  int dispatcher_id = 0;
  size_t dispatcher_size = 0;
} SHM_DIRECTOR;
#define SHM_DIRECTOR_SEED 1111
#define SHM_DIRECTOR_SIZE sizeof(atd::common::utility::SHM_DIRECTOR)

}  // namespace utility
}  // namespace common
}  // namespace atd