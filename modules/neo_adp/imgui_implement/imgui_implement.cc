#include "imgui_implement.h"

#include "calibrator.h"
#include "data_analyzer.h"
#include "log_monitor.h"
#include "modules/neo_adp/data_service/data_seg4data_monitor.h"
#include "modules/neo_adp/imgui_implement/data_analyzer.h"
#include "modules/neo_adp/imgui_implement/file_interface.h"
Switches4SubWindows ImGui_ComponentManager::switches;

void ImGui_ComponentManager::init() {
  atd::utility::Singleton::try_register<ResourceInterface_Manager>();

  atd::utility::Singleton::instance<DataRepository>()
      ->try_register<DataSeg4DataMonitor>(
          Data_Seg_Name_DataMonitor, DataMonitor_Max_BufferSize,
          []() { return PROTOCOL_POINTER->get_LatestFrame(); });
  atd::utility::Singleton::instance<DataRepository>()
      ->try_register<PlanningLog_Reader>(Data_Seg_Name_LogMonitor);

  atd::utility::Singleton::try_register<DataAnalyzer>();
  atd::utility::Singleton::instance<DataAnalyzer>()
      ->add_MonitorTab<LogDataObserver>(
          "Log Mode", DataMonitor_Max_BufferSize,
          []() { return LOG_READER_POINTER->get_ConstDataRef4Observer(); });
  atd::utility::Singleton::instance<DataAnalyzer>()
      ->add_MonitorTab<DataObserver_Manager>(
          "Real Mode", DataMonitor_Max_BufferSize,
          []() { return MONITOR_DATA_POINTER->get_ConstDataRef4Observer(); });
}

void ImGui_ComponentManager::Imgui_Drawing() {
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

void ImGui_ComponentManager::Show_Custom_Window() {
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

void ImGui_ComponentManager::Show_Log_Window(bool* swth) {
  if (!(*swth)) return;
  static LogMonitor monitor;

  ImGui::Begin("Log Info", swth);

  ImGui::Separator();

  monitor.render();

  if (ImGui::Button("Close")) switches.show_log_window = false;

  ImGui::End();
}

void ImGui_ComponentManager::Show_DataMonitor(bool* swth) {
  if (!(*swth)) return;
  ImGui::Begin("Data Monitor", swth);
  ImGui::Separator();
  atd::utility::Singleton::instance<DataAnalyzer>()->render();
  if (ImGui::Button("Close")) switches.show_data_monitor = false;
  ImGui::End();
}

void ImGui_ComponentManager::Show_Calibration_Console(bool* swth) {
  if (!(*swth)) return;
  static Calibrator calibrator;
  ImGui::Begin("Calibration Console", swth);
  calibrator.render();
  if (ImGui::Button("Close")) switches.show_calibration_console = false;
  ImGui::End();
}