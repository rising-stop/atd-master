#ifndef PLANNING_FUNCTION_FITTING_H_
#define PLANNING_FUNCTION_FITTING_H_

#include <functional>
#include <vector>

#include "modules/common/math/EuclideanGeometry/line/polynomial2D.h"
#include "modules/common/math/EuclideanGeometry/point/point2D.h"

namespace atd {
namespace utility {

class FunctionFitting {
 public:
  template <int ORDER>
  static Polynomial LeastSquareFitting(
      const std::vector<Point2D>&,
      const std::vector<double>& weight = std::vector<double>());

 public:
  FunctionFitting() = delete;
};

}  // namespace planning
}  // namespace atd

#include "function_fitting.tcc"

#endif