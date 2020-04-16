#ifndef PLANNING_NUMERICAL_INTEGRATION_H_
#define PLANNING_NUMERICAL_INTEGRATION_H_

#include <functional>

namespace atd {
namespace planning {
namespace common {

class NumericalIntegration {
 public:
  static double Midpoint(const double&, const double&,
                         const std::function<double(const double&)>&,
                         unsigned int subsection_num = 1);

  static double Trapezium(const double&, const double&,
                          const std::function<double(const double&)>&,
                          unsigned int subsection_num = 1);

  static double Simpson(const double&, const double&,
                        const std::function<double(const double&)>&,
                        unsigned int subsection_num = 1);

 public:
  NumericalIntegration() = delete;
  ~NumericalIntegration() = default;
};

}  // namespace common
}  // namespace planning
}  // namespace atd

#endif