#include <algorithm>

#include "discrete_line2D.h"

namespace Themis {
namespace utility {

template <typename TYPE>
DiscreteLine2D<TYPE>::DiscreteLine2D(const std::vector<POINT_TYPE>& target) {
  clone_from(target);
}

template <typename TYPE>
DiscreteLine2D<TYPE>::DiscreteLine2D(const std::deque<POINT_TYPE>& target) {
  clone_from(target);
}

template <typename TYPE>
DiscreteLine2D<TYPE>::DiscreteLine2D(
    const std::initializer_list<POINT_TYPE>& init_list) {
  clone_from(init_list);
}

template <typename TYPE>
void DiscreteLine2D<TYPE>::clone_from(
    const std::vector<typename DiscreteLine2D<TYPE>::POINT_TYPE>& target) {
  point_storage_.assign(target.get_PointSet().cbegin(),
                        target.get_PointSet().cend());
}

template <typename TYPE>
void DiscreteLine2D<TYPE>::clone_from(
    const std::deque<typename DiscreteLine2D<TYPE>::POINT_TYPE>& target) {
  point_storage_.assign(target.get_PointSet().cbegin(),
                        target.get_PointSet().cend());
}

template <typename TYPE>
void DiscreteLine2D<TYPE>::clone_from(
    const std::initializer_list<typename DiscreteLine2D<TYPE>::POINT_TYPE>&
        init_list) {
  point_storage_.assign(init_list.begin(), init_list.end());
}

template <typename TYPE>
std::pair<typename std::deque<
              typename DiscreteLine2D<TYPE>::POINT_TYPE>::const_iterator,
          double>
DiscreteLine2D<TYPE>::get_ClosestDistance(
    const typename DiscreteLine2D<TYPE>::POINT_TYPE& target) const {
  auto itr_min = std::min_element(
      get_PointSet().cbegin(), get_PointSet().cend(),
      [&](const typename DiscreteLine2D<TYPE>::POINT_TYPE& litem,
          const typename DiscreteLine2D<TYPE>::POINT_TYPE& ritem) -> bool {
        return litem.DistanceTo(target) < ritem.DistanceTo(target);
      });
  return std::make_pair(itr_min, itr_min->DistanceTo(target));
}

template <typename TYPE>
std::pair<typename std::deque<
              typename DiscreteLine2D<TYPE>::POINT_TYPE>::const_iterator,
          double>
DiscreteLine2D<TYPE>::get_Projection(
    const typename DiscreteLine2D<TYPE>::POINT_TYPE& target) const {
  return get_ClosestDistance(target);
}

template <typename TYPE>
std::deque<typename DiscreteLine2D<TYPE>::POINT_TYPE>*
DiscreteLine2D<TYPE>::get_Mutable_PointSet() {
  return &point_storage_;
}

template <typename TYPE>
const std::deque<typename DiscreteLine2D<TYPE>::POINT_TYPE>&
DiscreteLine2D<TYPE>::get_PointSet() const {
  return point_storage_;
}

}  // namespace planning
}  // namespace Themis