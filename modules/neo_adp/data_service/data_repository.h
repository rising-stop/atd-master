#pragma once

#include "modules/common/common_header.h"

class RepositorySegment {
 public:
  virtual bool update() = 0;

  virtual void error_handler() {}

 public:
  RepositorySegment() = default;
  ~RepositorySegment() = default;
};

class DataRepository : public atd::utility::Singleton {
  friend class atd::utility::Singleton;

 public:
  void update();

  template <typename T, typename... ARGS>
  bool try_register(const std::string& name, ARGS&&... args) {
    using DERIVED =
        typename std::enable_if<std::is_base_of<RepositorySegment, T>::value,
                                T>::type;
    auto ptr_derived = std::make_shared<DERIVED>(std::forward<ARGS>(args)...);
    return ptr_map_
        .insert(std::make_pair(
            std::string(name),
            std::dynamic_pointer_cast<RepositorySegment>(ptr_derived)))
        .second;
  }

  template <typename T>
  const std::shared_ptr<T> get_DataConstPointer(const std::string& name) const {
    auto citr_find = ptr_map_.find(name);
    if (citr_find == ptr_map_.cend()) {
      return {nullptr};
    }
    return std::dynamic_pointer_cast<T>(citr_find->second);
  }

  template <typename T>
  std::shared_ptr<T> get_DataPointer(const std::string& name) const {
    auto itr_find = ptr_map_.find(name);
    if (itr_find == ptr_map_.end()) {
      return {nullptr};
    }
    return std::dynamic_pointer_cast<T>(itr_find->second);
  }

 private:
  std::unordered_map<std::string, std::shared_ptr<RepositorySegment>> ptr_map_;

 private:
  DataRepository() = default;
  ~DataRepository() = default;
};