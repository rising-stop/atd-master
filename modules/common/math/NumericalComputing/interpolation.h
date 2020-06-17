#ifndef INTERPOLATION_H
#define INTERPOLATION_H

#include <vector>

#include "modules/common/math/EuclideanGeometry/point/point2D.h"

namespace atd {
namespace utility {

class Interpolation {
 public:
  static double NewtonInterp(const std::vector<Point2D>&, const double&);

  static std::vector<double> NewtonInterp(const std::vector<Point2D>&,
                                          const std::vector<double>&);

  static double LinearInterp(const std::vector<Point2D>&, const double&);

  static std::vector<double> LinearInterp(const std::vector<Point2D>&,
                                          const std::vector<double>&);

  static double QuadraticInterp(const std::vector<Point2D>&, const double&);

  static std::vector<double> QuadraticInterp(const std::vector<Point2D>&,
                                             const std::vector<double>&);

 public:
  Interpolation() = delete;
};

}  // namespace planning
}  // namespace atd

#endif  // INTERPOLATION_H
