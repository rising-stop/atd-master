CMAKE_MINIMUM_REQUIRED(VERSION 3.0.0)
project(common)

#set include files: *.h, *.hpp, *.hh in this project
FILE(GLOB_RECURSE PROJECT_HEADERS  
  "../common/*.h"
  "../protobuf_msg/*.h")
#set source files:*.cpp, *.c, *.cc in this project
FILE(GLOB_RECURSE PROJECT_SOURCES  
  "../common/*.cc"
  "../protobuf_msg/*.cc")

#set library files: *.lib in this project
# if (USE_CC_PEGASUS)
#     FILE(GLOB_RECURSE PROJECT_LIBRARIES
#     )
# else()
#     FILE(GLOB_RECURSE PROJECT_LIBRARIES
#     )
# endif()

add_library(${PROJECT_NAME} SHARED ${PROJECT_SOURCES} ${PROJECT_HEADERS})
target_link_libraries(${PROJECT_NAME} -pthread)

#add_executable(app_${PROJECT_NAME} "src/main.cc" ${PROJECT_SOURCES} ${PROJECT_HEADERS})
#target_link_libraries(app_${PROJECT_NAME} ${PROJECT_LIBRARIES} protobuf-lite lcm pthread)

