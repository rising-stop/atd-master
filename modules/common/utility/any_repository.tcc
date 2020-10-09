#include "any_repository.h"
#include "exceptions.h"

namespace atd {
namespace utility {

template <typename T, typename... ARGS>
std::pair<std::shared_ptr<T>, bool> Any_Repository::insert(
    const std::string &name, ARGS &&... args) {
  auto ptr_var = std::make_shared<T>(std::forward<ARGS>(args)...);
  auto ins_res = var_repository_.insert(std::make_pair(
      name, Variable(std::static_pointer_cast<void>(ptr_var), typeid(T).name(),
                     typeid(T).hash_code(), sizeof(T))));
  if (ins_res.second) {
    return {ptr_var, true};
  }
  if (ins_res.first->second.hash_code_ != typeid(T).hash_code()) {
    CUSTOM_EXCEPTION("Type Error");
  }
  return {std::static_pointer_cast<T>(ins_res.first->second.pointer_), false};
}

template <typename T, typename... ARGS>
std::pair<std::shared_ptr<T>, bool> Any_Repository::try_RegisterVar(
    const std::string &name, ARGS &&... args) {
  return insert<T>(name, std::forward<ARGS>(args)...);
}

template <typename T>
const std::pair<std::shared_ptr<T>, bool> Any_Repository::get_RegisteredVar(
    const std::string &name) const {
  auto itr_name = var_repository_.find(name);
  if (itr_name == var_repository_.end()) {
    return {nullptr, false};
  }
  if (typeid(T).hash_code() != itr_name->second.hash_code_) {
    return {nullptr, false};
  }
  return {std::static_pointer_cast<T>(itr_name->second.pointer_), true};
}

template <typename T>
std::pair<std::shared_ptr<T>, bool> Any_Repository::get_MutableRegisteredVar(
    const std::string &name) {
  auto itr_name = var_repository_.find(name);
  if (itr_name == var_repository_.end()) {
    return {nullptr, false};
  }
  if (typeid(T).hash_code() != itr_name->second.hash_code_) {
    return {nullptr, false};
  }
  return {std::static_pointer_cast<T>(itr_name->second.pointer_), true};
}

}  // namespace utility
}  // namespace atd