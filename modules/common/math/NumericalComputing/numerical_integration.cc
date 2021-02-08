#include "numerical_integration.h"

#include <algorithm>
#include <stdexcept>

#include "modules/common/math/mathcommon.h"

namespace Themis {
namespace utility {

double Midpoint(const double& lower, const double& upper,
                const std::function<double(const double&)>& func,
                unsigned int subsection_num = 1) {
  std::vector<double> key_point_list =
      MathCommon::linspace(lower, upper, subsection_num);
  if (key_point_list.size() != (subsection_num + 1)) {
    throw std::logic_error("wrong deviding subsection");
  }
  std::vector<double> middle_list = MathCommon::middle(key_point_list);
  double estimated_intg = 0;
  for (auto data : middle_list) {
    estimated_intg += func(data);
  }
  return estimated_intg * (upper - lower) / subsection_num;
}

double Trapezium(const double& lower, const double& upper,
                 const std::function<double(const double&)>& func,
                 unsigned int subsection_num = 1) {
  std::vector<double> key_point_list =
      MathCommon::linspace(lower, upper, subsection_num);
  if (key_point_list.size() != (subsection_num + 1)) {
    throw std::logic_error("wrong deviding subsection");
  }
  double estimated_intg = func(upper) + func(lower);
  std::for_each(key_point_list.begin() + 1, key_point_list.end() - 1,
                [&](const double& data) {
                  estimated_intg += static_cast<double>(2) * func(data);
                });
  return estimated_intg * (upper - lower) / subsection_num /
         static_cast<double>(2);
}

double Simpson(const double& lower, const double& upper,
               const std::function<double(const double&)>& func,
               unsigned int subsection_num = 1) {
  std::vector<double> key_point_list =
      MathCommon::linspace(lower, upper, subsection_num);
  if (key_point_list.size() != (subsection_num + 1)) {
    throw std::logic_error("wrong deviding subsection");
  }
  std::vector<double> middle_list = MathCommon::middle(key_point_list);
  double estimated_intg = func(upper) + func(lower);
  std::for_each(key_point_list.begin() + 1, key_point_list.end() - 1,
                [&](const double& data) {
                  estimated_intg += static_cast<double>(2) * func(data);
                });
  std::for_each(middle_list.begin(), middle_list.end(),
                [&](const double& data) {
                  estimated_intg += static_cast<double>(4) * func(data);
                });
  return estimated_intg * (upper - lower) / subsection_num /
         static_cast<double>(6);
}

}  // namespace planning
}  // namespace Themis