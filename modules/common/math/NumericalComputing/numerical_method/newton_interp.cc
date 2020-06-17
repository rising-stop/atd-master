#include "modules/common/math/NumericalComputing/numerical_method/newton_interp.h"

#include <stdexcept>

namespace atd {
namespace utility {

bool NewtonInterp2D::deal_DifferenceQuotient(
    const std::vector<Point2D>& source) {
  if (source.empty()) {
    return false;
  }
  for (auto point4loop : source) {
    diff_quo_.push_back(point4loop.get_y_());
    origin_x_.push_back(point4loop.get_x_());
  }

  for (int j = 1; j <= source.size(); j++) {
    for (int i = source.size(); i >= j; i--) {
      diff_quo_[i] =
          (diff_quo_[i] - diff_quo_[i - 1]) / (origin_x_[i] - origin_x_[i - j]);
    }
  }

  return true;
}

double NewtonInterp2D::get_InterpRes(const double& target) const {
  if (diff_quo_.empty() || origin_x_.empty()) {
    throw std::runtime_error("source data is empty");
  }
  int data_size = origin_x_.size() - 1;
  double u = diff_quo_[data_size];

  for (int i = data_size - 1; i >= 0; i--) {
    u = diff_quo_[i] + (target - origin_x_[i]) * u;
  }

  return u;
}

}  // namespace planning
}  // namespace atd