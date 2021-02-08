#include "modules/neo_adp/opengl_implement/gl_implement.hpp"
#include "modules/neo_adp/imgui_implement/imgui_implement.h"

/* main function */
int main(int args, char** argv) {
  GL_Implement::init();
  ImGui_ComponentManager::init();
  
  atd::utility::Singleton::instance<OpenGL_Frame>()->spin();
  exit(EXIT_SUCCESS);
}
