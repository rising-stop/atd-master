#ifndef PLANNING_MONOTONIC_DISCRETE_LINE2D_H_
#define PLANNING_MONOTONIC_DISCRETE_LINE2D_H_

#include "discrete_line2D.h"

namespace atd {
namespace utility {

class MonotonicDiscreteLine2D : public DiscreteLine2D<Point2D> {
 public:
  void clear();
  std::pair<bool, std::pair<typename DiscreteLine2D<Point2D>::const_iterator,
                            typename DiscreteLine2D<Point2D>::const_iterator>>
  get_AdjacentPoint(double) const;

  std::string get_DebugString() const;

  void PointRearrangement();

 private:
  bool flag_is_sorted_ = false;

 public:
  MonotonicDiscreteLine2D() = default;
  virtual ~MonotonicDiscreteLine2D() = default;
};

}  // namespace utility
}  // namespace atd

#endif