#include "utility.h"

namespace atd {
namespace utility {

template <typename... ARG>
std::string CString::cstring_cat(const char *str_cat, ARG &&... args) {
  std::string res;
  sprintf(res.data(), str_cat, std::forward<ARG>(args)...);
  return res;
}

template <typename METHOD>
void CString::cstring_split(const std::string &split_line, const char comma,
                            METHOD substring_handler, int op_times) {
  uint64_t sub_head_index = 0, sub_tail_index = 1;
  if (split_line.empty()) {
    return;
  }
  uint64_t index4loop = 0;
  int operate_times = 0;
  while (index4loop < split_line.size()) {
    if (operate_times == op_times) {
      break;
    }
    if (split_line.at(index4loop) == comma) {
      sub_tail_index = index4loop;
      substring_handler(
          split_line.substr(sub_head_index, sub_tail_index - sub_head_index));
      sub_head_index = index4loop + 1;
      operate_times++;
    }
    if (index4loop == (split_line.size() - 1)) {
      if (split_line.at(index4loop) != comma) {
        substring_handler(
            split_line.substr(sub_head_index, index4loop - sub_head_index));
        operate_times++;
      }
    }
    index4loop++;
  }
}

template <typename T, typename... ARGS>
std::pair<std::shared_ptr<T>, bool> Any_Repository::insert(
    const std::string &name, ARGS &&... args) {
  size_t type_name = typeid(T).hash_code();
  auto ptr_var = std::make_shared<T>(std::forward<ARGS>(args)...);
  auto ins_res = var_repository_.insert(std::make_pair(
      name,
      std::make_pair(type_name, static_cast<std::shared_ptr<void>>(ptr_var))));
  if (ins_res.second) {
    return {ptr_var, true};
  }
  return {static_cast<std::shared_ptr<T>>(ins_res.first->second), false};
}

template <typename T, typename... ARGS>
std::pair<std::shared_ptr<T>, bool> Any_Repository::try_RegisterVar(
    const std::string &name, ARGS &&... args) {
  return insert<T>(name, std::forward<ARGS>(args)...);
}

template <typename T>
std::pair<const std::shared_ptr<T>, bool> Any_Repository::get_RegisteredVar(
    const std::string &name) const {
  auto itr_name = var_repository_.find(name);
  if (itr_name == var_repository_.end()) {
    return {nullptr, false};
  }
  if (typeid(T).hash_code() != itr_name->first) {
    return {nullptr, false};
  }
  return {static_cast<std::shared_ptr<T>>(itr_name->second.second), true};
}

template <typename T>
std::pair<std::shared_ptr<T>, bool> Any_Repository::get_MutableRegisteredVar(
    const std::string &name) {
  auto itr_name = var_repository_.find(name);
  if (itr_name == var_repository_.end()) {
    return {nullptr, false};
  }
  if (typeid(T).hash_code() != itr_name->first) {
    return {nullptr, false};
  }
  return {static_cast<std::shared_ptr<T>>(itr_name->second.second), true};
}

}  // namespace utility
}  // namespace atd
