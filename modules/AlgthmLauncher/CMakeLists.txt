CMAKE_MINIMUM_REQUIRED(VERSION 3.1.10)
project(AlgthmLauncher)

#set include files: *.h, *.hpp, *.hh in this project
#file(GLOB_RECURSE PROJECT_HEADERS  "")
#set source files:*.cpp, *.c, *.cc in this project
file(GLOB_RECURSE PROJECT_SOURCES  
  "main.cc")
#set library files: *.lib in this project


if (USE_CC_PEGASUS)
    file(GLOB_RECURSE PROJECT_LIBRARIES
    "../../upload_files/lib/libcommon.so"
    )
else()
    file(GLOB_RECURSE PROJECT_LIBRARIES
    "../../bin/lib/libcommon.so"
    )
endif()

# add_library(${PROJECT_NAME} SHARED ${PROJECT_SOURCES} ${PROJECT_HEADERS})
# target_link_libraries(${PROJECT_NAME} ${PROJECT_LIBRARIES} protobuf-lite lcm pthread)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS})
target_link_libraries(${PROJECT_NAME} ${PROJECT_LIBRARIES})

