CMAKE_MINIMUM_REQUIRED(VERSION 3.0.0)
project(common)

file(GLOB_RECURSE PROJECT_HEADERS  
  "../common/*.h")
file(GLOB_RECURSE PROJECT_SOURCES  
  "../common/*.cc")

file(GLOB_RECURSE COMMON_LD_LIBRARY
  "../../3rd_party/lcm-1.3.0-saic/lib/liblcm.so")

add_library(${PROJECT_NAME} SHARED ${PROJECT_SOURCES} ${PROJECT_HEADERS})
target_link_libraries(${PROJECT_NAME} pthread ${COMMON_LD_LIBRARY})
