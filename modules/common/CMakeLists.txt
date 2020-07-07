CMAKE_MINIMUM_REQUIRED(VERSION 3.0.0)
project(common)

file(GLOB_RECURSE PROJECT_HEADERS  
  "../common/*.h"
  "../protobuf_msg/*.h")
file(GLOB_RECURSE PROJECT_SOURCES  
  "../common/*.cc"
  "../protobuf_msg/*.cc")

add_library(${PROJECT_NAME} SHARED ${PROJECT_SOURCES} ${PROJECT_HEADERS})
target_link_libraries(${PROJECT_NAME} pthread lcm)
