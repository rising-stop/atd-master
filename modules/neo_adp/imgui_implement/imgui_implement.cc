#include "imgui_implement.h"

Switches4SubWindows ImGui_Components::switches;

void ImGui_Components::Imgui_Drawing() {
  // Start the Dear ImGui frame
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  Show_Custom_Window();
  if (switches.show_demo_window)
    ImGui::ShowDemoWindow(&switches.show_demo_window);
  Show_Log_Window(&switches.show_log_window);
  Show_DataMonitor(&switches.show_data_monitor);
  Show_Calibration_Console(&switches.show_calibration_console);

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGui_Components::HelpMarker(const char* desc) {
  ImGui::TextDisabled("(?)");
  if (ImGui::IsItemHovered()) {
    ImGui::BeginTooltip();
    ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
    ImGui::TextUnformatted(desc);
    ImGui::PopTextWrapPos();
    ImGui::EndTooltip();
  }
}

void ImGui_Components::Show_Custom_Window() {
  static float f = 0.0f;
  static int counter = 0;

  ImGui::Begin("Main Console");

  // ImGui::Text("This is some useful text.");
  // format strings too)
  ImGui::Checkbox("Demo Window",
                  &switches.show_demo_window);  // Edit bools storing our window
                                                // open/close state
  ImGui::Checkbox("Log Info", &switches.show_log_window);
  ImGui::Checkbox("Data Monitor", &switches.show_data_monitor);
  ImGui::Checkbox("Calibration Console", &switches.show_calibration_console);

  ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
              1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
  ImGui::End();
}

void ImGui_Components::Show_Log_Window(bool* swth) {
  if (!(*swth)) return;
  static std::string log_content;
  static ImVec4 text_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
  static bool enable_output = true;
  if (enable_output) {
    static bool flag_offline = false;
    if (!atd::utility::Singleton::instance<DataDispatcher>()->get_TotalLogFrame(
            log_content)) {
      if (!flag_offline) {
        log_content.append("Ops! LCM messages are not received.");
        flag_offline = true;
      }
    } else {
      flag_offline = false;
    }
  }

  ImGui::Begin("Log Info",
               swth);  // Pass a pointer to our bool variable
  // (the window will have a closing button
  // that will clear the bool when clicked)
  HelpMarker("Log Info from Planning Module");

  ImGui::Checkbox("Enable Output Text", &enable_output);
  ImGui::ColorEdit3("clear color",
                    (float*)&text_color);  // Edit 3 floats representing a color

  {
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar;
    ImGui::BeginChild("ChildL",
                      ImVec2(ImGui::GetWindowContentRegionWidth() * 0.9f,
                             ImGui::GetWindowContentRegionHeight() * 0.9f),
                      false, window_flags);
    // ImGui::Text(log_content.c_str());
    ImGui::TextColored(text_color, log_content.c_str());

    ImGui::EndChild();
  }

  if (ImGui::Button("Close")) switches.show_log_window = false;
  ImGui::End();
}

void ImGui_Components::Show_DataMonitor(bool* swth) {
  if (!(*swth)) return;

  static std::vector<DataMonitor*> monitor_set;

  ImGui::Begin("Data Monitor", swth);
  if (ImGui::Button("Monitor - Add on")) {
    static int monitor_counter = 0;
    monitor_set.push_back(new DataMonitor(std::to_string(++monitor_counter)));
  }
  ImGui::SameLine();
  static bool enable_all = true;
  ImGui::Checkbox("Enable All", &enable_all);

  ImGui::Separator();
  static bool enable_once_flag = false;
  if (!enable_all) {
    for (auto single_monitor : monitor_set) {
      single_monitor->Disable();
    }
    enable_once_flag = true;
  } else {
    if (enable_once_flag) {
      for (auto single_monitor : monitor_set) {
        single_monitor->Enable();
      }
      enable_once_flag = false;
    }
  }
  for (auto single_monitor : monitor_set) {
    single_monitor->Render();
  }

  if (ImGui::Button("Monitor - Remove")) {
    if (monitor_set.size()) {
      delete monitor_set.back();
      monitor_set.pop_back();
    }
  }

  if (ImGui::Button("Close")) switches.show_data_monitor = false;
  ImGui::End();
}

void ImGui_Components::Show_Calibration_Console(bool* swth) {
  if (!(*swth)) return;

  static std::map<std::string, DataDispatcher::CalibrationVariable> calib_info;
  atd::utility::Singleton::instance<DataDispatcher>()->get_CalibInfo(
      calib_info);

  ImGui::Begin("Calibration Console", swth);

  static std::map<std::string, bool> dynamic_calib_list;
  for (auto single_calib_info : calib_info) {
    dynamic_calib_list.insert({single_calib_info.first, false});
  }
  auto itr_dyn_cal_list = dynamic_calib_list.begin();
  while (itr_dyn_cal_list != dynamic_calib_list.end()) {
    if (calib_info.find(itr_dyn_cal_list->first) == calib_info.end()) {
      itr_dyn_cal_list = dynamic_calib_list.erase(itr_dyn_cal_list);
    } else {
      itr_dyn_cal_list++;
    }
  }

  if (ImGui::Button("Calibration_Switch")) ImGui::OpenPopup("Calibration_List");
  if (ImGui::BeginPopup("Calibration_List")) {
    for (auto& menu_item : dynamic_calib_list)
      ImGui::MenuItem(menu_item.first.c_str(), "", &(menu_item.second));
    ImGui::EndPopup();
  }

  ImGui::SameLine();
  static bool calib_enable = false;
  ImGui::Checkbox("Enable", &calib_enable);
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

  if (ImGui::Button("Close")) switches.show_calibration_console = false;
  ImGui::End();
}