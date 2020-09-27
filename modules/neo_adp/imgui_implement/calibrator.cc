#include "calibrator.h"

void Calibrator::render() {
  select_ActivatedMenu();

  if (ImGui::Button("Calibration_Switch")) ImGui::OpenPopup("Calibration_List");
  if (ImGui::BeginPopup("Calibration_List")) {
    for (auto& menu_item : float_menu_status_)
      ImGui::MenuItem(menu_item.first.c_str(), "", &(menu_item.second));
    for (auto& menu_item : int_menu_status_)
      ImGui::MenuItem(menu_item.first.c_str(), "", &(menu_item.second));
    for (auto& menu_item : uint_menu_status_)
      ImGui::MenuItem(menu_item.first.c_str(), "", &(menu_item.second));
    ImGui::EndPopup();
  }

  ImGui::SameLine();
  ImGui::Checkbox("Enable", &enable_);
  ImGui::Separator();

  if (calib_info.empty()) {
    ImGui::Text("No Calibration Variable Detected");
  } else {
    for (auto calib_name : dynamic_calib_list) {
      if (!calib_name.second) {
        continue;
      }
      auto calib_var = calib_info.find(calib_name.first);
      std::string button_name = "Input ";
      button_name.append(calib_var->second.name_);
      button_name.append(" by keyboard");
      bool focus = ImGui::Button(button_name.c_str());
      ImGui::SameLine();

      std::string reset_botton_name = "Reset ";
      reset_botton_name.append(calib_var->second.name_);
      if (ImGui::Button(reset_botton_name.c_str())) {
        calib_var->second.var_ = calib_var->second.init_;
      }

      std::string slider_name;
      slider_name.append(std::to_string(calib_var->second.var_));
      if (focus) ImGui::SetKeyboardFocusHere();
      ImGui::SliderFloat(calib_var->second.name_.c_str(),
                         &calib_var->second.var_, calib_var->second.min_,
                         calib_var->second.max_);
      if (calib_enable) {
        atd::utility::Singleton::instance<DataDispatcher>()
            ->set_CalibrationInfo(calib_var->second.name_,
                                  calib_var->second.var_);
      }

      ImGui::Separator();
    }
  }
}

void Calibrator::select_ActivatedMenu() {
  atd::utility::Singleton::instance<DataDispatcher>()->get_CalibInfo_As_Float(
      float_calib_var_);
  atd::utility::Singleton::instance<DataDispatcher>()->get_CalibInfo_As_Int(
      int_calib_var_);
  atd::utility::Singleton::instance<DataDispatcher>()->get_CalibInfo_As_UInt(
      uint_calib_var_);

  for (auto single_calib_info : float_calib_var_) {
    float_menu_status_.insert({single_calib_info.first, false});
  }
  auto itr_dyn_cal_list = float_menu_status_.begin();
  while (itr_dyn_cal_list != float_menu_status_.end()) {
    if (float_calib_var_.find(itr_dyn_cal_list->first) ==
        float_calib_var_.end()) {
      itr_dyn_cal_list = float_menu_status_.erase(itr_dyn_cal_list);
    } else {
      itr_dyn_cal_list++;
    }
  }
  auto itr_dyn_cal_list = int_menu_status_.begin();
  for (auto single_calib_info : int_calib_var_) {
    int_menu_status_.insert({single_calib_info.first, false});
  }
  while (itr_dyn_cal_list != int_menu_status_.end()) {
    if (int_calib_var_.find(itr_dyn_cal_list->first) == int_calib_var_.end()) {
      itr_dyn_cal_list = int_menu_status_.erase(itr_dyn_cal_list);
    } else {
      itr_dyn_cal_list++;
    }
  }
  auto itr_dyn_cal_list = uint_menu_status_.begin();
  for (auto single_calib_info : uint_calib_var_) {
    uint_menu_status_.insert({single_calib_info.first, false});
  }
  while (itr_dyn_cal_list != uint_menu_status_.end()) {
    if (uint_calib_var_.find(itr_dyn_cal_list->first) ==
        uint_calib_var_.end()) {
      itr_dyn_cal_list = uint_menu_status_.erase(itr_dyn_cal_list);
    } else {
      itr_dyn_cal_list++;
    }
  }
}
