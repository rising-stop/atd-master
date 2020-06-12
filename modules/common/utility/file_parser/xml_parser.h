#ifndef PLANNING_TABLE_PARSER_H_
#define PLANNING_TABLE_PARSER_H_

#include <string>

#include "modules/common/3rd_party/tinyxml2/tinyxml2.h"
#include "modules/common/math/EuclideanGeometry/line/monotonic_discrete_line2D.h"

namespace atd {
namespace common {
namespace utility {

using namespace tinyxml2;

class XMLParser {
 public:
  static int Insert2DTable(
      const char*, const char*,
      const atd::planning::common::MonotonicDiscreteLine2D&);
  static int Import2DTable(const char*, const char*,
                           atd::planning::common::MonotonicDiscreteLine2D&);

 public:
  XMLParser() = delete;
};

}  // namespace utility
}  // namespace common
}  // namespace atd

#endif