#pragma once

#include <vector>

#include "data_monitor.h"
#include "modules/common/tools/logging/debug_logging.h"

struct Switches4SubWindows {
  bool show_demo_window = false;
  bool show_log_window = false;
  bool show_data_monitor = false;
  bool show_calibration_console = false;
};

class ImGui_Components {
 public:
  static void Imgui_Drawing();

 private:
  static void Show_Custom_Window();
  static void HelpMarker(const char* desc);
  static void Show_Log_Window(bool* swth);
  static void Show_DataMonitor(bool* swth);
  static void Show_Calibration_Console(bool* swth);

  static Switches4SubWindows switches;
};