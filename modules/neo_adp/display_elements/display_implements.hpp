#pragma once

#include <cmath>

#include "common_frame.hpp"
#include "modules/common/common_header.h"
#include "protobuf_msg/planning_log.pb.h"

static void drawLabel(const ThreadData &td, ddVec3_In pos, const char *name) {
  if (!keys.showLabels) {
    return;
  }
  // Only draw labels inside the camera frustum.
  if (camera.isPointInsideFrustum(pos[0], pos[1], pos[2])) {
    const ddVec3 textColor = {0.8f, 0.8f, 1.0f};
    dd::projectedText(td.ddContext, name, pos, textColor,
                      toFloatPtr(camera.vpMatrix), 0, 0, WindowWidth,
                      WindowHeight, 0.3f);
  }
}

static void drawGrid(const ThreadData &td) {
  if (keys.showGrid) {
    dd::xzSquareGrid(td.ddContext, -40.0f, 40.0f, -20.0f, 100.0f, -1.0f, 5.0f,
                     dd::colors::DarkGray);
  }
}

static void convert_BoxData(ddVec3 *corner_points, ddVec3 origin,
                            const float width, const float height,
                            const float deepth, const float heading) {
  float init_ang = atan2(height, width);
  float diag_length = sqrt(width * width + height * height) / 2.0f;
  corner_points[0][0] = origin[0] + diag_length * sin(heading + init_ang);
  corner_points[0][1] = origin[1];
  corner_points[0][2] = origin[2] + diag_length * cos(heading + init_ang);
  corner_points[1][0] = origin[0] - diag_length * sin(heading + init_ang);
  corner_points[1][1] = origin[1];
  corner_points[1][2] = origin[2] + diag_length * cos(heading + init_ang);
  corner_points[2][0] = origin[0] - diag_length * sin(heading + init_ang);
  corner_points[2][1] = origin[1];
  corner_points[2][2] = origin[2] - diag_length * cos(heading + init_ang);
  corner_points[3][0] = origin[0] + diag_length * sin(heading + init_ang);
  corner_points[3][1] = origin[1];
  corner_points[3][2] = origin[2] - diag_length * cos(heading + init_ang);
  corner_points[4][0] = corner_points[0][0];
  corner_points[4][1] = corner_points[0][1] + deepth;
  corner_points[4][2] = corner_points[0][2];
  corner_points[5][0] = corner_points[1][0];
  corner_points[5][1] = corner_points[1][1] + deepth;
  corner_points[5][2] = corner_points[1][2];
  corner_points[6][0] = corner_points[2][0];
  corner_points[6][1] = corner_points[2][1] + deepth;
  corner_points[6][2] = corner_points[2][2];
  corner_points[7][0] = corner_points[3][0];
  corner_points[7][1] = corner_points[3][1] + deepth;
  corner_points[7][2] = corner_points[3][2];
}

static void draw_PlanningElements(const ThreadData &td) {
  static atd::protocol::OPENGL_ELEMENT adp_msg;
  atd::utility::Singleton::instance<DataDispatcher>()->get_GLElements(adp_msg);

  ddVec3 origin = {0.0f, 0.0f, 0.0f};
  drawLabel(td, origin, "ego vehicle");
  dd::box(td.ddContext, origin, dd::colors::BlueViolet, 1.8f, 1.5f, 4.68f);
  dd::point(td.ddContext, origin, dd::colors::White, 5.0f);

  auto box_set = adp_msg.box_set();
  for (auto single_box : box_set) {
    ddVec3 box_origin = {single_box.origin().y(), single_box.origin().z(),
                         single_box.origin().x()};
    ddVec3 corner_points[8];
    convert_BoxData(corner_points, box_origin, single_box.width(),
                    single_box.height(), single_box.deepth(),
                    single_box.heading());
    if (single_box.discription() == "CIPV") {
      drawLabel(td, box_origin, "CIPV");
      dd::box(td.ddContext, corner_points, dd::colors::Green);
      dd::point(td.ddContext, box_origin, dd::colors::White, 3.0f);
    } else if (single_box.discription() == "TOS") {
      drawLabel(td, box_origin, "TOS");
      dd::box(td.ddContext, corner_points, dd::colors::Red);
      dd::point(td.ddContext, box_origin, dd::colors::White, 3.0f);
    } else if (single_box.discription() == "Selected") {
      drawLabel(td, box_origin, "Selected");
      dd::box(td.ddContext, corner_points, dd::colors::Yellow);
      dd::point(td.ddContext, box_origin, dd::colors::White, 3.0f);
    } else {
      drawLabel(td, box_origin, single_box.discription().c_str());
      dd::box(td.ddContext, corner_points, dd::colors::White);
      dd::point(td.ddContext, box_origin, dd::colors::White, 3.0f);
    }
  }

  auto line_set = adp_msg.line_set();
  for (auto single_line : line_set) {
    auto point_size = single_line.sample_points().size();
    if (point_size > 2) {
      ddVec3 point_ori{single_line.sample_points(point_size / 2).y(),
                       single_line.sample_points(point_size / 2).z(),
                       single_line.sample_points(point_size / 2).x()};
      drawLabel(td, point_ori, single_line.discription().c_str());
      dd::point(td.ddContext, point_ori, dd::colors::White, 5.0f);
      for (uint32_t index = 0; index < (point_size - 1); index++) {
        ddVec3 seg_start{single_line.sample_points(index).y(),
                         single_line.sample_points(index).z(),
                         single_line.sample_points(index).x()};
        ddVec3 seg_end{single_line.sample_points(index + 1).y(),
                       single_line.sample_points(index + 1).z(),
                       single_line.sample_points(index + 1).x()};
        dd::line(td.ddContext, seg_start, seg_end, dd::colors::Yellow);
      }
    }
  }
}

static void drawMiscObjects(const ThreadData &td) {
  // Start a row of objects at this position:
  ddVec3 origin = {-15.0f, 0.0f, 0.0f};

  // Box with a point at it's center:
  drawLabel(td, origin, "box");
  dd::box(td.ddContext, origin, dd::colors::Blue, 1.5f, 1.5f, 1.5f);
  dd::point(td.ddContext, origin, dd::colors::White, 15.0f);
  origin[0] += 3.0f;

  // Sphere with a point at its center
  drawLabel(td, origin, "sphere");
  dd::sphere(td.ddContext, origin, dd::colors::Red, 1.0f);
  dd::point(td.ddContext, origin, dd::colors::White, 15.0f);
  origin[0] += 4.0f;

  // Two cones, one open and one closed:
  const ddVec3 condeDir = {0.0f, 2.5f, 0.0f};
  origin[1] -= 1.0f;

  drawLabel(td, origin, "cone (open)");
  dd::cone(td.ddContext, origin, condeDir, dd::colors::Yellow, 1.0f, 2.0f);
  dd::point(td.ddContext, origin, dd::colors::White, 15.0f);
  origin[0] += 4.0f;

  drawLabel(td, origin, "cone (closed)");
  dd::cone(td.ddContext, origin, condeDir, dd::colors::Cyan, 0.0f, 1.0f);
  dd::point(td.ddContext, origin, dd::colors::White, 15.0f);
  origin[0] += 4.0f;

  // Axis-aligned bounding box:
  const ddVec3 bbMins = {-1.0f, -0.9f, -1.0f};
  const ddVec3 bbMaxs = {1.0f, 2.2f, 1.0f};
  const ddVec3 bbCenter = {(bbMins[0] + bbMaxs[0]) * 0.5f,
                           (bbMins[1] + bbMaxs[1]) * 0.5f,
                           (bbMins[2] + bbMaxs[2]) * 0.5f};
  drawLabel(td, origin, "AABB");
  dd::aabb(td.ddContext, bbMins, bbMaxs, dd::colors::Orange);
  dd::point(td.ddContext, bbCenter, dd::colors::White, 15.0f);

  // Move along the Z for another row:
  origin[0] = -15.0f;
  origin[2] += 5.0f;

  // A big arrow pointing up:
  const ddVec3 arrowFrom = {origin[0], origin[1], origin[2]};
  const ddVec3 arrowTo = {origin[0], origin[1] + 5.0f, origin[2]};
  drawLabel(td, arrowFrom, "arrow");
  dd::arrow(td.ddContext, arrowFrom, arrowTo, dd::colors::Magenta, 1.0f);
  dd::point(td.ddContext, arrowFrom, dd::colors::White, 15.0f);
  dd::point(td.ddContext, arrowTo, dd::colors::White, 15.0f);
  origin[0] += 4.0f;

  // Plane with normal vector:
  const ddVec3 planeNormal = {0.0f, 1.0f, 0.0f};
  drawLabel(td, origin, "plane");
  dd::plane(td.ddContext, origin, planeNormal, dd::colors::Yellow,
            dd::colors::Blue, 1.5f, 1.0f);
  dd::point(td.ddContext, origin, dd::colors::White, 15.0f);
  origin[0] += 4.0f;

  // Circle on the Y plane:
  drawLabel(td, origin, "circle");
  dd::circle(td.ddContext, origin, planeNormal, dd::colors::Orange, 1.5f,
             15.0f);
  dd::point(td.ddContext, origin, dd::colors::White, 15.0f);
  origin[0] += 3.2f;

  // Tangent basis vectors:
  const ddVec3 normal = {0.0f, 1.0f, 0.0f};
  const ddVec3 tangent = {1.0f, 0.0f, 0.0f};
  const ddVec3 bitangent = {0.0f, 0.0f, 1.0f};
  origin[1] += 0.1f;
  drawLabel(td, origin, "tangent basis");
  dd::tangentBasis(td.ddContext, origin, normal, tangent, bitangent, 2.5f);
  dd::point(td.ddContext, origin, dd::colors::White, 15.0f);

  // And a set of intersecting axes:
  origin[0] += 4.0f;
  origin[1] += 1.0f;
  drawLabel(td, origin, "cross");
  dd::cross(td.ddContext, origin, 2.0f);
  dd::point(td.ddContext, origin, dd::colors::White, 15.0f);
}

static void drawFrustum(const ThreadData &td) {
  const ddVec3 color = {0.8f, 0.3f, 1.0f};
  const ddVec3 origin = {-8.0f, 0.5f, 14.0f};
  drawLabel(td, origin, "frustum + axes");

  // The frustum will depict a fake camera:
  const Matrix4 proj =
      Matrix4::perspective(degToRad(45.0f), 800.0f / 600.0f, 0.5f, 4.0f);
  const Matrix4 view =
      Matrix4::lookAt(Point3(-8.0f, 0.5f, 14.0f), Point3(-8.0f, 0.5f, -14.0f),
                      Vector3::yAxis());
  const Matrix4 clip = inverse(proj * view);
  dd::frustum(td.ddContext, toFloatPtr(clip), color);

  // A white dot at the eye position:
  dd::point(td.ddContext, origin, dd::colors::White, 15.0f);

  // A set of arrows at the camera's origin/eye:
  const Matrix4 transform = Matrix4::translation(Vector3(-8.0f, 0.5f, 14.0f)) *
                            Matrix4::rotationZ(degToRad(60.0f));
  dd::axisTriad(td.ddContext, toFloatPtr(transform), 0.3f, 2.0f);
}

static void drawText(const ThreadData &td) {
  // HUD text:
  const ddVec3 textColor = {1.0f, 1.0f, 1.0f};
  const ddVec3 textPos2D = {10.0f, 15.0f, 0.0f};
  dd::screenText(td.ddContext,
                 "[SPACE]  to toggle labels and grid on/off on/off",
                 textPos2D, textColor, 0.55f);

  const ddVec3 displayColor = {0.9f, 0.9f, 0.9f};
  const ddVec3 displayPos2D = {10.0f, 45.0f, 0.0f};

  static std::string basic_info;
  if (!atd::utility::Singleton::instance<DataDispatcher>()
           ->get_BasicDisplayInfo(basic_info)) {
    basic_info = "System OffLine";
  }
  dd::screenText(td.ddContext, basic_info.c_str(), displayPos2D, displayColor,
                 0.55f);
}

static void init4Display() {
  atd::utility::Singleton::try_register<OpenGL_Frame>();
  atd::utility::Singleton::try_register<DataDispatcher>();

  atd::utility::Singleton::instance<OpenGL_Frame>()->register_CallBack(
      &drawGrid);
  // atd::utility::Singleton::instance<OpenGL_Frame>()->register_CallBack(
  // &drawMiscObjects);
  // atd::utility::Singleton::instance<OpenGL_Frame>()->register_CallBack(
  // &drawFrustum);
  atd::utility::Singleton::instance<OpenGL_Frame>()->register_CallBack(
      &drawText);
  atd::utility::Singleton::instance<OpenGL_Frame>()->register_CallBack(
      &draw_PlanningElements);
}
