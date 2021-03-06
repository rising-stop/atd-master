#pragma once

#include "modules/common/common_header.h"

/**
 * @brief max buffer size for log monitor
 */
#define LogMonitor_BufferSize 11

/**
 * @brief max time (frame) waiting for reconnection
 */
#define DateDispatcher_TimeOut 100

/**
 * @brief max buffer size for data monitor
 */
#define DataMonitor_Max_BufferSize 3600

/**
 * @brief max buffer size for data monitor
 */
#define DataMonitor_Min_BufferSize 10

/**
 * @brief time out for waiting planning responsing the calibration variable's
 * alternation
 */
#define Calibrator_TimeOut 10

/**
 * @brief times new value of some calibration sended
 */
#define Calibrator_AlterCommand_SendTime 3

/**
 * @brief define data seg names
 */
#define Data_Seg_Name_LCMProtocol "LCMSubscriber"
#define Data_Seg_Name_DataMonitor "DataMonitor"
#define Data_Seg_Name_LogMonitor "LogMonitor"

