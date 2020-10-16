#pragma once

#include "imgui_basic_component.h"
#include "modules/common/common_header.h"
#include "modules/neo_adp/data_service/data_repository.h"
#include "protobuf_msg/planning_log.pb.h"

class Calibrator : public ImGui_Components {
 public:
  virtual void render() override;

 private:
  void udpate_Database();
  void active_MenuItem();
  void remove_ZombieMenuItem();

  void render_CalibConsole();
  void render_FloatConsole(const std::string&,
                           const atd::utility::CalibrationVariable<float>*);
  void render_IntConsole(const std::string&,
                         const atd::utility::CalibrationVariable<int>*);
  void render_UIntConsole(const std::string&,
                          const atd::utility::CalibrationVariable<uint32_t>*);

  template <typename T>
  void fill_CalibrationSender(const std::string& name, const T& val);

  void send_CalibrationAlternation();

 private:
  /**
   * @brief data base for calibrator
   */
  atd::utility::Any_Repository calib_repository_;

  std::map<std::string, bool> menu_status_;

  bool enable_ = false;

  atd::utility::Proto_Messages<atd::protocol::DISPLAY_CALIBRATION> cal_var_;
  atd::utility::LCM_Proxy<
      atd::utility::Proto_Messages<atd::protocol::DISPLAY_CALIBRATION>>
      calib_publisher_{atd::utility::LCM_MODE::SENDER, "PlanningCalib"};

 public:
  Calibrator() = default;
  ~Calibrator() = default;
};

#include "calibrator.tcc"
