#include "imgui_basic_component.h"
#include "modules/neo_adp/data_service/data_dispatcher.h"

class Calibrator : public ImGui_Components {
 public:
  virtual void render() override;

 private:
  void select_ActivatedMenu();

  std::map<std::string, CalibrationVariable<float>> float_calib_var_;
  std::map<std::string, CalibrationVariable<int32_t>> int_calib_var_;
  std::map<std::string, CalibrationVariable<uint32_t>> uint_calib_var_;

  std::map<std::string, bool> float_menu_status_;
  std::map<std::string, bool> int_menu_status_;
  std::map<std::string, bool> uint_menu_status_;

  bool enable_ = false;

 public:
  Calibrator() = default;
  ~Calibrator() = default;
};