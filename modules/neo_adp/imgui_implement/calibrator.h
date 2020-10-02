#include "imgui_basic_component.h"
#include "modules/neo_adp/data_service/data_dispatcher.h"

class Calibrator : public ImGui_Components {
 public:
  virtual void render() override;

 private:
  void udpate_Database();
  void active_MenuItem();
  void remove_ZombieMenuItem();

  void render_CalibConsole() const;
  void render_FloatConsole(const std::string&,
                           const CalibrationVariable<float>*) const;
  void render_IntConsole(const std::string&,
                         const CalibrationVariable<int>*) const;
  void render_IntConsole(const std::string&,
                         const CalibrationVariable<uint32_t>*) const;

 private:
  /**
   * @brief data base for calibrator
   */
  Any_CalibrationRepository calib_repository_;

  std::map<std::string, bool> menu_status_;

  bool enable_ = false;

 private:
  void command_CalibOnline();
  void command_CalibFloatOnline();
  void command_CalibIntOnline();
  void command_CalibUIntOnline();

 public:
  Calibrator() = default;
  ~Calibrator() = default;
};