#ifndef PLANNING_SL_POINT2D_H_
#define PLANNING_SL_POINT2D_H_

#include "modules/common/math/EuclideanGeometry/point/point2D.h"

namespace Themis {
namespace utility {

class SLPoint2D : public Point2D {
  PROTECTED_MEMBER(double, s_)
  PROTECTED_MEMBER(double, l_)

 public:
  SLPoint2D() = default;
  virtual ~SLPoint2D() = default;
};

}  // namespace planning
}  // namespace Themis

#endif