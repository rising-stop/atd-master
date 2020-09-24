#include "gl_implement.h"

void GL_Implement::drawLabel(co nst ThreadData &td, ddVec3_In pos,
                             const char *name) {
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

void GL_Implement::drawGrid(const ThreadData &td) {
  if (keys.showGrid) {
    dd::xzSquareGrid(td.ddContext, -40.0f, 40.0f, -20.0f, 100.0f, -1.0f, 5.0f,
                     dd::colors::DarkGray);
  }
}

void GL_Implement::convert_BoxData(ddVec3 *corner_points, ddVec3 origin,
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

void GL_Implement::draw_PlanningElements(const ThreadData &td) {
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
      dd::box(td.ddContext, corner_points, dd::colors::Yellow);
      dd::point(td.ddContext, box_origin, dd::colors::White, 3.0f);
    } else if (single_box.discription() == "Selected") {
      drawLabel(td, box_origin, "Selected");
      dd::box(td.ddContext, corner_points, dd::colors::Red);
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
      if (single_line.discription() == "Planning_Line") {
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
      } else if (single_line.discription() == "RawRate_Line") {
        drawLabel(td, point_ori, single_line.discription().c_str());
        dd::point(td.ddContext, point_ori, dd::colors::White, 5.0f);
        for (uint32_t index = 0; index < (point_size - 1); index++) {
          ddVec3 seg_start{single_line.sample_points(index).y(),
                           single_line.sample_points(index).z(),
                           single_line.sample_points(index).x()};
          ddVec3 seg_end{single_line.sample_points(index + 1).y(),
                         single_line.sample_points(index + 1).z(),
                         single_line.sample_points(index + 1).x()};
          dd::line(td.ddContext, seg_start, seg_end, dd::colors::DeepPink);
        }
      } else {
        drawLabel(td, point_ori, single_line.discription().c_str());
        dd::point(td.ddContext, point_ori, dd::colors::White, 5.0f);
        for (uint32_t index = 0; index < (point_size - 1); index++) {
          ddVec3 seg_start{single_line.sample_points(index).y(),
                           single_line.sample_points(index).z(),
                           single_line.sample_points(index).x()};
          ddVec3 seg_end{single_line.sample_points(index + 1).y(),
                         single_line.sample_points(index + 1).z(),
                         single_line.sample_points(index + 1).x()};
          dd::line(td.ddContext, seg_start, seg_end, dd::colors::White);
        }
      }
    }
  }
}

void GL_Implement::drawText(const ThreadData &td) {
  // HUD text:
  const ddVec3 textColor = {1.0f, 1.0f, 1.0f};
  const ddVec3 textPos2D = {10.0f, 15.0f, 0.0f};
  dd::screenText(td.ddContext, "[SPACE]  to toggle labels and grid on/off",
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

void GL_Implement::drawFrustum(const ThreadData &td) {
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