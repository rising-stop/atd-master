CMAKE_MINIMUM_REQUIRED(VERSION 3.0.0)
project(atd-master)

# BUILD_TYPE(Debug)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fPIC")

set(ATD_MASTER_ROOT ${PROJECT_SOURCE_DIR})
include_directories(${ATD_MASTER_ROOT})

set(ATD_MASTER_3RDPARTY_LIBS_DIR ${ATD_MASTER_ROOT}/3rd_party)
set(PROTOBUF_INSTALL_PATH ${ATD_MASTER_3RDPARTY_LIBS_DIR}/protobuf-3.3.0)
set(PROTO_MSG_PATH ${ATD_MASTER_ROOT}/protobuf_msg)
include_directories(${PROTO_MSG_PATH})

if (USE_CC_PEGASUS)

    message(STATUS "================  -] building PEGASUS-AARACH64-linux [-  ================")

    include_directories(${PROTOBUF_INSTALL_PATH}/pegasus/include)
	link_directories(${PROTOBUF_INSTALL_PATH}/pegasus/lib)
 
else()

    message(STATUS "================  -] building x86-64 [-  ================")

    include_directories(${PROTOBUF_INSTALL_PATH}/x86/include)
	link_directories(${PROTOBUF_INSTALL_PATH}/x86/lib)

endif()

message("ATD_MASTER_ROOT: ${ATD_MASTER_ROOT}")
message("PROTOBUF_INSTALL_PATH: ${PROTOBUF_INSTALL_PATH}")

add_subdirectory(protobuf_msg)
# add_subdirectory(modules)
