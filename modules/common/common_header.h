/**
 * common file for planning modules
 *
 * author   : liangle
 *
 */

#ifndef PLANNING_HEADER_H_
#define PLANNING_HEADER_H_

/**
 * functional code pieces
 */

#include "modules/common/common_support/class_support.h"
#include "modules/common/common_support/nonlinear_relation/nonlinear_relation.h"
#include "modules/common/common_support/numerical_support.h"
#include "modules/common/common_support/step_support.h"

/**
 * ipc functions
 */

/**
 * observer functions
 */

#include "modules/common/common_support/observer/ObservingLogging.h"
#include "modules/common/common_support/timer/timer.h"

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

#endif