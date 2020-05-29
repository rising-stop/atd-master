CMAKE_MINIMUM_REQUIRED(VERSION 3.0.0)
project(atd-master)

set(ATD_MASTER_ROOT ${PROJECT_SOURCE_DIR})
set(PROTOBUF_PROTOC_PATH ${ATD_MASTER_ROOT}/3rd_party/protobuf-3.3.0/x86/bin)
set(PROTOBUF_INCLUDE_PATH ${ATD_MASTER_ROOT}/3rd_party/protobuf-3.3.0/x86/include)
set(PROTO_MSG_PATH ${ATD_MASTER_ROOT}/protobuf_msg)
set(PROTO_MSG_IMPLEMENT_PATH ${ATD_MASTER_ROOT}/protobuf_msg)
set(PROTOBUF_LIBRARY_X86_PATH ${ATD_MASTER_ROOT}/3rd_party/protobuf-3.3.0/x86/lib)
set(PROTOBUF_LIBRARY_ARM_PATH ${ATD_MASTER_ROOT}/3rd_party/protobuf-3.3.0/pegasus/lib)
if (USE_CC_PEGASUS)
	message(STATUS "================  -] Config for PEGASUS-AARACH64-linux [-  ================")
	set(PROTOBUF_LIBRARY_PATH ${PROTOBUF_LIBRARY_ARM_PATH})
else()
	message(STATUS "================  -] Config for x86-64 [-  ================")
	set(PROTOBUF_LIBRARY_PATH ${PROTOBUF_LIBRARY_X86_PATH})
endif()

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fPIC")

include_directories(${ATD_MASTER_ROOT})
include_directories(${PROTOBUF_INCLUDE_PATH})
include_directories(${PROTO_MSG_IMPLEMENT_PATH})

link_directories(${PROTOBUF_LIBRARY_PATH})

message(STATUS "ATD_MASTER_ROOT: ${ATD_MASTER_ROOT}")
message(STATUS "PROTOBUF_INCLUDE_PATH: ${PROTOBUF_INCLUDE_PATH}")
message(STATUS "PROTOBUF_PROTOC_PATH: ${PROTOBUF_PROTOC_PATH}")
message(STATUS "PROTOBUF_LIBRARY_PATH: ${PROTOBUF_LIBRARY_PATH}")
message(STATUS "PROTO_MSG_PATH: ${PROTO_MSG_PATH}")
message(STATUS "PROTO_MSG_IMPLEMENT_PATH: ${PROTO_MSG_IMPLEMENT_PATH}")


add_subdirectory(protobuf_msg)
add_subdirectory(modules)
