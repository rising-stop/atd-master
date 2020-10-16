#include "calibrator.h"

void Calibrator::render() {
  cal_var_.Clear();
  active_MenuItem();

  if (ImGui::Button("Calibration_Switch")) ImGui::OpenPopup("Calibration_List");
  if (ImGui::BeginPopup("Calibration_List")) {
    for (auto& menu_item : menu_status_)
      ImGui::MenuItem(menu_item.first.c_str(), "", &(menu_item.second));
    ImGui::EndPopup();
  }

  ImGui::SameLine();
  ImGui::Checkbox("Enable", &enable_);

  render_CalibConsole();
  ImGui::Spacing();
  ImGui::Separator();

  if (menu_status_.empty()) {
    ImGui::Text("No Calibration Variable Detected");
  } else {
  }

  send_CalibrationAlternation();
}

void Calibrator::udpate_Database() {
  static atd::protocol::DISPLAY_CALIBRATION calib_frame;
  atd::utility::Singleton::instance<RealTimeDataDispatcher>()->get_LatestDisplayCalib(
      calib_frame);
  for (const auto& float_var : calib_frame.calib_float()) {
    auto reg_res =
        calib_repository_
            .try_RegisterVar<atd::utility::CalibrationVariable<float>>(
                float_var.name(), float_var.data(),
                float_var.data_upper_bound(), float_var.data_lower_bound(),
                float_var.data_init());
    if (!reg_res.second) {
      reg_res.first->set_Var(float_var.data());
    }
  }
  for (const auto& int_var : calib_frame.calib_int()) {
    auto reg_res =
        calib_repository_
            .try_RegisterVar<atd::utility::CalibrationVariable<int>>(
                int_var.name(), int_var.data(), int_var.data_upper_bound(),
                int_var.data_lower_bound(), int_var.data_init());
    if (!reg_res.second) {
      reg_res.first->set_Var(int_var.data());
    }
  }
  for (const auto& uint_var : calib_frame.calib_uint()) {
    auto reg_res =
        calib_repository_
            .try_RegisterVar<atd::utility::CalibrationVariable<uint32_t>>(
                uint_var.name(), uint_var.data(), uint_var.data_upper_bound(),
                uint_var.data_lower_bound(), uint_var.data_init());
    if (!reg_res.second) {
      reg_res.first->set_Var(uint_var.data());
    }
  }
}

void Calibrator::render_CalibConsole() {
  for (auto calib_name : menu_status_) {
    if (!calib_name.second) {
      continue;
    }
    auto float_item =
        calib_repository_.get_RegisteredVar<CalibrationVariable<float>>(
            calib_name.first);
    if (float_item.second) {
      render_FloatConsole(calib_name.first, float_item.first.get());
      continue;
    }

    auto int_item =
        calib_repository_.get_RegisteredVar<CalibrationVariable<int>>(
            calib_name.first);
    if (int_item.second) {
      render_IntConsole(calib_name.first, int_item.first.get());
      continue;
    }

    auto uint_item =
        calib_repository_.get_RegisteredVar<CalibrationVariable<uint32_t>>(
            calib_name.first);
    if (uint_item.second) {
      render_UIntConsole(calib_name.first, uint_item.first.get());
      continue;
    }
  }
}

void Calibrator::render_FloatConsole(const std::string& name,
                                     const CalibrationVariable<float>* var) {
  std::string button_name = "Input ";
  button_name.append(name);
  button_name.append(" by keyboard");
  bool focus = ImGui::Button(button_name.c_str());
  ImGui::SameLine();

  std::string reset_botton_name = "Reset ";
  reset_botton_name.append(name);
  bool is_reset = ImGui::Button(reset_botton_name.c_str());

  std::string slider_name;
  slider_name.append(
      atd::utility::CString::cstring_cat("%.6f", var->get_Var()));
  if (focus) ImGui::SetKeyboardFocusHere();
  float current_val = var->get_Var();
  ImGui::SliderFloat(name.c_str(), &current_val, var->get_Min(),
                     var->get_Max());
  if (enable_) {
    if (is_reset) {
      fill_CalibrationSender(name, var->get_Init());
    } else {
      if (std::fabs(current_val - var->get_Var()) > 1e-5) {
        fill_CalibrationSender(name, current_val);
      }
    }
  }
  ImGui::Separator();
}

void Calibrator::render_IntConsole(const std::string& name,
                                   const CalibrationVariable<int>* var) {
  std::string button_name = "Input ";
  button_name.append(name);
  button_name.append(" by keyboard");
  bool focus = ImGui::Button(button_name.c_str());
  ImGui::SameLine();

  std::string reset_botton_name = "Reset ";
  reset_botton_name.append(name);
  bool is_reset = ImGui::Button(reset_botton_name.c_str());

  std::string slider_name;
  slider_name.append(atd::utility::CString::cstring_cat("%d", var->get_Var()));
  if (focus) ImGui::SetKeyboardFocusHere();
  int current_val = var->get_Var();
  ImGui::SliderInt(name.c_str(), &current_val, var->get_Min(), var->get_Max());
  if (enable_) {
    if (is_reset) {
      fill_CalibrationSender(name, var->get_Init());
    } else {
      if (current_val != var->get_Var()) {
        fill_CalibrationSender(name, current_val);
      }
    }
  }
  ImGui::Separator();
}

void Calibrator::render_UIntConsole(const std::string& name,
                                    const CalibrationVariable<uint32_t>* var) {
  std::string button_name = "Input ";
  button_name.append(name);
  button_name.append(" by keyboard");
  bool focus = ImGui::Button(button_name.c_str());
  ImGui::SameLine();

  std::string reset_botton_name = "Reset ";
  reset_botton_name.append(name);
  bool is_reset = ImGui::Button(reset_botton_name.c_str());

  std::string slider_name;
  slider_name.append(atd::utility::CString::cstring_cat("%d", var->get_Var()));
  if (focus) ImGui::SetKeyboardFocusHere();
  int current_val = (var->get_Var() > INT32_MAX) ? INT32_MAX : var->get_Var();
  uint32_t upper_bound =
      (var->get_Max() > INT32_MAX) ? INT32_MAX : var->get_Max();
  uint32_t lower_bound =
      (var->get_Min() > INT32_MAX) ? INT32_MAX : var->get_Min();

  ImGui::SliderInt(name.c_str(), &current_val, lower_bound, upper_bound);
  if (enable_) {
    if (is_reset) {
      fill_CalibrationSender(name, var->get_Init());
    } else {
      if (current_val != var->get_Var()) {
        fill_CalibrationSender(name, static_cast<uint32_t>(current_val));
      }
    }
  }
  ImGui::Separator();
}

void Calibrator::active_MenuItem() {
  for (const auto& item : calib_repository_) {
    menu_status_.insert({item.first, false});
  }
}

void Calibrator::remove_ZombieMenuItem() {
  std::vector<std::string> active_menu_item;
  for (const auto& item : menu_status_) {
    if (item.second) {
      active_menu_item.push_back(item.first);
    }
  }
  menu_status_.clear();
  active_MenuItem();
  for (const auto& act_item : active_menu_item) {
    auto find_res = menu_status_.find(act_item);
    if (find_res == menu_status_.end()) {
      continue;
    }
    find_res->second = true;
  }
}

void Calibrator::send_CalibrationAlternation() {
  for (int i = 0; i < Calibrator_AlterCommand_SendTime; i++) {
    calib_publisher_.publish(cal_var_);
  }
}
