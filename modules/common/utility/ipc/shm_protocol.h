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

typedef struct {
  void clear() { com_compnents_.clear(); }

  SHM_MODULE* add_ID(const UNIT_ID& id) {
    if (com_compnents_.find(id) == com_compnents_.end()) {
      com_compnents_.insert(std::make_pair(id, SHM_MODULE()));
    }
    return &com_compnents_[id];
  }

  bool try_get_ID(const UNIT_ID& id, SHM_MODULE& mdl) const {
    if (com_compnents_.find(id) == com_compnents_.end()) {
      return false;
    }
    mdl = com_compnents_.at(id);
    return true;
  }

  std::unordered_map<UNIT_ID, SHM_MODULE> com_compnents_;
} COM_COMPONENT;

typedef std::unordered_map<UNIT_ID, COM_COMPONENT> COM_CONTAINER;

typedef struct {
  void clear() { com_container_.clear(); }

  COM_COMPONENT* add_ID(const UNIT_ID& id) {
    if (com_container_.find(id) == com_container_.end()) {
      com_container_.insert(std::make_pair(id, COM_COMPONENT()));
    }
    return &com_container_[id];
  }

  bool try_get_ID(const UNIT_ID& id, COM_COMPONENT& comp) const {
    if (com_container_.find(id) == com_container_.end()) {
      return false;
    }
    comp = com_container_.at(id);
    return true;
  }

  COM_CONTAINER com_container_;
} SHM_DISTRIBUTOR;

#define SHM_DISTRIBUTOR_SIZE sizeof(atd::common::utility::SHM_DISTRIBUTOR)

}  // namespace utility
}  // namespace common
}  // namespace atd