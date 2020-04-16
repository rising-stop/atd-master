#include "arrow2D.h"
#include <stdexcept>
#include "modules/common/math/mathcommon.h"

namespace atd {
namespace planning {
namespace common {

void Arrow2D::set_ThetaFromPoint(const Point2D& target) {
  if (!has_x_() || !has_y_()) {
    throw std::runtime_error(
        "Arrow2D: empty arrow can not set theta from other points");
  }
  theta_ = MathCommon::arctan(target.get_y_() - y_, target.get_x_() - x_);
}

}  // namespace common
}  // namespace planning
}  // namespace atd