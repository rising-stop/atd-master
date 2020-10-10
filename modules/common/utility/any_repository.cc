#include "any_repository.h"

namespace atd {
namespace utility {

const_Any_Repository_iterator::const_Any_Repository_iterator(
    const size_t& any_size, const list_iterator_type& itr_list,
    const Any_Repository* ptr_any)
    : any_size_(any_size), itr_list_(itr_list), ptr_any_(ptr_any) {}

const_Any_Repository_iterator& const_Any_Repository_iterator::operator++() {
  increment();
  return *this;
}

const_Any_Repository_iterator const_Any_Repository_iterator::operator++(int) {
  auto itr_old = *this;
  increment();
  return itr_old;
}

const typename const_Any_Repository_iterator::value_type&
const_Any_Repository_iterator::operator*() const {
  return *itr_list_;
}

const typename const_Any_Repository_iterator::value_type*
const_Any_Repository_iterator::operator->() const {
  return &(*itr_list_);
}

bool const_Any_Repository_iterator::operator==(
    const const_Any_Repository_iterator& rhs) const {
  return (any_size_ == rhs.any_size_ && itr_list_ == rhs.itr_list_ &&
          ptr_any_ == rhs.ptr_any_);
}

bool const_Any_Repository_iterator::operator!=(
    const const_Any_Repository_iterator& rhs) const {
  return !(*this == rhs);
}

void const_Any_Repository_iterator::increment() { ++itr_list_; }

Any_Repository_iterator::Any_Repository_iterator(
    const size_t& any_size, const list_iterator_type& itr_list,
    const Any_Repository* ptr_any)
    : const_Any_Repository_iterator(any_size, itr_list, ptr_any) {}

Any_Repository_iterator::value_type& Any_Repository_iterator::operator*() {
  return const_cast<value_type&>(*this->itr_list_);
}

Any_Repository_iterator::value_type* Any_Repository_iterator::operator->() {
  return const_cast<value_type*>(&(*this->itr_list_));
}

Any_Repository_iterator& Any_Repository_iterator::operator++() {
  this->increment();
  return *this;
}
Any_Repository_iterator Any_Repository_iterator::operator++(int) {
  auto itr_old = *this;
  this->increment();
  return itr_old;
}

Any_Repository::iterator Any_Repository::begin() {
  if (var_repository_.empty()) {
    return end();
  }
  return Any_Repository_iterator(var_repository_.size(),
                                 var_repository_.begin(), this);
}

Any_Repository::iterator Any_Repository::end() {
  return Any_Repository_iterator(var_repository_.size(), var_repository_.end(),
                                 this);
}

Any_Repository::const_iterator Any_Repository::begin() const {
  if (var_repository_.empty()) {
    return end();
  }
  return const_Any_Repository_iterator(var_repository_.size(),
                                       var_repository_.cbegin(), this);
}

Any_Repository::const_iterator Any_Repository::end() const {
  return const_Any_Repository_iterator(var_repository_.size(),
                                       var_repository_.cend(), this);
}

Any_Repository::const_iterator Any_Repository::cbegin() const {
  return begin();
}
Any_Repository::const_iterator Any_Repository::cend() const { return end(); }

}  // namespace utility
}  // namespace atd