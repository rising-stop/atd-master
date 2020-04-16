#ifndef PLANNING_DISCRETE_LINE2D_H_
#define PLANNING_DISCRETE_LINE2D_H_

#include <stddef.h>

#include <deque>
#include <vector>

#include "modules/common/math/EuclideanGeometry/point/point2D.h"

namespace atd {
namespace planning {
namespace common {

template <typename TYPE>
class DiscreteLine2D {
 public:
  typedef
      typename std::enable_if<std::is_base_of<Point2D, TYPE>::value, TYPE>::type
          POINT_TYPE;
  typedef size_t size_type;
  typedef typename std::deque<POINT_TYPE>::iterator iterator;
  typedef typename std::deque<POINT_TYPE>::const_iterator const_iterator;
  typedef typename std::deque<POINT_TYPE>::reverse_iterator reverse_iterator;
  typedef typename std::deque<POINT_TYPE>::const_reverse_iterator
      const_reverse_iterator;

 public:
  void clone_from(const std::vector<POINT_TYPE>&);
  void clone_from(const std::deque<POINT_TYPE>&);
  void clone_from(const std::initializer_list<POINT_TYPE>&);

  std::pair<typename std::deque<POINT_TYPE>::const_iterator, double>
  get_ClosestDistance(const POINT_TYPE&) const;

  std::pair<typename std::deque<POINT_TYPE>::const_iterator, double>
  get_Projection(const POINT_TYPE&) const;

 protected:
  std::deque<POINT_TYPE> point_storage_;

 public:
  DiscreteLine2D() = default;
  DiscreteLine2D(const DiscreteLine2D&) = default;
  DiscreteLine2D(DiscreteLine2D&&) = default;
  DiscreteLine2D(const std::initializer_list<POINT_TYPE>&);
  DiscreteLine2D(const std::vector<POINT_TYPE>&);
  DiscreteLine2D(const std::deque<POINT_TYPE>&);
  virtual ~DiscreteLine2D() = default;

  std::deque<POINT_TYPE>* get_Mutable_PointSet();
  const std::deque<POINT_TYPE>& get_PointSet() const;
};

}  // namespace common
}  // namespace planning
}  // namespace atd

#include "discrete_line2D.tcc"

#endif