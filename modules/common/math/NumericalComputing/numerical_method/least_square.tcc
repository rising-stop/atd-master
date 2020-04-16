#include "least_square.h"
#include <algorithm>
#include <stdexcept>

namespace atd {
namespace planning {
namespace common {

template <int ORDER>
LeastSquare2D<ORDER>::LeastSquare2D(const std::vector<Point2D>& source,
                                    const std::vector<double>& weight) {
  if (!deal_UpdateSources(source, weight)) {
    throw std::logic_error("the source is not compatiable with the weight");
  }
}

template <int ORDER>
bool LeastSquare2D<ORDER>::deal_UpdateSources(
    const std::vector<Point2D>& source, const std::vector<double>& weight) {
  if (source.empty() || weight.empty()) {
    return false;
  }
  if (source.size() != weight.size()) {
    return false;
  }
  for (unsigned int index4loop = 0; index4loop < source.size(); index4loop++) {
    sources_.push_back(
        std::make_pair(weight.at(index4loop), source.at(index4loop)));
  }
  return true;
}

template <int ORDER>
bool LeastSquare2D<ORDER>::deal_Fitting() {
  if (sources_.empty()) {
    return false;
  }

  least_square_polynomial_.reset();

  for (unsigned int index_fit = 0; index_fit < PK_SIZE_; index_fit++) {
    if (index_fit == 0) {
      pk_.push_back(get_OrthogonalPolynomialZeroOrder());
    } else if (index_fit == 1) {
      pk_.push_back(get_OrthogonalPolynomialOneOrder());
    } else {
      pk_.push_back(
          get_OrthogonalPolynomial(*(pk_.end() - 1), *(pk_.end() - 2)));
    }
    double coeff_a = get_CoefficientA(pk_.back());
    least_square_polynomial_ += pk_.back() * coeff_a;
  }
  return true;
}

template <int ORDER>
const Polynomial& LeastSquare2D<ORDER>::get_FitttingRes() const {
  if (!least_square_polynomial_.has_param()) {
    throw std::runtime_error("not been fitted yet");
  }
  return least_square_polynomial_;
}

template <int ORDER>
Polynomial LeastSquare2D<ORDER>::get_OrthogonalPolynomial(
    const Polynomial& pk, const Polynomial& pk_minus) {
  Polynomial alpha = Polynomial{1, -get_Alpha(pk)};
  auto former = pk * alpha;
  auto later = pk_minus * get_Beta(pk, pk_minus);
  return (pk * alpha) - (pk_minus * get_Beta(pk, pk_minus));
}

template <int ORDER>
Polynomial LeastSquare2D<ORDER>::get_OrthogonalPolynomialOneOrder() {
  double alpha_0 = get_Alpha(get_OrthogonalPolynomialZeroOrder());
  return Polynomial{1, -alpha_0};
}

template <int ORDER>
Polynomial LeastSquare2D<ORDER>::get_OrthogonalPolynomialZeroOrder() {
  return Polynomial{1};
}

template <int ORDER>
double LeastSquare2D<ORDER>::get_Alpha(const Polynomial& pk) const {
  double top = 0, bottom = 0;
  std::for_each(sources_.begin(), sources_.end(),
                [&](const std::pair<double, Point2D>& item) {
                  top += item.first * item.second.get_x_() *
                         pk.get_PolynomialValue(item.second.get_x_()) *
                         pk.get_PolynomialValue(item.second.get_x_());
                  bottom += item.first *
                            pk.get_PolynomialValue(item.second.get_x_()) *
                            pk.get_PolynomialValue(item.second.get_x_());
                });
  return top / bottom;
}

template <int ORDER>
double LeastSquare2D<ORDER>::get_Beta(const Polynomial& pk,
                                      const Polynomial& pk_minus) const {
  double top = 0, bottom = 0;
  std::for_each(sources_.begin(), sources_.end(),
                [&](const std::pair<double, Point2D>& item) {
                  top += item.first *
                         pk.get_PolynomialValue(item.second.get_x_()) *
                         pk.get_PolynomialValue(item.second.get_x_());
                  bottom += item.first *
                            pk_minus.get_PolynomialValue(item.second.get_x_()) *
                            pk_minus.get_PolynomialValue(item.second.get_x_());
                });
  return top / bottom;
}

template <int ORDER>
inline double LeastSquare2D<ORDER>::get_CoefficientA(
    const Polynomial& pk) const {
  double top = 0, bottom = 0;
  std::for_each(sources_.begin(), sources_.end(),
                [&](const std::pair<double, Point2D>& item) {
                  top += item.first * item.second.get_y_() *
                         pk.get_PolynomialValue(item.second.get_x_());
                  bottom += item.first *
                            pk.get_PolynomialValue(item.second.get_x_()) *
                            pk.get_PolynomialValue(item.second.get_x_());
                });
  return top / bottom;
}

}  // namespace common
}  // namespace planning
}  // namespace atd