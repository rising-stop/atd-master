#include "modules/planning_common/planning_header.h"

using atd::planning::common::BasicGeometry;
using atd::planning::common::Point2D;

int main(int argc, char **argv) {

  static bool flag_once = true;
  if (flag_once) {
    auto lseg = std::make_pair(Point2D(0, 10), Point2D(10, -10));
    auto rseg = std::make_pair(Point2D(0, -5), Point2D(8, 10));
    std::cout << NUMINFO(lseg) << NUMINFO(rseg) << std::endl;

    auto res = BasicGeometry::LineSegmentIntersection(lseg, rseg);

    if (res.first) {
      std::cout << "intersect point: " << res.second << std::endl;
    }

    flag_once = false;
  }
  
  return 0;
}