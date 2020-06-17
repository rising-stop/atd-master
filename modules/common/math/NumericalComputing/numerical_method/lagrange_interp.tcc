#include "lagrange_interp.h"
#include <stdexcept>

namespace atd {
namespace utility {

template <int ORDER>
double LagrangeInterp<ORDER>::get_InterpRes(const std::vector<Point2D>& source,
                                            const double& target) {
  if (source.size() != BASIS_NUM) {
    throw std::logic_error(
        "source size is not compatiable with Lagrange order");
  }
  double interp_res = 0;
  for (unsigned int index4loop = 0; index4loop < BASIS_NUM; index4loop++) {
    double basis_function_value = 1;
    for (unsigned int basis_index = 0; basis_index < BASIS_NUM; basis_index++) {
      if (basis_index == index4loop) {
        continue;
      }
      basis_function_value *=
          (target - source.at(basis_index).get_x_()) /
          (source.at(index4loop).get_x_() - source.at(basis_index).get_x_());
    }
    interp_res += source.at(index4loop).get_y_() * basis_function_value;
  }
  return interp_res;
}

template <int ORDER>
std::vector<double> LagrangeInterp<ORDER>::get_InterpRes(
    const std::vector<Point2D>& source, const std::vector<double>& targets) {
  std::vector<double> interp_res;
  for (auto res4loop : targets) {
    interp_res.push_back(get_InterpRes(source, res4loop));
  }
  return interp_res;
}

}  // namespace planning
}  // namespace atd