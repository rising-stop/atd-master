#include <functional>
#include <iostream>
#include <map>

#include "modules/common/utility/utility.h"
#include "modules/neo_adp/imgui-opengl3/imgui_impl_glfw.h"
#include "modules/neo_adp/imgui-opengl3/imgui_impl_opengl3.h"
#include "modules/neo_adp/imgui_module/imgui.h"
#include "modules/neo_adp/libs/gl3w/GL/gl3w.h"
#include "modules/neo_adp/libs/glfw/include/GLFW/glfw3.h"
#include "modules/neo_adp/opengl_test/shader.h"

class OpenGL_Frame : public atd::utility::Singleton {
  friend class atd::utility::Singleton;

 public:
  /**
   * @throw Exception of register failure
   */
  void register_CallBack(int, std::function<void()>) {}
  void spin();

 private:
  GLFWwindow* Init4GLFW(int, int);
  void Imgui_Setting(GLFWwindow*);

  std::map<int, std::function<void()>> rendering_events_;

  OpenGL_Frame() = default;
  ~OpenGL_Frame() = default;
};