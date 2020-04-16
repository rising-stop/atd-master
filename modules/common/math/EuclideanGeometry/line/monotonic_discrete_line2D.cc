#include "monotonic_discrete_line2D.h"

#include <algorithm>
#include <sstream>
#include <stdexcept>

namespace atd {
namespace planning {
namespace common {

void MonotonicDiscreteLine2D::clear() {
  flag_is_sorted_ = false;
  point_storage_.clear();
}

std::pair<bool, std::pair<typename DiscreteLine2D<Point2D>::const_iterator,
                          typename DiscreteLine2D<Point2D>::const_iterator>>
MonotonicDiscreteLine2D::get_AdjacentPoint(double x) const {
  if (point_storage_.size() < 2) {
    throw std::runtime_error("too few samples to get interpolate value");
  }
  if (x < point_storage_.front().get_x_()) {
    return std::make_pair(
        false, std::make_pair(point_storage_.end(), point_storage_.end()));
  }
  if (x > point_storage_.back().get_x_()) {
    return std::make_pair(false, std::make_pair(point_storage_.end() - 1,
                                                point_storage_.end() - 1));
  }
  for (auto itr4loop = point_storage_.begin() + 1;
       itr4loop != point_storage_.end(); itr4loop++) {
    if (x <= itr4loop->get_x_()) {
      return std::make_pair(true, std::make_pair(itr4loop - 1, itr4loop));
    }
  }
  return std::make_pair(
      false, std::make_pair(point_storage_.end(), point_storage_.end()));
}

void MonotonicDiscreteLine2D::PointRearrangement() {
  if (flag_is_sorted_) {
    return;
  }
  std::sort(point_storage_.begin(), point_storage_.end(),
            [](const Point2D& lpoint, const Point2D& rpoint) -> bool {
              return lpoint.get_x_() < rpoint.get_x_();
            });
  flag_is_sorted_ = true;
}

std::string MonotonicDiscreteLine2D::get_DebugString() const {
  std::stringstream sstm;
  if (point_storage_.empty()) {
    sstm << "empty storage";
  }
  for (auto point4loop : point_storage_) {
    sstm << "(" << point4loop.get_x_() << ", " << point4loop.get_y_() << ") ";
  }
  return sstm.str();
}

}  // namespace common
}  // namespace planning
}  // namespace atd