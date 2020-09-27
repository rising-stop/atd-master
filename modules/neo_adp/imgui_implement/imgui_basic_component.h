#pragma once

#include "modules/neo_adp/imgui-opengl3/imgui_impl_glfw.h"
#include "modules/neo_adp/imgui-opengl3/imgui_impl_opengl3.h"
#include "modules/neo_adp/imgui_module/imgui.h"

class ImGui_Components {
 public:
  virtual void render() = 0;

 protected:
  void HelpMarker(const char* desc);

 public:
  ImGui_Components() = default;
  ~ImGui_Components() = default;
};