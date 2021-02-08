#include "modules/common/math/NumericalComputing/function_fitting.h"
#include <stdexcept>
#include "modules/common/math/NumericalComputing/numerical_method/least_square.h"

namespace Themis {
namespace utility {

template <int ORDER>
Polynomial FunctionFitting::LeastSquareFitting(
    const std::vector<Point2D>& point_source,
    const std::vector<double>& weight) {
  if (point_source.empty()) {
    throw std::runtime_error("least square fitting: empty source");
  }
  std::vector<double> weight_source;
  if (weight.empty()) {
    weight_source = std::vector<double>(point_source.size(), 1.0);
  }
  if (weight_source.size() != point_source.size()) {
    throw std::runtime_error(
        "least square fitting: different size between point_source and "
        "weight_source");
  }
  LeastSquare2D<ORDER> least_square;
  least_square.deal_UpdateSources(point_source, weight_source);
  if (least_square.deal_Fitting()) {
    return least_square.get_FitttingRes();
  }
  return Polynomial();
}

}  // namespace planning
}  // namespace Themis