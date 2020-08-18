CMAKE_MINIMUM_REQUIRED(VERSION 3.0.0)
project(atd-master)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 11)

set(ATD_MASTER_ROOT ${PROJECT_SOURCE_DIR})
set(LCM_INSTALL_PATH ${ATD_MASTER_ROOT}/3rd_party/lcm-1.3.0-saic)
set(PROTO_MSG_PATH ${ATD_MASTER_ROOT}/protobuf_msg)
set(PROTO_MSG_IMPLEMENT_PATH ${ATD_MASTER_ROOT}/protobuf_msg)

set(LCM_GEN_PATH ${LCM_INSTALL_PATH}/bin)
set(LCM_INCLUDE_PATH ${LCM_INSTALL_PATH}/include)
set(LCM_LIBRARY_PATH ${LCM_INSTALL_PATH}/lib)

find_package(Protobuf REQUIRED)

include_directories(${ATD_MASTER_ROOT})
include_directories(${LCM_INCLUDE_PATH})
include_directories(${PROTO_MSG_IMPLEMENT_PATH})
include_directories(${PROTOBUF_INCLUDE_DIRS})

link_directories(${LCM_LIBRARY_PATH})

message(STATUS "ATD_MASTER_ROOT: ${ATD_MASTER_ROOT}")
message(STATUS "PROTO_MSG_PATH: ${PROTO_MSG_PATH}")
message(STATUS "PROTO_MSG_IMPLEMENT_PATH: ${PROTO_MSG_IMPLEMENT_PATH}")

add_definitions(-Wno-format-security)

add_subdirectory(protobuf_msg)
add_subdirectory(modules)
