#include "modules/common/math/NumericalComputing/interpolation.h"

#include "modules/common/math/NumericalComputing/numerical_method/lagrange_interp.h"
#include "modules/common/math/NumericalComputing/numerical_method/newton_interp.h"

namespace atd {
namespace planning {
namespace common {

double Interpolation::NewtonInterp(const std::vector<Point2D>& source,
                                   const double& target) {
  NewtonInterp2D newton_interp;
  newton_interp.deal_DifferenceQuotient(source);
  return newton_interp.get_InterpRes(target);
}

std::vector<double> Interpolation::NewtonInterp(
    const std::vector<Point2D>& source, const std::vector<double>& target) {
  NewtonInterp2D newton_interp;
  newton_interp.deal_DifferenceQuotient(source);
  std::vector<double> interp_res;
  for (auto target4loop : target) {
    newton_interp.get_InterpRes(target4loop);
  }
  return interp_res;
}

double Interpolation::LinearInterp(const std::vector<Point2D>& source,
                                   const double& target) {
  LagrangeInterp<1> linear_interp;
  return linear_interp.get_InterpRes(source, target);
}

std::vector<double> Interpolation::LinearInterp(
    const std::vector<Point2D>& source, const std::vector<double>& targets) {
  LagrangeInterp<1> linear_interp;
  return linear_interp.get_InterpRes(source, targets);
}

double Interpolation::QuadraticInterp(const std::vector<Point2D>& source,
                                      const double& target) {
  LagrangeInterp<2> quadratic_interp;
  return quadratic_interp.get_InterpRes(source, target);
}

std::vector<double> Interpolation::QuadraticInterp(
    const std::vector<Point2D>& source, const std::vector<double>& targets) {
  LagrangeInterp<2> quadratic_interp;
  return quadratic_interp.get_InterpRes(source, targets);
}

}  // namespace common
}  // namespace planning
}  // namespace atd
