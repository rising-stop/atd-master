#ifndef PLANNING_BASICGEOMETRY_H
#define PLANNING_BASICGEOMETRY_H

#include "modules/common/math/EuclideanGeometry/line/discrete_line2D.h"
#include "modules/common/math/EuclideanGeometry/point/point2D.h"

namespace atd {
namespace utility {

class BasicGeometry {
 public:
  static double Distance2LineSegment2D(const Point2D&,
                                       const std::pair<Point2D, Point2D>&);

  static double ThreePointsCurvature(const Point2D&, const Point2D&,
                                     const Point2D&);

  static Point2D CircleCenter(const Point2D&, const Point2D&, const Point2D&);

  static std::pair<bool, Point2D> LineSegmentIntersection(
      const std::pair<Point2D, Point2D>&, const std::pair<Point2D, Point2D>&);

  static Point2D IntersectedPoint(const std::pair<Point2D, Point2D>&,
                                  const std::pair<Point2D, Point2D>&);

 public:
  BasicGeometry() = delete;
};

}  // namespace utility
}  // namespace atd

#endif  // BASICGEOMETRY_H
