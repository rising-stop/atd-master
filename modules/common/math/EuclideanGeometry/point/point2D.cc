#include "point2D.h"
#include "modules/common/common_support/numerical_support.h"

#include <cmath>

namespace atd {
namespace utility {

Point2D::Point2D(const double& x, const double& y) : x_(x), y_(y) {}

Point2D::Point2D(const Point2D& target) {
  x_ = target.get_x_();
  y_ = target.get_y_();
}

Point2D::Point2D(Point2D&& target) {
  x_ = target.get_x_();
  y_ = target.get_y_();
}

void Point2D::operator=(const Point2D& target) {
  x_ = target.get_x_();
  y_ = target.get_y_();
}

double Point2D::DistanceTo(const Point2D& point) const {
  return sqrt((x_ - point.get_x_()) * (x_ - point.get_x_()) +
              (y_ - point.get_y_()) * (y_ - point.get_y_()));
}

Vector2D Point2D::VectorTo(const Point2D& point) const {
  Vector2D vector_res;
  vector_res[0] = point.get_x_() - get_x_();
  vector_res[1] = point.get_y_() - get_y_();
  return vector_res;
}

double Point2D::SlopeTo(const Point2D& target) const {
  if (fabs(target.get_x_() - get_x_()) < SYSTEM_PRECISION) {
    return std::numeric_limits<double>::infinity();
  }
  return (target.get_y_() - get_y_()) / (target.get_x_() - get_x_());
}

std::ostream& operator<<(std::ostream& ostm, const Point2D& point2d) {
  ostm << "(" << point2d.get_x_() << ", " << point2d.get_y_() << ")";
  return ostm;
}

}  // namespace planning
}  // namespace atd