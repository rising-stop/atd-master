#ifndef PLANNING_NEWTON_INTERP_H_
#define PLANNING_NEWTON_INTERP_H_

#include <vector>

#include "modules/common/math/EuclideanGeometry/point/point2D.h"

namespace atd {
namespace planning {
namespace common {

class NewtonInterp2D {
 public:
  bool deal_DifferenceQuotient(const std::vector<Point2D>&);
  double get_InterpRes(const double&) const;

 private:
  std::vector<double> origin_x_;
  std::vector<double> diff_quo_;

 public:
  NewtonInterp2D() = default;
  ~NewtonInterp2D() = default;
};

}  // namespace common
}  // namespace planning
}  // namespace atd

#endif