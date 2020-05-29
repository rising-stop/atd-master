#pragma once

#include <string>

// #include "protobuf_msg/shm_dispatcher.pb.h"

namespace atd {
namespace common {
namespace utility {

typedef struct {
  bool flag_init_ = false;
  uint32_t dispatcher_id = 0;
  size_t dispatcher_size = 0;
} SHM_DIRECTOR;
#define SHM_DIRECTOR_SEED 1111
#define SHM_DIRECTOR_SIZE sizeof(atd::common::utility::SHM_DIRECTOR)

typedef struct {
  uint32_t total_pkg_ = 0;
  uint32_t current_pkg_ = 0;
  uint32_t pkg_to_write_ = 0;
  uint32_t pkg_to_read_ = 0;
} HANDSHAKE_PROTOCOL;

#define SHM_PIECE_SIZE 512 * 512
typedef struct {
  uint64_t msg_count_ = 0;
  HANDSHAKE_PROTOCOL hand_shake_;
  size_t available_size_ = 0;
  char msg_[SHM_PIECE_SIZE];
} SHM_BASIC_FRAME;

}  // namespace utility
}  // namespace common
}  // namespace atd