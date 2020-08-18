#ifndef PLANNING_POLYNOMIAL2D_H_
#define PLANNING_POLYNOMIAL2D_H_

#include <iostream>
#include <stdexcept>
#include <unordered_map>
#include <vector>

namespace atd {
namespace utility {

class Polynomial {
  /* basic operation for polynomial */
 public:
  void set_PolynomialParameters(const std::initializer_list<double>&);
  void set_PolynomialParameters(const std::vector<double>&);
  void set_PolynomialParameters(const int&);
  void reset();

  double get_PolynomialValue(const double&) const;
  Polynomial get_Derivative() const;
  Polynomial get_Integration(const double&) const;

  double get_Coefficient(const int&) const;
  std::vector<double> get_Coefficient() const;
  std::vector<double>* get_Mutable_Parameters();
  double get_Length(const double&, const double&) const;
  int get_HighestDegree() const;

  Polynomial& operator=(const Polynomial&);
  Polynomial operator+(const Polynomial&) const;
  Polynomial& operator+=(const Polynomial&);
  Polynomial operator-(const Polynomial&) const;
  Polynomial& operator-=(const Polynomial&);
  Polynomial operator*(const Polynomial&) const;
  Polynomial& operator*=(const Polynomial&);
  Polynomial operator*(const double&) const;
  Polynomial& operator*=(const double&);

  inline int get_ItemOrder(int) const;
  inline int get_ItemIndex(int) const;
  /* base data */
 private:
  int ORDER_;
  int PARAM_SIZE_;
  std::vector<double> source_;

  /* source manager */
 public:
  bool has_param() const;

 protected:
  void set_has_param();

 private:
  bool flag_has_param_ = false;

  /* base class constractor */
 public:
  Polynomial() = default;
  ~Polynomial() = default;
  Polynomial(const Polynomial&) = default;
  Polynomial(Polynomial&&) = default;
  Polynomial(const std::initializer_list<double>&);
  Polynomial(const std::vector<double>&);
  explicit Polynomial(const int& order);

 public:
  friend std::ostream& operator<<(std::ostream&, const Polynomial&);
};

}  // namespace utility
}  // namespace atd

#endif  // PLANNING_POLYNOMIAL_H
