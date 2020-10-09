#include "calibrator.h"

template <typename T>
void Calibrator::fill_CalibrationSender(const std::string& name, const T& val) {
  std::string type_name = typeid(T).name();
  if (typeid(float).name() == type_name) {
    auto ptr_float = cal_var_.add_calib_float();
    ptr_float->set_name(name);
    ptr_float->set_data(val);
  } else if (typeid(int).name() == type_name) {
    auto ptr_int = cal_var_.add_calib_int();
    ptr_int->set_name(name);
    ptr_int->set_data(val);
  } else if (typeid(uint32_t).name() == type_name) {
    auto ptr_uint = cal_var_.add_calib_uint();
    ptr_uint->set_name(name);
    ptr_uint->set_data(val);
  } else {
  }
}