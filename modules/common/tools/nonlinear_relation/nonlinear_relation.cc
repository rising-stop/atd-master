#include "nonlinear_relation.h"

#include <stdexcept>

#include "modules/common/math/NumericalComputing/interpolation.h"
#include "modules/common/utility/file_parser/xml_parser.h"

namespace Themis {
namespace utility {

std::mutex NonLinearRelation::mutex_instance_;
NonLinearRelation* NonLinearRelation::instance_ = nullptr;

bool NonLinearRelation::RelationRegistration(const char* xml_path,
                                             const char* table_ID) {
  data_base_.insert(std::make_pair(table_ID, MonotonicDiscreteLine2D()));
  int register_res =
      XMLParser::Import2DTable(xml_path, table_ID, data_base_[table_ID]);
  if (register_res) {
    return false;
  }
  data_base_[table_ID].PointRearrangement();
  return true;
}

double NonLinearRelation::LinearMatching(const char* table_ID,
                                         const double& num) const {
  if (data_base_.find(table_ID) == data_base_.cend()) {
    throw std::runtime_error("LinearMatching: id is not regitered yet");
  }
  auto point_check = data_base_.at(table_ID).get_AdjacentPoint(num);
  if (point_check.first) {
    std::vector<Point2D> source;
    source.push_back(*point_check.second.first);
    source.push_back(*point_check.second.second);
    return Interpolation::LinearInterp(source, num);
  }
  if (point_check.second.first->get_x_() <
      data_base_.at(table_ID).get_PointSet().cbegin()->get_x_()) {
    return data_base_.at(table_ID).get_PointSet().cbegin()->get_x_();
  } else {
    return (data_base_.at(table_ID).get_PointSet().cend() - 1)->get_x_();
  }
}

double NonLinearRelation::InterpMatching(const char*, const double&) const {}

NonLinearRelation* NonLinearRelation::get_Instance() {
  if (instance_) {
    return instance_;
  }
  std::lock_guard<std::mutex> static_mutex_guard(mutex_instance_);
  if (!instance_) {
    instance_ = new NonLinearRelation();
  }
  return instance_;
}

}  // namespace utility
}  // namespace Themis