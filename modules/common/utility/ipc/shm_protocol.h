#pragma once

#include <stddef.h>
#include <stdint.h>

#include <string>
#include <unordered_map>

namespace atd {
namespace common {
namespace utility {

#define SHM_DISTRIBUTOR_SEED 1111
#define SEM_MUTEX_SEMNUM 1

typedef struct {
  bool is_available_ = false;
  uint32_t id_ = 0;
  uint64_t size_ = 0;
} SHM;

typedef struct {
  bool is_available_ = false;
  uint32_t id_ = 0;
  uint32_t num_ = 0;
} SEM;

enum SEM_MUTEX_MODE : int { DUAL_MUTEX = 1, SHARED_MUTEX = 2 };

typedef struct {
  SHM shm_;
  SEM sem_;
  SEM_MUTEX_MODE mode_ = DUAL_MUTEX;
} SHM_MODULE;

typedef std::string UNIT_ID;

typedef std::unordered_map<UNIT_ID, SHM_MODULE> COM_COMPONENT;

typedef std::unordered_map<UNIT_ID, COM_COMPONENT> COM_CONTAINER;

typedef struct {
  bool is_init_ = false;
  COM_CONTAINER com_container_;
} SHM_DISTRIBUTOR;

#define SHM_DISTRIBUTOR_SIZE sizeof(atd::common::utility::SHM_DISTRIBUTOR)

}  // namespace utility
}  // namespace common
}  // namespace atd