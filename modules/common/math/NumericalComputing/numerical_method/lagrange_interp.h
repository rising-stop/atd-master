#ifndef PLANNING_LAGRANGE_INTERP_H_
#define PLANNING_LAGRANGE_INTERP_H_

#include <vector>
#include "modules/common/math/EuclideanGeometry/point/point2D.h"

namespace atd {
namespace utility {

template <int ORDER>
class LagrangeInterp {
 private:
  static const int BASIS_NUM = ORDER + 1;

 public:
  double get_InterpRes(const std::vector<Point2D>&, const double&);
  std::vector<double> get_InterpRes(const std::vector<Point2D>&,
                                    const std::vector<double>&);

 public:
  LagrangeInterp() = default;
};

}  // namespace planning
}  // namespace atd

#include "lagrange_interp.tcc"

#endif