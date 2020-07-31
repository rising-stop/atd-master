#include "common_frame.h"

void glfw_error_callback(int error, const char* description) {
  std::cerr << "Glfw Error: " << error << " " << description << std::endl;
}

void window_size_callback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}

GLFWwindow* OpenGL_Frame::Init4GLFW(int window_width, int window_height) {
  /* Setup window */
  glfwSetErrorCallback(glfw_error_callback);
  if (!glfwInit()) return nullptr;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  // only glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  GLFWwindow* window = glfwCreateWindow(
      window_width, window_height, "ATD Autonomous Driving System", NULL, NULL);
  if (window == nullptr) {
    glfwTerminate();
    throw std::exception();
  }

  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, window_size_callback);
  glfwSwapInterval(1);

  if (gl3wInit()) {
    return nullptr;
  }

  return window;
}

void Imgui_Setting(GLFWwindow* ptr_window) {
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  (void)io;
  // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable
  // Keyboard Controls io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; //
  // Enable Gamepad Controls

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();
  // ImGui::StyleColorsClassic();

  // Setup Platform/Renderer bindings
  // GL 3.0 + GLSL 330
  const char* glsl_version = "#version 330";
  ImGui_ImplGlfw_InitForOpenGL(ptr_window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);
}

void OpenGL_Frame::register_CallBack(int, std::function<void()>) {}

void OpenGL_Frame::spin() {
  auto ptr_window = Init4GLFW(800, 600);
  Imgui_Setting(ptr_window);

  while (!glfwWindowShouldClose(ptr_window)) {
    glfwPollEvents();  // checking outsource events
    for (auto call_back : rendering_events_) {
      call_back.second();
    }
    int view_width, view_height;
    glfwGetFramebufferSize(ptr_window, &view_width, &view_height);
    glViewport(0, 0, view_width, view_height);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(ptr_window);
  }

  glfwDestroyWindow(ptr_window);
  glfwTerminate();
}
