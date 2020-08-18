CMAKE_MINIMUM_REQUIRED(VERSION 3.0.0)
project(LogParser)

#set include files: *.h, *.hpp, *.hh in this project
file(GLOB_RECURSE PROJECT_HEADERS
  "../../protobuf_msg/*.h")
#set source files:*.cpp, *.c, *.cc in this project
file(GLOB_RECURSE PROJECT_SOURCES
  "../../protobuf_msg/*.cc"
  "main.cc")
#set library files: *.lib in this project

file(GLOB_RECURSE PROJECT_LIBRARIES
  "../../bin/lib/libcommon.so"
  "../../3rd_party/lcm-1.3.0-saic/lib/liblcm.so"
)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS})
target_link_libraries(${PROJECT_NAME} ${PROJECT_LIBRARIES} -lpthread libprotobuf-lite.a)
