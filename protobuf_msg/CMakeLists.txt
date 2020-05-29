CMAKE_MINIMUM_REQUIRED(VERSION 3.0.0)
project(protobuf_msg)

set(PROTOC_PATH ${PROTOBUF_INSTALL_PATH}/x86/bin)
set(PROTOBUF_LIB_DIR ${PROTOBUF_INSTALL_PATH}/x86/lib)
set(PROTOBUF_HEADER_DIR ${ATD_MASTER_ROOT}/protobuf_msg)
set(PROTOBUF_SRC_DIR ${ATD_MASTER_ROOT}/protobuf_msg)

message("start generating protobuf message")
execute_process(COMMAND sh protobuf_generate_cpp.sh ${PROTOBUF_LIB_DIR} ${PROTOC_PATH} ${PROTOBUF_HEADER_DIR} ${PROTOBUF_SRC_DIR}
	WORKING_DIRECTORY ${PROTOBUF_HEADER_DIR}
	RESULT_VARIABLE RES
	OUTPUT_VARIABLE OUT_RES
	ERROR_VARIABLE ERROR_RES)

#if( NOT "${RES}" STREQUAL "" )
#	message("${RES}")
#endif()
if( NOT "${OUT_RES}" STREQUAL "" )
	message("${OUT_RES}")
endif()
if( NOT "${ERROR_RES}" STREQUAL "" )
	message(FATAL_ERROR "${ERROR_RES}")
else()
	message("protobuf_msg generation done")
endif()
