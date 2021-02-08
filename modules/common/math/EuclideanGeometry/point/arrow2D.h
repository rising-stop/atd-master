#ifndef PLANNING_ARROW2_H_
#define PLANNING_ARROW2_H_

#include "modules/common/math/EuclideanGeometry/point/point2D.h"

namespace Themis {
namespace utility {

class Arrow2D : public Point2D {
 public:
  void set_ThetaFromPoint(const Point2D&);

  PROTECTED_MEMBER(double, theta_)

 public:
  Arrow2D() = default;
  virtual ~Arrow2D() = default;
};

}  // namespace planning
}  // namespace Themis

#endif