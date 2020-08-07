message(STATUS "start generating protobuf message")

execute_process(COMMAND sh protobuf_generate_cpp.sh  
	${PROTO_MSG_PATH} ${PROTO_MSG_PATH}
	WORKING_DIRECTORY ${PROTO_MSG_PATH}
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
