#pragma once

#include <cmath>

#include "modules/common/common_header.h"
#include "modules/neo_adp/display_elements/common_frame.hpp"
#include "protobuf_msg/planning_log.pb.h"

class GL_Implement {
 public:
  /**
   * @brief draw item label in gl windows
   */
  static void drawLabel(const ThreadData &td, ddVec3_In pos, const char *name);

  /**
   * @brief draw item grid in gl windows
   */
  static void drawGrid(const ThreadData &td);

  /**
   * @brief main planning elements display process
   */
  static void draw_PlanningElements(const ThreadData &td);

  /**
   * @brief draw text info in the window
   */
  static void drawText(const ThreadData &td);

  /**
   * @brief debug draw examples
   */
  static void drawFrustum(const ThreadData &td);

 private:
  /**
   * @brief box data structure converter, convert standard heading included box
   * data to 8 conner points
   */
  static void convert_BoxData(ddVec3 *corner_points, ddVec3 origin,
                              const float width, const float height,
                              const float deepth, const float heading);
};

