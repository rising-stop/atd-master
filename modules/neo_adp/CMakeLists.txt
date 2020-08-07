CMAKE_MINIMUM_REQUIRED(VERSION 3.0.0)
project(NeoADP)

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_GLIBCXX_USE_CXX11_ABI=0")

include_directories("${ATD_MASTER_ROOT}/modules/neo_adp/libs/gl3w")

#set include files: *.h, *.hpp, *.hh in this project
file(GLOB_RECURSE PROJECT_HEADERS
  "imgui_module/*.h"
  "imgui-opengl3/*.h"
  "libs/*.h"
  "../../protobuf_msg/*.h")
#set source files:*.cpp, *.c, *.cc in this project
file(GLOB_RECURSE PROJECT_SOURCES
  "imgui_module/*.cpp"
  "imgui-opengl3/*.cpp"
  "libs/*.c"
  "display_elements/*.hpp"
  "display_elements/*.cc"
  "../../protobuf_msg/*.cc"
  "main.cc")
#set library files: *.lib in this project

file(GLOB_RECURSE PROJECT_LIBRARIES
  "../../bin/lib/libcommon.so"
  "../../3rd_party/lcm-1.3.0-saic/lib/liblcm.so"
)

add_definitions(-D IMGUI_IMPL_OPENGL_LOADER_GL3W)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS})
target_link_libraries(${PROJECT_NAME} ${PROJECT_LIBRARIES} -lGL -ldl -lglfw3 -lpthread -lX11 libprotobuf-lite.a)
