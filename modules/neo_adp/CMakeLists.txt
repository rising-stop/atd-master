CMAKE_MINIMUM_REQUIRED(VERSION 3.0.0)
project(NeoADP)

include_directories("${ATD_MASTER_ROOT}/modules/neo_adp/libs/gl3w")

#set include files: *.h, *.hpp, *.hh in this project
file(GLOB_RECURSE PROJECT_HEADERS
  "imgui_module/*.h"
  "imgui-opengl3/*.h"
  "libs/*.h")
#set source files:*.cpp, *.c, *.cc in this project
file(GLOB_RECURSE PROJECT_SOURCES
  "imgui_module/*.cpp"
  "imgui-opengl3/*.cpp"
  "libs/*.c"
  "main.cc")
#set library files: *.lib in this project

# file(GLOB_RECURSE PROJECT_LIBRARIES)

add_definitions(-D IMGUI_IMPL_OPENGL_LOADER_GL3W)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS})
target_link_libraries(${PROJECT_NAME} ${PROJECT_LIBRARIES} -lGL -ldl -lglfw3 -lpthread -lX11)

