#pragma once

#include <cstddef>
#include <memory>
#include <string>
#include <unordered_map>

namespace Themis {
namespace utility {

class Any_Repository_iterator;
class const_Any_Repository_iterator;

/**
 * @brief Defination of class Any_Repository
 */
class Any_Repository {
 public:
  class Variable {
   public:
    std::shared_ptr<void> pointer_;
    std::string type_name_;
    size_t hash_code_ = 0u;
    size_t size_ = 0u;

   public:
    Variable(std::shared_ptr<void> &pointer, const std::string &name,
             const size_t &hash_code, const size_t &size)
        : pointer_(pointer),
          type_name_(name),
          hash_code_(hash_code),
          size_(size) {}

    Variable(std::shared_ptr<void> &&pointer, const std::string &name,
             const size_t &hash_code, const size_t &size)
        : pointer_(pointer),
          type_name_(name),
          hash_code_(hash_code),
          size_(size) {}
  };

 public:
  using key_type = std::string;
  using mapped_type = Variable;
  using value_type = std::unordered_map<std::string, Variable>::value_type;
  using reference = value_type &;
  using const_reference = const value_type &;
  using size_type = size_t;
  using differece_type = ptrdiff_t;
  using iterator = atd::utility::Any_Repository_iterator;
  using const_iterator = atd::utility::const_Any_Repository_iterator;
  using ListType = std::unordered_map<std::string, Variable>;

  iterator begin();
  iterator end();
  const_iterator begin() const;
  const_iterator end() const;
  const_iterator cbegin() const;
  const_iterator cend() const;

 public:
  template <typename T, typename... ARGS>
  std::pair<std::shared_ptr<T>, bool> try_RegisterVar(const std::string &name,
                                                      ARGS &&... args);

  template <typename T>
  std::pair<std::shared_ptr<T> const, bool> get_RegisteredVar(
      const std::string &name) const;

  template <typename T>
  std::pair<std::shared_ptr<T>, bool> get_MutableRegisteredVar(
      const std::string &name);

  void clear() { var_repository_.clear(); }

  void remove_Var(const std::string &name) {
    auto res_find = var_repository_.find(name);
    if (res_find != var_repository_.end()) {
      var_repository_.erase(res_find);
    }
  }

 private:
  template <typename T, typename... ARGS>
  std::pair<std::shared_ptr<T>, bool> insert(const std::string &name,
                                             ARGS &&... args);

 private:
  std::unordered_map<std::string, Variable> var_repository_;

 public:
  Any_Repository() = default;
  ~Any_Repository() = default;
};

/**
 * @brief Defination of class const_Any_Repository_iterator
 */
class const_Any_Repository_iterator {
 public:
  using value_type = typename Any_Repository::value_type;
  using difference_type = ptrdiff_t;
  using iterator_catetory = std::forward_iterator_tag;
  using pointer = value_type *;
  using reference = value_type &;
  using list_iterator_type = typename Any_Repository::ListType::const_iterator;

  const_Any_Repository_iterator() = default;

  const_Any_Repository_iterator(const size_t &any_size,
                                const list_iterator_type &itr_list,
                                const Any_Repository *ptr_any);

  const value_type &operator*() const;

  const value_type *operator->() const;

  const_Any_Repository_iterator &operator++();
  const_Any_Repository_iterator operator++(int);

  bool operator==(const const_Any_Repository_iterator &rhs) const;
  bool operator!=(const const_Any_Repository_iterator &rhs) const;

 protected:
  size_t any_size_;
  list_iterator_type itr_list_;
  const Any_Repository *ptr_any_ = nullptr;

  void increment();
};

/**
 * @brief Defination of class Any_Repository_iterator
 */
class Any_Repository_iterator : public const_Any_Repository_iterator {
 public:
  using value_type = typename Any_Repository::value_type;
  using difference_type = ptrdiff_t;
  using iterator_catetory = std::forward_iterator_tag;
  using pointer = value_type *;
  using reference = value_type &;
  using list_iterator_type = typename Any_Repository::ListType::iterator;

  Any_Repository_iterator() = default;
  Any_Repository_iterator(const size_t &any_size,
                          const list_iterator_type &itr_list,
                          const Any_Repository *ptr_any);

  value_type &operator*();
  value_type *operator->();

  Any_Repository_iterator &operator++();
  Any_Repository_iterator operator++(int);
};

}  // namespace utility
}  // namespace Themis

#include "any_repository.tcc"
