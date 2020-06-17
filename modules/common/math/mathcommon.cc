#include "mathcommon.h"
#include <algorithm>
#include <cmath>
#include <memory>
#include <stdexcept>

namespace atd {
namespace utility {

static const double PI = 3.1415926535898;

double MathCommon::arctan(const double& delta_y, const double& delta_x) {
  double theta_ori = atan2(delta_y, delta_x);
  if (theta_ori < 0) {
    theta_ori += PI * 2;
  }
  return theta_ori;
}

std::vector<double> MathCommon::linspace(const double& start, const double& end,
                                         const unsigned int& num) {
  std::vector<double> sqnce = {start};
  if (num > 1) {
    double gap = (end - start) / (num);
    for (unsigned i = 1; i < num - 1; ++i) {
      sqnce.push_back(start + i * gap);
    }
  }  //  if (num > 2)
  sqnce.push_back(end);
  return sqnce;
}

std::vector<double> MathCommon::diff(const std::vector<double>& target) {
  unsigned int target_size = target.size();
  if (target_size < 2) {
    throw std::logic_error("can not execute diff operation");
  }
  std::vector<double> target_diff;
  for (unsigned int i = 0; i < (target_size - 1); i++) {
    target_diff.push_back(target.at(i + 1) - target.at(i));
  }
  return target_diff;
}

std::vector<double> MathCommon::middle(const std::vector<double>& target) {
  unsigned int target_size = target.size();
  if (target_size < 2) {
    throw std::logic_error("can not execute diff operation");
  }
  std::vector<double> target_diff;
  for (unsigned int i = 0; i < (target_size - 1); i++) {
    target_diff.push_back((target.at(i + 1) + target.at(i)) /
                          static_cast<double>(2));
  }
  return target_diff;
}

std::vector<double> MathCommon::merge_vector_from_end(
    const std::vector<double>& first_item,
    const std::vector<double>& second_item) {
  size_t first_size = first_item.size();
  size_t second_size = second_item.size();
  std::vector<double> merge_vector;
  if (first_size > second_size) {
    merge_vector.assign(first_item.begin(), first_item.end());
    for (int index4loop = 0; index4loop < second_size; index4loop++) {
      merge_vector[first_size - 1 - index4loop] +=
          second_item.at(second_size - 1 - index4loop);
    }
  } else {
    merge_vector.assign(second_item.begin(), second_item.end());
    for (int index4loop = 0; index4loop < first_size; index4loop++) {
      merge_vector[second_size - 1 - index4loop] +=
          first_item.at(first_size - 1 - index4loop);
    }
  }
  return merge_vector;
}

}  // namespace planning
}  // namespace atd
