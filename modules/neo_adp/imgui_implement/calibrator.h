#include "imgui_basic_component.h"
#include "modules/neo_adp/data_service/data_dispatcher.h"

class Calibrator : public ImGui_Components {
 public:
  virtual void render() override;

 private:
  void udpate_Database();
  void select_ActivatedMenu();
  void render_CalibConsole();

 private:
  /**
   * @brief data base for calibrator
   */
  Any_CalibrationRepository calib_repository_;

  std::map<std::string, bool> menu_status_;

  bool enable_ = false;

 public:
  Calibrator() = default;
  ~Calibrator() = default;
};