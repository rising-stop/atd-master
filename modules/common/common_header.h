/**
 * common file for planning modules
 *
 * author   : liangle
 *
 */

#pragma once

/**
 * ipc functions
 */
#include "modules/common/utility/ipc/lcm_dispatcher/lcm_dispatcher.h"
#include "modules/common/utility/ipc/semaphore_mutex.h"
#include "modules/common/utility/ipc/shared_memory.h"
#include "modules/common/utility/ipc/shm_protocol.h"

/**
 * file handler
 */
#include "modules/common/utility/file_parser/csv_parser.h"
#include "modules/common/utility/file_parser/planning_log_parser.h"
#include "modules/common/utility/file_parser/xml_parser.h"

/**
 * observer functions
 */
#include "modules/common/utility/timer.h"

/**
 * common math functions
 */
#include "modules/common/math/EuclideanGeometry/basicgeometry.h"
#include "modules/common/math/EuclideanGeometry/line/discrete_line2D.h"
#include "modules/common/math/EuclideanGeometry/line/monotonic_discrete_line2D.h"
#include "modules/common/math/EuclideanGeometry/line/polynomial2D.h"
#include "modules/common/math/EuclideanGeometry/point/arrow2D.h"
#include "modules/common/math/EuclideanGeometry/point/point2D.h"
#include "modules/common/math/EuclideanGeometry/point/sl_point2D.h"
#include "modules/common/math/EuclideanGeometry/point/vectorXD.h"
#include "modules/common/math/NumericalComputing/function_fitting.h"
#include "modules/common/math/NumericalComputing/interpolation.h"
#include "modules/common/math/NumericalComputing/numerical_integration.h"
#include "modules/common/math/mathcommon.h"

/**
 * tools
 */
#include "modules/common/tools/logging/debug_logging.h"
