#include "imgui_basic_component.h"

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

void ImGui_Components::set_BoxMessage(const std::string& str) {
  box_message_ = str;
  flag_is_show_msgbox_ = true;
}

void ImGui_Components::MessageBox() {
  if (!flag_is_show_msgbox_) {
    return;
  }
  if (!ImGui::Begin("Message", &flag_is_show_msgbox_,
                    ImGuiWindowFlags_AlwaysAutoResize)) {
    ImGui::End();
    return;
  }
  ImGui::Text(box_message_.c_str());
  if (ImGui::Button("Close")) {
    flag_is_show_msgbox_ = false;
  }
  ImGui::End();
}
