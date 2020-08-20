#ifndef MATHCOMMON_H
#define MATHCOMMON_H

#include <vector>

#define SYSTEM_PRECISION 1e-6

namespace atd {
namespace utility {

class MathCommon {
 public:
  MathCommon() = delete;

  static double arctan(const double&, const double&);

  static std::vector<double> linspace(const double&, const double&,
                                      const unsigned int&);

  static std::vector<double> diff(const std::vector<double>&);

  static std::vector<double> middle(const std::vector<double>&);

  static std::vector<double> merge_vector_from_end(const std::vector<double>&,
                                                   const std::vector<double>&);
};

}  // namespace planning
}  // namespace atd

#endif  // MATHCOMMON_H
