#ifndef PLANNING_LEAST_SQUARE_H_
#define PLANNING_LEAST_SQUARE_H_

#include <vector>

#include "modules/common/math/EuclideanGeometry/line/polynomial2D.h"
#include "modules/common/math/EuclideanGeometry/point/point2D.h"

namespace atd {
namespace utility {

template <int ORDER>
class LeastSquare2D {
 public:
  bool deal_UpdateSources(const std::vector<Point2D>&,
                          const std::vector<double>&);

  bool deal_Fitting();

  const Polynomial& get_FitttingRes() const;

 private:
  Polynomial get_OrthogonalPolynomial(const Polynomial&, const Polynomial&);
  Polynomial get_OrthogonalPolynomialOneOrder();
  Polynomial get_OrthogonalPolynomialZeroOrder();

  inline double get_Alpha(const Polynomial&) const;
  inline double get_Beta(const Polynomial&, const Polynomial&) const;

  inline double get_CoefficientA(const Polynomial&) const;

  static const size_t PK_SIZE_ = ORDER + 1;
  std::vector<Polynomial> pk_;
  Polynomial least_square_polynomial_;
  std::vector<std::pair<double, Point2D>> sources_;

 public:
  LeastSquare2D() = default;
  LeastSquare2D(const std::vector<Point2D>&, const std::vector<double>&);
};

}  // namespace planning
}  // namespace atd

#include "least_square.tcc"

#endif