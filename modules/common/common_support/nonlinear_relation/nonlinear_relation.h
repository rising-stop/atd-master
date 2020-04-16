#ifndef PLANNING_NONLINEAR_RELATION_H_
#define PLANNING_NONLINEAR_RELATION_H_

#include <mutex>
#include <string>
#include <unordered_map>

#include "modules/common/math/EuclideanGeometry/line/monotonic_discrete_line2D.h"

#ifndef NONLINEAR_RELATION
#define NONLINEAR_RELATION \
  atd::planning::common::NonLinearRelation::get_Instance()
#endif

namespace atd {
namespace planning {
namespace common {

class NonLinearRelation {
 public:
  bool RelationRegistration(const char*, const char*);

  double LinearMatching(const char*, const double&) const;
  double InterpMatching(const char*, const double&) const;

 private:
  std::unordered_map<const char*, MonotonicDiscreteLine2D> data_base_;

 public:
  static NonLinearRelation* get_Instance();

 private:
  static std::mutex mutex_instance_;
  static NonLinearRelation* instance_;

 private:
  NonLinearRelation() = default;
  ~NonLinearRelation() = default;
};

}  // namespace common
}  // namespace planning
}  // namespace atd

#endif