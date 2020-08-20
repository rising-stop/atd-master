#include "basicgeometry.h"

#include <algorithm>
#include <cmath>
#include <memory>

#include "modules/common/math/mathcommon.h"

namespace atd {
namespace utility {

double BasicGeometry::Distance2LineSegment2D(
    const Point2D& point, const std::pair<Point2D, Point2D>& segment) {
  auto vector_segment = segment.first.VectorTo(segment.second),
       vector_eage = segment.first.VectorTo(point);

  return fabs(vector_segment.inner_product(vector_eage) /
              vector_segment.norm());
}

/**
 * @brief Get curvature using three points
 * @return curvature
 */

double BasicGeometry::ThreePointsCurvature(const Point2D& a, const Point2D& b,
                                           const Point2D& c) {
  double sqr_a = a.DistanceTo(b), sqr_b = a.DistanceTo(c),
         sqr_c = b.DistanceTo(c);
  if ((fabs(sqr_a) < SYSTEM_PRECISION) || (fabs(sqr_b) < SYSTEM_PRECISION) ||
      (fabs(sqr_c) < SYSTEM_PRECISION)) {
    throw std::runtime_error(
        "ThreePointsCurvature: abnormal point are inputed");
  }

  double square_curvature =
      (2 * sqr_b * sqr_c + 2 * sqr_a * sqr_c + 2 * sqr_b * sqr_a -
       sqr_a * sqr_a - sqr_b * sqr_b - sqr_c * sqr_c) /
      (sqr_a * sqr_b * sqr_c);

  return sqrt(square_curvature);
}

// std::pair<std::pair<int, double>, double> BasicGeometry::NumericRadius(
//     const std::vector<double>& x_list, const std::vector<double>& y_list) {
//   /* curvature -- large interval*/
//   const double max_radius = 10000.0;
//   if (x_list.size() != y_list.size()) {
//     throw std::logic_error("NumericRadius: not equal x y size");
//   }
//   std::vector<double> radius_list;
//   if (x_list.size() > 21) {
//     for (unsigned int index4loop = 0; index4loop != x_list.size() - 21;
//          index4loop++) {
//       double tmp_r =
//           1 / BasicGeometry::ThreePointsCurvature(
//                   x_list.at(index4loop), y_list.at(index4loop),
//                   x_list.at(index4loop + 10), y_list.at(index4loop + 10),
//                   x_list.at(index4loop + 20), y_list.at(index4loop + 20));
//       if (tmp_r > max_radius) {
//         tmp_r = max_radius;
//       }
//       radius_list.push_back(tmp_r);
//     }
//   } else {
//     if (x_list.size() < 4) {
//       radius_list.push_back(max_radius);
//     } else {
//       for (unsigned int index4loop = 0; index4loop != x_list.size() - 3;
//            index4loop++) {
//         double tmp_r =
//             1 / BasicGeometry::ThreePointsCurvature(
//                     x_list.at(index4loop), y_list.at(index4loop),
//                     x_list.at(index4loop + 1), y_list.at(index4loop + 1),
//                     x_list.at(index4loop + 2), y_list.at(index4loop + 2));
//         if (tmp_r > max_radius) {
//           tmp_r = max_radius;
//         }
//         radius_list.push_back(tmp_r);
//       }
//     }
//   }

//   double avg_radius =
//       std::accumulate(radius_list.begin(), radius_list.end(), 0) /
//       radius_list.size();
//   auto itr_min_radius =
//       std::min_element(radius_list.begin(), radius_list.end());

//   int min_index = itr_min_radius - radius_list.begin();
//   double min_radius = *itr_min_radius;
//   /* curvature -- large interval*/

//   return std::make_pair(std::make_pair(min_index, min_radius), avg_radius);
// }

Point2D BasicGeometry::CircleCenter(const Point2D& p1, const Point2D& p2,
                                    const Point2D& p3) {
  if ((fabs(p1.DistanceTo(p2)) < SYSTEM_PRECISION) ||
      (fabs(p2.DistanceTo(p3)) < SYSTEM_PRECISION) ||
      (fabs(p3.DistanceTo(p1)) < SYSTEM_PRECISION)) {
    throw std::runtime_error(
        "ThreePointsCurvature: abnormal point are inputed");
  }
  double x21 = p2.get_x_() - p1.get_x_();
  double y21 = p2.get_y_() - p1.get_y_();
  double x32 = p3.get_x_() - p2.get_x_();
  double y32 = p3.get_y_() - p2.get_y_();

  double xy21 = p2.get_x_() * p2.get_x_() - p1.get_x_() * p1.get_x_() +
                p2.get_y_() * p2.get_y_() - p1.get_y_() * p1.get_y_();
  double xy32 = p3.get_x_() * p3.get_x_() - p2.get_x_() * p2.get_x_() +
                p3.get_y_() * p3.get_y_() - p2.get_y_() * p2.get_y_();
  double y0 = (x32 * xy21 - x21 * xy32) / 2 * (y21 * x32 - y32 * x21);
  double x0 = (xy21 - 2 * y0 * y21) / (2.0 * x21);
  return Point2D(x0, y0);
}

// BasicPoint BasicGeometry::TangentDirection(const BasicPoint& p1,
//                                            const BasicPoint& tangent_point,
//                                            const BasicPoint& p3) {
//   BasicPoint center = CircleCenter(p1, tangent_point, p3);
//   if (std::isnan(center.get_value_x()) || std::isnan(center.get_value_y())) {
//     return BasicPoint((p3.get_value_x() - p1.get_value_x()),
//                       (p3.get_value_y() - p1.get_value_y()));
//   } else {
//     double center_x = center.get_value_x() - tangent_point.get_value_x();
//     double center_y = center.get_value_y() - tangent_point.get_value_y();
//     if (fabs(center_x) < std::numeric_limits<double>::epsilon()) {
//       double delta_x = p3.get_value_x() - tangent_point.get_value_x();
//       return BasicPoint(delta_x, 0);
//     } else if (fabs(center_y) < std::numeric_limits<double>::epsilon()) {
//       double delta_y = p3.get_value_y() - tangent_point.get_value_y();
//       return BasicPoint(0, delta_y);
//     } else {
//       double delta_y = p3.get_value_y() - tangent_point.get_value_y();
//       double delta_x = -(center_y * delta_y) / center_x;
//       return BasicPoint(delta_x, delta_y);
//     }  // if (fabs(center_x) .....)
//   }    // if (std::isnan ......)
// }

std::pair<bool, Point2D> BasicGeometry::LineSegmentIntersection(
    const std::pair<Point2D, Point2D>& lseg,
    const std::pair<Point2D, Point2D>& rseg) {
  if (std::min(lseg.first.get_x_(), lseg.second.get_x_()) <=
          std::max(rseg.first.get_x_(), rseg.second.get_x_()) &&
      std::max(lseg.first.get_x_(), lseg.second.get_x_()) >=
          std::min(rseg.first.get_x_(), rseg.second.get_x_()) &&
      std::min(lseg.first.get_y_(), lseg.second.get_y_()) <=
          std::max(rseg.first.get_y_(), rseg.second.get_y_()) &&
      std::max(lseg.first.get_y_(), lseg.second.get_y_()) >=
          std::min(rseg.first.get_y_(), rseg.second.get_y_())) {
    double rcross = (rseg.first.VectorTo(rseg.second)
                         .cross_product(rseg.first.VectorTo(lseg.first)) *
                     rseg.first.VectorTo(rseg.second)
                         .cross_product(rseg.first.VectorTo(lseg.second))),
           lcross = (lseg.first.VectorTo(lseg.second)
                         .cross_product(lseg.first.VectorTo(rseg.first)) *
                     lseg.first.VectorTo(lseg.second)
                         .cross_product(lseg.first.VectorTo(rseg.second)));
    if ((rcross < 0) && (lcross < 0)) {
      return std::make_pair(true, IntersectedPoint(lseg, rseg));
    }
  }
  return std::make_pair(false,
                        Point2D(std::numeric_limits<double>::quiet_NaN(),
                                std::numeric_limits<double>::quiet_NaN()));
}

Point2D BasicGeometry::IntersectedPoint(
    const std::pair<Point2D, Point2D>& lseg,
    const std::pair<Point2D, Point2D>& rseg) {
  double k_1 = lseg.first.SlopeTo(lseg.second);
  double k_2 = rseg.first.SlopeTo(rseg.second);

  if ((fabs(k_1 - k_2) < SYSTEM_PRECISION) ||
      (std::isinf(k_1) && std::isinf(k_2))) {
    throw std::logic_error("IntersectedPoint: parallel line do not intersect");
  }

  if (std::isinf(k_1)) {
    return Point2D(lseg.first.get_x_(),
                   (lseg.first.get_x_() - rseg.first.get_x_()) * k_2 +
                       rseg.first.get_y_());
  }
  if (std::isinf(k_2)) {
    return Point2D(rseg.first.get_x_(),
                   (rseg.first.get_x_() - lseg.first.get_x_()) * k_1 +
                       lseg.first.get_y_());
  }

  double res_x = (rseg.first.get_y_() - lseg.first.get_y_() -
                  rseg.first.get_x_() * k_2 + lseg.first.get_x_() * k_1) /
                 (k_1 - k_2);
  double res_y = (res_x - lseg.first.get_x_()) * k_1 + lseg.first.get_y_();
  return Point2D(res_x, res_y);
}

}  // namespace planning
}  // namespace atd
