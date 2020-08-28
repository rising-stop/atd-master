#pragma once

#include <vector>

#include "data_monitor.hpp"
#include "modules/neo_adp/imgui-opengl3/imgui_impl_glfw.h"
#include "modules/neo_adp/imgui-opengl3/imgui_impl_opengl3.h"
#include "modules/neo_adp/imgui_module/imgui.h"

struct Switches4SubWindows {
  bool show_demo_window = false;
  bool show_log_window = false;
  bool show_data_monitor = false;
} switches;

static void HelpMarker(const char* desc) {
  ImGui::TextDisabled("(?)");
  if (ImGui::IsItemHovered()) {
    ImGui::BeginTooltip();
    ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
    ImGui::TextUnformatted(desc);
    ImGui::PopTextWrapPos();
    ImGui::EndTooltip();
  }
}

static void Show_Custom_Window() {
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

  ImGui::SliderFloat("float", &f, 0.0f,
                     1.0f);  // Edit 1 float using a slider from 0.0f to 1.0f

  if (ImGui::Button("Button"))  // Buttons return true when clicked (most
                                // widgets return true when edited/activated)
    counter++;
  ImGui::SameLine();
  ImGui::Text("counter = %d", counter);

  ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
              1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
  ImGui::End();
}

static void Show_Log_Window(bool* swth) {
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

static void Show_DataMonitor(bool* swth) {
  if (!(*swth)) return;

  static std::vector<DataMonitor*> monitor_set;

  ImGui::Begin("Data Monitor", swth);
  if (ImGui::Button("Monitor - Add on")) {
    monitor_set.push_back(new DataMonitor());
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

  if (ImGui::Button("Close")) switches.show_log_window = false;
  ImGui::End();
}

static void Imgui_Drawing() {
  // Start the Dear ImGui frame
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  Show_Custom_Window();
  if (switches.show_demo_window)
    ImGui::ShowDemoWindow(&switches.show_demo_window);
  Show_Log_Window(&switches.show_log_window);
  Show_DataMonitor(&switches.show_data_monitor);

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}