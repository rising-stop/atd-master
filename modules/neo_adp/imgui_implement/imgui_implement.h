#pragma once

#include <vector>

#include "modules/common/tools/logging/debug_logging.h"

struct Switches4SubWindows {
  bool show_demo_window = false;
  bool show_log_window = false;
  bool show_data_monitor = false;
  bool show_calibration_console = false;
  bool flag_real_time_mode = true;
  bool flag_log_mode = false;
};

class ImGui_ComponentManager {
 public:
  static void Imgui_Drawing();

 private:
  static void Show_Custom_Window();
  static void Show_Log_Window(bool* swth);
  static void Show_DataMonitor(bool* swth);
  static void Show_Calibration_Console(bool* swth);

  static Switches4SubWindows switches;

 public:
  ImGui_ComponentManager() = default;
};