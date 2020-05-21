CMAKE_MINIMUM_REQUIRED(VERSION 3.0.0)
project(atd-master)

# predefined symbol : USE_CC_PEGASUS(OFF)
#                     BUILD_TYPE(Debug)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fPIC")

set(ATD_MASTER_ROOT ${PROJECT_SOURCE_DIR})
include_directories(${ATD_MASTER_ROOT})

set(ATD_MASTER_3RDPARTY_LIBS_DIR ${ATD_MASTER_ROOT}/3rd_party)

if (USE_CC_PEGASUS)

    message(STATUS "================  -] building PEGASUS-AARACH64-linux [-  ================")

    set(PROTOBUF_INSTALL_PATH ${ATD_MASTER_3RDPARTY_LIBS_DIR}/protobuf-3.3.0/pegasus)
    include_directories(${PROTOBUF_INSTALL_PEGASUS}/include)
    link_directories(${PROTOBUF_INSTALL_PEGASUS}/lib)
 
    set(EIGEN3_INSTALL_PATH ${ATD_MASTER_3RDPARTY_LIBS_DIR}/eigen-eigen-b9cd8366d4e8/pegasus)
    include_directories(${EIGEN3_INSTALL_PEGASUS}/include)
    include_directories(${EIGEN3_INSTALL_PATH})
    include_directories(${EIGEN3_INSTALL_PATH}/include)
    include_directories(${EIGEN3_INSTALL_PATH}/include/eigen3)

    set(LCM_INSTALL_PATH ${ATD_MASTER_3RDPARTY_LIBS_DIR}/lcm-1.3.0-saic/pegasus)
    include_directories(${LCM_INSTALL_PEGASUS}/include)
    link_directories(${LCM_INSTALL_PEGASUS}/lib)

else()

    message(STATUS "================  -] building x86-64 [-  ================")

    set(PROTOBUF_INSTALL_PATH ${ATD_MASTER_3RDPARTY_LIBS_DIR}/protobuf-3.3.0/x86)
    include_directories(${PROTOBUF_INSTALL_PEGASUS}/include)
    link_directories(${PROTOBUF_INSTALL_PEGASUS}/lib)

    set(EIGEN3_INSTALL_PATH ${ATD_MASTER_3RDPARTY_LIBS_DIR}/eigen-eigen-b9cd8366d4e8/x86)
    include_directories(${EIGEN3_INSTALL_PATH})
    include_directories(${EIGEN3_INSTALL_PATH}/include)
    include_directories(${EIGEN3_INSTALL_PATH}/include/eigen3)

    set(LCM_INSTALL_PATH ${ATD_MASTER_3RDPARTY_LIBS_DIR}/lcm-1.3.0-saic/x86)
    include_directories(${LCM_INSTALL_PEGASUS}/include)
    link_directories(${LCM_INSTALL_PEGASUS}/lib)

endif()

message("ATD_MASTER_ROOT: ${ATD_MASTER_ROOT}")
message("PROTOBUF_INSTALL_PATH: ${PROTOBUF_INSTALL_PATH}")
message("EIGEN3_INSTALL_PATH: ${EIGEN3_INSTALL_PATH}")
message("LCM_INSTALL_PATH: ${LCM_INSTALL_PATH}")

add_subdirectory(modules)
