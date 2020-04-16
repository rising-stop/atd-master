CMAKE_MINIMUM_REQUIRED(VERSION 3.1.10)
project(test)

#set include files: *.h, *.hpp, *.hh in this project
FILE(GLOB_RECURSE PROJECT_HEADERS  
  "../test_unit/*.h")
#set source files:*.cpp, *.c, *.cc in this project
FILE(GLOB_RECURSE PROJECT_SOURCES  
  "main.cc")
#set library files: *.lib in this project


if (USE_CC_PEGASUS)
    FILE(GLOB_RECURSE PROJECT_LIBRARIES
    "../../bin_aarch64_pegasus/nodes/libplanning_common.so"
    )
else()
    FILE(GLOB_RECURSE PROJECT_LIBRARIES
    "../../bin/nodes/libplanning_common.so"
    )
endif()

# add_library(${PROJECT_NAME} SHARED ${PROJECT_SOURCES} ${PROJECT_HEADERS})
# target_link_libraries(${PROJECT_NAME} ${PROJECT_LIBRARIES} protobuf-lite lcm pthread)

add_executable(app_${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS})
target_link_libraries(app_${PROJECT_NAME} ${PROJECT_LIBRARIES})

