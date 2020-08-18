#ifndef PLANNING_POINT2D_H_
#define PLANNING_POINT2D_H_

#include <ostream>
#include <vector>

#include "modules/common/utility/class_support.h"
#include "modules/common/math/EuclideanGeometry/point/vectorXD.h"

namespace atd {
namespace utility {

class Point2D {
 public:
  void operator=(const Point2D&);

  double DistanceTo(const Point2D&) const;
  Vector2D VectorTo(const Point2D&) const;
  double SlopeTo(const Point2D&) const;

  PROTECTED_MEMBER(double, x_)
  PROTECTED_MEMBER(double, y_)

 public:
  Point2D() = default;
  Point2D(const double& x, const double& y);
  Point2D(const Point2D&);
  Point2D(Point2D&&);
  virtual ~Point2D() = default;

 public:
  friend std::ostream& operator<<(std::ostream&, const Point2D&);
};

}  // namespace planning
}  // namespace atd

#endif