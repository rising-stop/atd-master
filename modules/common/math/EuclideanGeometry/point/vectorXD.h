#ifndef PLANNING_VECTOR_XD_H_
#define PLANNING_VECTOR_XD_H_

#include <vector>

namespace Themis {
namespace utility {

template <int ORDER>
class VectorXD {
 public:
  bool empty() const;
  int get_Order() const;
  void set_Value(const std::initializer_list<double>&);
  void set_Value(const std::vector<double>&);
  double at(const int&) const;
  double& operator[](const int&);

  VectorXD<ORDER>& operator=(const VectorXD<ORDER>&);
  VectorXD<ORDER> operator+(const VectorXD<ORDER>&) const;
  VectorXD<ORDER>& operator+=(const VectorXD&);
  VectorXD<ORDER> operator-(const VectorXD<ORDER>&) const;
  VectorXD<ORDER>& operator-=(const VectorXD<ORDER>&);
  VectorXD<ORDER> operator*(const double&)const;
  VectorXD<ORDER>& operator*=(const double&);

  double norm() const;
  double inner_product(const VectorXD<ORDER>&) const;

  double cross_product(const VectorXD<2>&) const;
  VectorXD<3> cross_product(const VectorXD<3>&) const;

  bool is_Collinear(const VectorXD<ORDER>&) const;

 private:
  std::vector<double> elements_ = std::vector<double>(ORDER, 0.0);

 public:
  VectorXD() = default;
  explicit VectorXD(const std::initializer_list<double>&);
  explicit VectorXD(const std::vector<double>&);
  VectorXD(const VectorXD<ORDER>&);
  VectorXD(VectorXD<ORDER>&&) = default;
  ~VectorXD() = default;
};

typedef VectorXD<2> Vector2D;
typedef VectorXD<3> Vector3D;

}  // namespace planning
}  // namespace Themis

#include "vectorXD.tcc"

#endif