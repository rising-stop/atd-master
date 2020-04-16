#ifndef PLANNING_TABLE_PARSER_H_
#define PLANNING_TABLE_PARSER_H_

#include <string>

#include "modules/common/3rd_party/tinyxml2/tinyxml2.h"
#include "modules/common/math/EuclideanGeometry/line/monotonic_discrete_line2D.h"

namespace atd {
namespace planning {
namespace common {

using namespace tinyxml2;

class XMLParser {
 public:
  static int Insert2DTable(const char*, const char*,
                           const MonotonicDiscreteLine2D&);
  static int Import2DTable(const char*, const char*, MonotonicDiscreteLine2D&);

 public:
  XMLParser() = delete;
};

}  // namespace common
}  // namespace planning
}  // namespace atd

#endif