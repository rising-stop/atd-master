#include "modules/neo_adp/display_elements/common_frame.h"

/* main function */
int main(int args, char** argv) {
  // Our state
  bool ImGui = true;
  bool the_same_color = false;
  bool draw_trangle_without_render = false;
  bool draw_trangle = false;
  bool bonus_draw_line = false;
  bool bonus_draw_another_trangle = false;
  unsigned int VBO, VAO, EBO;
  bool show_demo_window = true;

  GLuint VertexArrayID;
  glGenVertexArrays(1, &VertexArrayID);
  glBindVertexArray(VertexArrayID);

  GLuint programID = LoadShaders("SimpleVertexShader.vertexshader",
                                 "SimpleFragmentShader.fragmentshader");
  ImVec4 v1 = ImVec4(-0.25f, -0.25f, 0.0f, 1.00f);
  ImVec4 v2 = ImVec4(0.25f, -0.25f, 0.0f, 1.00f);
  ImVec4 v3 = ImVec4(0.0f, 0.25f, 0.0f, 1.00f);

  GLuint vertexbuffer;

  // Main loop
  while (!glfwWindowShouldClose(ptr_window)) {

    /* 创建 VBO */
    GLfloat g_vertex_buffer_data[] = {
        v1.x, v1.y, v1.z, v2.x, v2.y, v2.z, v3.x, v3.y, v3.z,
    };
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER,
                 vertexbuffer); /* 顶点缓冲对象 GL_ARRAY_BUFFER */
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data),
                 g_vertex_buffer_data, GL_STATIC_DRAW);
    /**
     * GL_STATIC_DRAW ：数据不会或几乎不会改变。
     * GL_DYNAMIC_DRAW：数据会被改变很多。
     * GL_STREAM_DRAW ：数据每次绘制时都会改变。*/
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (void*)0);
    /**
     * 第一个参数指定我们要配置的顶点属性。顶点着色器中使用layout(location=0)定义了position顶点属性的位置值(Location)
     * 第二个参数指定顶点属性的大小。顶点属性是一个vec3，它由3个值组成，所以大小是
     * 第三个参数指定数据的类型，这里是GL_FLOAT(GLSL中vec*都是由浮点数值组成的)。
     * 第四个参数定义我们是否希望数据被标准化(Normalize)。如果我们设置为GL_TRUE，所有数据都会被映射到0（对于有符号型signed数据是-1）到1之间。我们把它设置为GL_FALSE
     * 第五个参数叫做步长(Stride)，它告诉我们在连续的顶点属性组之间的间隔。由于下个组位置数据在3个float之后，我们把步长设置为3sizeof(float)。要注意的是由于我们知道这个数组是紧密排列的（在两个顶点属性之间没有空隙）我们也可以设置为0来让OpenGL决定具体步长是多少（只有当数值是紧密排列时才可用）。一旦我们有更多的顶点属性，我们就必须更小心地定义每个顶点属性之间的间隔
     * 最后一个参数的类型是void*，所以需要我们进行这个奇怪的强制类型转换。它表示位置数据在缓冲中起始位置的偏移量(Offset)。由于位置数据在数组的开头，所以这里是0。
     */
    glEnableVertexAttribArray(0);
    glUseProgram(programID); /* 每个着色器调用和渲染调用都会使用这个着色器了*/
    glDrawArrays(GL_TRIANGLES, 0, 3);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::Begin("change vertex", &ImGui, ImGuiWindowFlags_MenuBar);
    ImGui::SliderFloat("", &v3.y, -1.0f, 1.0f, "v3.y = %.3f");
    ImGui::End();

    if (show_demo_window) ImGui::ShowDemoWindow(&show_demo_window);

    // Rendering
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  }

  // Cleanup
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(ptr_window);
  glfwTerminate();

  return 0;
}
