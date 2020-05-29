CMAKE_MINIMUM_REQUIRED(VERSION 3.0.0)
project(protobuf_msg)

set(PROTOC_PATH ${PROTOBUF_PROTOC_PATH})
set(PROTOBUF_LIB_DIR ${PROTOBUF_LIBRARY_X86_PATH})
set(PROTOBUF_HEADER_DIR ${PROTO_MSG_PATH})
set(PROTOBUF_SRC_DIR ${PROTO_MSG_IMPLEMENT_PATH})

message(STATUS "start generating protobuf message")
execute_process(COMMAND sh protobuf_generate_cpp.sh ${PROTOBUF_LIB_DIR} ${PROTOC_PATH} ${PROTOBUF_HEADER_DIR} ${PROTOBUF_SRC_DIR}
	WORKING_DIRECTORY ${PROTOBUF_HEADER_DIR}
	RESULT_VARIABLE RES
	OUTPUT_VARIABLE OUT_RES
	ERROR_VARIABLE ERROR_RES)

#if( NOT "${RES}" STREQUAL "" )
#	message("${RES}")
#endif()
if( NOT "${OUT_RES}" STREQUAL "" )
	message(STATUS "${OUT_RES}")
endif()
if( NOT "${ERROR_RES}" STREQUAL "" )
	message(FATAL_ERROR "${ERROR_RES}")
else()
	message(STATUS "protobuf_msg generation done")
endif()
