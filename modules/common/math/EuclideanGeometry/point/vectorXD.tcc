#include <cmath>
#include <limits>
#include <stdexcept>

#include "vectorXD.h"

namespace Themis {
namespace utility {

template <int ORDER>
bool VectorXD<ORDER>::empty() const {
  return elements_.empty();
}

template <int ORDER>
int VectorXD<ORDER>::get_Order() const {
  return ORDER;
}

template <int ORDER>
void VectorXD<ORDER>::set_Value(
    const std::initializer_list<double>& init_list) {
  if (init_list.size() != ORDER) {
    throw std::runtime_error("init list is not compatiable with vector order.");
  }
  elements_.clear();
  for (auto item4loop : init_list) {
    elements_.push_back(item4loop);
  }
}

template <int ORDER>
void VectorXD<ORDER>::set_Value(const std::vector<double>& init_list) {
  if (init_list.size() != ORDER) {
    throw std::runtime_error("init list is not compatiable with vector order.");
  }
  elements_ = init_list;
}

template <int ORDER>
double VectorXD<ORDER>::at(const int& index) const {
  return elements_.at(index);
}

template <int ORDER>
double& VectorXD<ORDER>::operator[](const int& index) {
  return elements_[index];
}

template <int ORDER>
VectorXD<ORDER>& VectorXD<ORDER>::operator=(const VectorXD<ORDER>& target) {
  if (target.get_Order() != ORDER) {
    throw std::runtime_error(
        "target vector is not compatiable with the self ORDER");
  }
  elements_.clear();
  for (int index4loop = 0; index4loop < ORDER; index4loop++) {
    elements_.push_back(target.at(index4loop));
  }
  return *this;
}

template <int ORDER>
VectorXD<ORDER> VectorXD<ORDER>::operator+(
    const VectorXD<ORDER>& target) const {
  std::vector<double> item_list;
  for (int index4loop = 0; index4loop < ORDER; index4loop++) {
    item_list.push_back(elements_.at(index4loop) + target.at(index4loop));
  }
  return VectorXD<ORDER>(item_list);
}

template <int ORDER>
VectorXD<ORDER>& VectorXD<ORDER>::operator+=(const VectorXD& target) {
  for (int index4loop = 0; index4loop < ORDER; index4loop++) {
    elements_[index4loop] += target.at(index4loop);
  }
  return *this;
}

template <int ORDER>
VectorXD<ORDER> VectorXD<ORDER>::operator-(
    const VectorXD<ORDER>& target) const {
  std::vector<double> item_list;
  for (int index4loop = 0; index4loop < ORDER; index4loop++) {
    item_list.push_back(elements_.at(index4loop) - target.at(index4loop));
  }
  return VectorXD<ORDER>(item_list);
}

template <int ORDER>
VectorXD<ORDER>& VectorXD<ORDER>::operator-=(const VectorXD<ORDER>& target) {
  for (int index4loop = 0; index4loop < ORDER; index4loop++) {
    elements_[index4loop] -= target.at(index4loop);
  }
  return *this;
}

template <int ORDER>
VectorXD<ORDER> VectorXD<ORDER>::operator*(const double& incrmt) const {
  std::vector<double> item_list;
  for (int index4loop = 0; index4loop < ORDER; index4loop++) {
    item_list.push_back(elements_.at(index4loop) * incrmt);
  }
  return VectorXD<ORDER>(item_list);
}

template <int ORDER>
VectorXD<ORDER>& VectorXD<ORDER>::operator*=(const double& incrmt) {
  for (int index4loop = 0; index4loop < ORDER; index4loop++) {
    elements_[index4loop] *= incrmt;
  }
  return *this;
}

template <int ORDER>
double VectorXD<ORDER>::norm() const {
  double norm_base = 0;
  for (auto item4loop : elements_) {
    norm_base += item4loop * item4loop;
  }
  return sqrt(norm_base);
}

template <int ORDER>
double VectorXD<ORDER>::inner_product(const VectorXD<ORDER>& target) const {
  double product_base = 0;
  for (int index4loop = 0; index4loop < ORDER; index4loop++) {
    product_base += elements_.at(index4loop) * target.at(index4loop);
  }
  return product_base;
}

template <int ORDER>
double VectorXD<ORDER>::cross_product(const VectorXD<2>& target) const {
  if (ORDER != 2) {
    std::logic_error("cross_product do not exist in current ORDER");
  }
  return elements_.at(0) * target.at(1) - elements_.at(1) * target.at(0);
}

template <int ORDER>
VectorXD<3> VectorXD<ORDER>::cross_product(const VectorXD<3>& target) const {
  if (ORDER != 3) {
    std::logic_error("cross_product do not exist in current ORDER");
  }
  double x = elements_.at(1) * target.at(2) - target.at(1) * elements_.at(2),
         y = target.at(0) * elements_.at(2) - elements_.at(0) * target.at(2),
         z = elements_.at(0) * target.at(1) - target.at(0) * elements_.at(1);

  return VectorXD<3>{x, y, z};
}

template <int ORDER>
bool VectorXD<ORDER>::is_Collinear(const VectorXD<ORDER>& target) const {
  if (ORDER < 2) {
    throw std::runtime_error("vector size must be greater than 1");
  }
  if (fabs(inner_product(target)) < 0.0001) {
    return true;
  }
  return false;
}

template <int ORDER>
VectorXD<ORDER>::VectorXD(const std::initializer_list<double>& target) {
  this->set_Value(target);
}

template <int ORDER>
VectorXD<ORDER>::VectorXD(const std::vector<double>& target) {
  this->set_Value(target);
}

template <int ORDER>
VectorXD<ORDER>::VectorXD(const VectorXD<ORDER>& target) {
  *this = target;
}

}  // namespace planning
}  // namespace Themis