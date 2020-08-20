#include "polynomial2D.h"

#include <cmath>

#include "modules/common/math/NumericalComputing/numerical_integration.h"
#include "modules/common/math/mathcommon.h"

namespace atd {
namespace utility {

Polynomial::Polynomial(const std::initializer_list<double>& init_list) {
  set_PolynomialParameters(init_list);
}
Polynomial::Polynomial(const std::vector<double>& init_list) {
  set_PolynomialParameters(init_list);
}

Polynomial::Polynomial(const int& order) { set_PolynomialParameters(order); }

void Polynomial::set_has_param() { flag_has_param_ = true; }

bool Polynomial::has_param() const { return flag_has_param_; }

std::vector<double> Polynomial::get_Coefficient() const {
  if (!has_param()) {
    throw std::logic_error("poly contains nothing");
  }
  return source_;
}

void Polynomial::reset() {
  ORDER_ = 0;
  PARAM_SIZE_ = 0;
  source_.clear();
  flag_has_param_ = false;
}

std::vector<double>* Polynomial::get_Mutable_Parameters() {
  set_has_param();
  return &source_;
}

double Polynomial::get_Coefficient(const int& index) const {
  if ((index < 0) || (index > ORDER_)) {
    throw std::logic_error("index is out of range");
  }
  return source_.at(index);
}

void Polynomial::set_PolynomialParameters(
    const std::initializer_list<double>& init_list) {
  ORDER_ = (init_list.size() - 1);
  PARAM_SIZE_ = ORDER_ + 1;
  if (init_list.size() != PARAM_SIZE_) {
    throw std::logic_error("param size wrong, not compatiable with ORDER");
  }
  source_.assign(init_list.begin(), init_list.end());
  set_has_param();
}

void Polynomial::set_PolynomialParameters(
    const std::vector<double>& init_list) {
  ORDER_ = (init_list.size() - 1);
  PARAM_SIZE_ = ORDER_ + 1;
  if (init_list.size() != PARAM_SIZE_) {
    throw std::logic_error("param size wrong, not compatiable with ORDER");
  }
  source_.assign(init_list.begin(), init_list.end());
  set_has_param();
}

void Polynomial::set_PolynomialParameters(const int& order) {
  ORDER_ = order;
  PARAM_SIZE_ = ORDER_ + 1;
  if (order < 0) {
    throw std::runtime_error(
        "polynomial: polynomial order can not be negative");
  }
  source_ = std::vector<double>(PARAM_SIZE_, 0);
  set_has_param();
}

double Polynomial::get_PolynomialValue(const double& x_value) const {
  if (!has_param()) {
    throw std::runtime_error("polynomial parameter empty");
  }
  double multi_base = 1;
  double accumulate_base = source_.at(PARAM_SIZE_ - 1);
  for (int index4loop = 0; index4loop < ORDER_; index4loop++) {
    multi_base *= x_value;
    accumulate_base += multi_base * source_.at(ORDER_ - 1 - index4loop);
  }

  return accumulate_base;
}

Polynomial Polynomial::get_Derivative() const {
  if (ORDER_ == 0) {
    throw std::logic_error("derivative does not exist");
  }
  if (!has_param()) {
    throw std::logic_error("param list is empty");
  }
  std::vector<double> derivative_param;
  for (int order_param = 0; order_param < (PARAM_SIZE_ - 1); order_param++) {
    derivative_param.push_back(source_.at(order_param) *
                               (ORDER_ - order_param));
  }
  return Polynomial(derivative_param);
}

Polynomial Polynomial::get_Integration(const double& const_value) const {
  if (!has_param()) {
    throw std::logic_error("param list is empty");
  }
  std::vector<double> integration_param;
  for (int order_param = 0; order_param < (PARAM_SIZE_ + 1); order_param++) {
    if (integration_param.size() == PARAM_SIZE_) {
      integration_param.push_back(const_value);
      break;
    }
    integration_param.push_back(source_.at(order_param) /
                                static_cast<double>(ORDER_ - order_param + 1));
  }
  return Polynomial(integration_param);
}

double Polynomial::get_Length(const double& lower, const double& upper) const {
  if (!has_param()) {
    throw std::runtime_error("missing base data");
  }
  double line_scale = (upper - lower), resolution = 1.0;
  unsigned int dev_num = 3;
  if (line_scale > 50) {
    dev_num = static_cast<int>((upper - lower) / resolution);
  } else if (line_scale > 5) {
    resolution = 0.5;
    dev_num = static_cast<int>((upper - lower) / resolution);
  }
  if (dev_num > 100){
    dev_num = 100;
  }
  if (dev_num < 3){
    dev_num = 3;
  }
  throw std::runtime_error("get_Length: function not be completed yet");
  return 0;
  // return NumericalIntegration::Simpson(
  //     lower, upper,
  //     [&](const double& x) -> double {
  //       double d = get_Derivative().get_PolynomialValue(x);
  //       return sqrt(d * d + 1);
  //     },
  //     dev_num);
}

int Polynomial::get_HighestDegree() const { return ORDER_; }

Polynomial& Polynomial::operator=(const Polynomial& target) {
  ORDER_ = target.get_HighestDegree();
  PARAM_SIZE_ = ORDER_ + 1;
  source_.assign(target.get_Coefficient().begin(),
                 target.get_Coefficient().end());
  flag_has_param_ = target.has_param();
  return *this;
}

Polynomial Polynomial::operator+(const Polynomial& added) const {
  std::vector<double> new_poly =
      MathCommon::merge_vector_from_end(source_, added.get_Coefficient());
  return Polynomial(new_poly);
}

Polynomial& Polynomial::operator+=(const Polynomial& added) {
  std::vector<double> new_poly =
      MathCommon::merge_vector_from_end(source_, added.get_Coefficient());
  set_PolynomialParameters(new_poly);
  return *this;
}

Polynomial Polynomial::operator-(const Polynomial& subtracted) const {
  std::vector<double> vec_sub;
  auto sub_param = subtracted.get_Coefficient();
  for (auto itr4loop = sub_param.begin(); itr4loop != sub_param.end();
       itr4loop++) {
    vec_sub.push_back(-(*itr4loop));
  }
  std::vector<double> new_poly =
      MathCommon::merge_vector_from_end(source_, vec_sub);
  return Polynomial(new_poly);
}

Polynomial& Polynomial::operator-=(const Polynomial& subtracted) {
  std::vector<double> vec_sub;
  for (auto itr4loop = subtracted.get_Coefficient().begin();
       itr4loop != subtracted.get_Coefficient().end(); itr4loop++) {
    vec_sub.push_back(-(*itr4loop));
  }
  std::vector<double> new_poly =
      MathCommon::merge_vector_from_end(source_, vec_sub);
  set_PolynomialParameters(new_poly);
  return *this;
}

int Polynomial::get_ItemOrder(int index) const { return ORDER_ - index; }

int Polynomial::get_ItemIndex(int order) const { return ORDER_ - order; }

Polynomial Polynomial::operator*(const Polynomial& target) const {
  if (!target.has_param()) {
    throw std::runtime_error("target polynomial contains nothing");
  }
  int target_order = target.get_HighestDegree();
  std::vector<double> coeff_res =
      std::vector<double>(ORDER_ + target_order + 1, 0);
  for (unsigned int index_self = 0; index_self < ORDER_ + 1; index_self++) {
    for (unsigned int index_target = 0; index_target < target_order + 1;
         index_target++) {
      int index_comb = target_order + ORDER_ - get_ItemOrder(index_self) -
                       target.get_ItemOrder(index_target);
      coeff_res[index_comb] +=
          source_.at(index_self) * target.get_Coefficient(index_target);
    }
  }
  return Polynomial(coeff_res);
}

Polynomial& Polynomial::operator*=(const Polynomial& target) {
  if (!target.has_param()) {
    throw std::runtime_error("target polynomial contains nothing");
  }
  int target_order = target.get_HighestDegree();
  std::vector<double> coeff_res =
      std::vector<double>(ORDER_ + target_order + 1, 0);
  for (unsigned int index_self = 0; index_self < ORDER_ + 1; index_self++) {
    for (unsigned int index_target = 0; index_target < target_order + 1;
         index_target++) {
      int index_comb = target_order + ORDER_ - get_ItemOrder(index_self) -
                       target.get_ItemOrder(index_target);
      coeff_res[index_comb] +=
          source_.at(index_self) * target.get_Coefficient(index_target);
    }
  }
  set_PolynomialParameters(coeff_res);
  return *this;
}

Polynomial Polynomial::operator*(const double& target) const {
  std::vector<double> coeff_res = source_;
  for (auto& coeff4loop : coeff_res) {
    coeff4loop *= target;
  }
  return Polynomial(coeff_res);
}

Polynomial& Polynomial::operator*=(const double& target) {
  std::vector<double> coeff_res = source_;
  for (auto& coeff4loop : coeff_res) {
    coeff4loop *= target;
  }
  set_PolynomialParameters(coeff_res);
  return *this;
}

std::ostream& operator<<(std::ostream& ostm, const Polynomial& poly) {
  if (!poly.has_param()) {
    return ostm;
  }
  int order = poly.get_HighestDegree();
  double current_coeff = poly.get_Coefficient(poly.get_ItemIndex(order));
  if (order == 0) {
    ostm << current_coeff;
    return ostm;
  }
  for (int order4loop = order; order4loop > 0; order4loop--) {
    double current_coeff = poly.get_Coefficient(poly.get_ItemIndex(order4loop));
    if (order4loop == order) {
      ostm << current_coeff << " x^" << order4loop;
      continue;
    }
    if (current_coeff >= 0) {
      ostm << " + " << fabs(current_coeff) << " x^" << order4loop;
    } else {
      ostm << " - " << fabs(current_coeff) << " x^" << order4loop;
    }
  }
  double zero_order = poly.get_Coefficient(poly.get_ItemIndex(0));
  if (zero_order >= 0) {
    ostm << " + " << fabs(zero_order);
  } else {
    ostm << " - " << fabs(zero_order);
  }
  return ostm;
}

}  // namespace planning
}  // namespace atd
