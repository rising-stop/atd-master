#pragma once

#include <cmath>
#include <functional>

#include "modules/common/common_header.h"
#include "modules/neo_adp/data_service/data_seg4lcm_protocol.h"
#include "modules/neo_adp/display_elements/common_frame.hpp"
#include "protobuf_msg/planning_log.pb.h"

class GL_Implement {
 public:
  static void init() {
    atd::utility::Singleton::try_register<DataRepository>();
    atd::utility::Singleton::try_register<Runtime_Calculator<>>();
    atd::utility::Singleton::try_register<Runtime_Counter>();
    atd::utility::Singleton::try_register<OpenGL_Frame>();

    atd::utility::Singleton::instance<DataRepository>()
        ->try_register<RealTimeDataDispatcher>(Data_Seg_Name_LCMProtocol);

    atd::utility::Singleton::instance<OpenGL_Frame>()->register_CallBack(
        &GL_Implement::drawGrid);
    atd::utility::Singleton::instance<OpenGL_Frame>()->register_CallBack(
        &GL_Implement::drawText);
    atd::utility::Singleton::instance<OpenGL_Frame>()->register_CallBack(
        &GL_Implement::draw_PlanningElements);
  }

  /**
   * @brief draw item label in gl windows
   */
  static void drawLabel(const ThreadData &td, ddVec3_In pos, const char *name) {
    if (!keys.showLabels) {
      return;
    }
    // Only draw labels inside the camera frustum.
    if (camera.isPointInsideFrustum(pos[0], pos[1], pos[2])) {
      const ddVec3 textColor = {0.8f, 0.8f, 1.0f};
      dd::projectedText(td.ddContext, name, pos, textColor,
                        toFloatPtr(camera.vpMatrix), 0, 0, WindowWidth,
                        WindowHeight, 0.4f);
      // dd::screenText(td.ddContext, name, pos, textColor, 0.55f);
    }
  }

  /**
   * @brief draw item grid in gl windows
   */
  static void drawGrid(const ThreadData &td) {
    if (keys.showGrid) {
      dd::xzSquareGrid(td.ddContext, -40.0f, 40.0f, -20.0f, 100.0f, -1.0f, 5.0f,
                       dd::colors::DarkGray);
    }
  }

  /**
   * @brief main planning elements display process
   */
  static void draw_PlanningElements(const ThreadData &td) {
    static std::shared_ptr<atd::protocol::OPENGL_ELEMENT> ptr_disp_msg;
    auto ptr_gl_element = PROTOCOL_POINTER->get_LatestGLElement();
    if (ptr_gl_element) {
      ptr_disp_msg = ptr_gl_element;
    }

    ddVec3 origin = {0.0f, 0.0f, 0.0f};
    drawLabel(td, origin, "ego vehicle");
    dd::box(td.ddContext, origin, dd::colors::BlueViolet, 1.8f, 1.5f, 4.68f);
    dd::point(td.ddContext, origin, dd::colors::White, 5.0f);

    auto box_set = ptr_disp_msg->box_set();
    for (auto single_box : box_set) {
      ddVec3 box_origin = {single_box.origin().x(), single_box.origin().y(),
                           single_box.origin().z()};
      ddVec3 label_origin = {single_box.origin().y(), single_box.origin().z(),
                             single_box.origin().x()};

      std::string display_content;
      display_content.append(single_box.name());
      display_content.push_back('\n');
      display_content.append(single_box.discription());
      if (single_box.name() == "CIPV") {
        drawLabel(td, label_origin, display_content.c_str());
        draw_OrientedRect(td, box_origin, single_box.width(),
                          single_box.height(), single_box.deepth(),
                          single_box.heading(), dd::colors::Green);
        // dd::point(td.ddContext, box_origin, dd::colors::White, 3.0f);
      } else if (single_box.name() == "TOS") {
        drawLabel(td, label_origin, display_content.c_str());
        draw_OrientedRect(td, box_origin, single_box.width(),
                          single_box.height(), single_box.deepth(),
                          single_box.heading(), dd::colors::Yellow);
        // dd::point(td.ddContext, box_origin, dd::colors::White, 3.0f);
      } else if (single_box.name() == "Selected") {
        drawLabel(td, label_origin, display_content.c_str());
        draw_OrientedRect(td, box_origin, single_box.width(),
                          single_box.height(), single_box.deepth(),
                          single_box.heading(), dd::colors::Red);
        // dd::point(td.ddContext, box_origin, dd::colors::White, 3.0f);
      } else if (single_box.name() == "CCIV") {
        drawLabel(td, label_origin, display_content.c_str());
        draw_OrientedRect(td, box_origin, single_box.width(),
                          single_box.height(), single_box.deepth(),
                          single_box.heading(), dd::colors::Orange);
        // dd::point(td.ddContext, box_origin, dd::colors::White, 3.0f);
      } else {
        drawLabel(td, label_origin, display_content.c_str());
        // std::cout << display_content << std::endl;
        draw_OrientedRect(td, box_origin, single_box.width(),
                          single_box.height(), single_box.deepth(),
                          single_box.heading(), dd::colors::AliceBlue);
        // dd::point(td.ddContext, box_origin, dd::colors::White, 3.0f);
      } 
    }

    auto line_set = ptr_disp_msg->line_set();
    for (auto single_line : line_set) {
      std::string display_content;
      display_content.append(single_line.name());
      display_content.push_back('\n');
      display_content.append(single_line.discription());
      auto point_size = single_line.sample_points().size();
      if (point_size > 2) {
        if (single_line.name() == "Planning_Line") {
          draw_Line(
              td,
              [&](int index) { return single_line.sample_points(index).x(); },
              [&](int index) { return single_line.sample_points(index).y(); },
              [&](int index) { return single_line.sample_points(index).z(); },
              point_size, display_content, dd::colors::Yellow);
        } else if (single_line.name() == "RawRate_Line") {
          draw_Line(
              td,
              [&](int index) { return single_line.sample_points(index).x(); },
              [&](int index) { return single_line.sample_points(index).y(); },
              [&](int index) { return single_line.sample_points(index).z(); },
              point_size, display_content, dd::colors::DarkGreen);
        } else {
          draw_Line(
              td,
              [&](int index) { return single_line.sample_points(index).x(); },
              [&](int index) { return single_line.sample_points(index).y(); },
              [&](int index) { return single_line.sample_points(index).z(); },
              point_size, display_content, dd::colors::LawnGreen);
        }
      }
    }

    auto ploy_set = ptr_disp_msg->ploy_set();
    for (auto single_ploy : ploy_set) {
      std::string display_content;
      display_content.append(single_ploy.name());
      display_content.push_back('\n');
      display_content.append(single_ploy.discription());
      int point_size = static_cast<int>(single_ploy.upper_bound() -
                                        single_ploy.lower_bound());
      if (point_size > 2) {
        float ploy_step =
            (single_ploy.upper_bound() - single_ploy.lower_bound()) /
            point_size;
        if (single_ploy.name() == "PlannedTraj") {
          draw_Line(
              td,
              [&](int index) {
                return index * ploy_step + single_ploy.lower_bound();
              },
              [&](int index) {
                float local_x = index * ploy_step + single_ploy.lower_bound();
                atd::utility::Polynomial tmp_ploy;
                std::vector<double> param_list;
                for (auto item : single_ploy.coeffients()) {
                  param_list.push_back(item);
                }

                tmp_ploy.set_PolynomialParameters(param_list);
                return tmp_ploy.get_PolynomialValue(local_x);
              },
              [&](int index) { return 0.0f; }, point_size, display_content,
              dd::colors::Yellow);
        } else if (single_ploy.name() == "PredictedTraj") {
          draw_Line(
              td,
              [&](int index) {
                return index * ploy_step + single_ploy.lower_bound();
              },
              [&](int index) {
                float local_x = index * ploy_step + single_ploy.lower_bound();
                float base = 1.0f;
                int ploy_size = single_ploy.coeffients_size();
                float res = single_ploy.coeffients(ploy_size - 1);
                for (int index4loop = ploy_size - 2; index4loop >= 0;
                     --index4loop) {
                  base *= local_x;
                  res += base * single_ploy.coeffients(index4loop);
                }
                return res;
              },
              [&](int index) { return 0.0f; }, point_size, display_content,
              dd::colors::LightPink);
        } else {
          draw_Line(
              td,
              [&](int index) {
                return index * ploy_step + single_ploy.lower_bound();
              },
              [&](int index) {
                float local_x = index * ploy_step + single_ploy.lower_bound();
                atd::utility::Polynomial tmp_ploy;
                std::vector<double> param_list;
                for (auto item : single_ploy.coeffients()) {
                  param_list.push_back(item);
                }

                tmp_ploy.set_PolynomialParameters(param_list);
                return tmp_ploy.get_PolynomialValue(local_x);
              },
              [&](int index) { return 0.0f; }, point_size, display_content,
              dd::colors::White);
        }
      }
    }
  }

  /**
   * @brief draw text info in the window
   */
  static void drawText(const ThreadData &td) {
    // HUD text:
    const ddVec3 textColor = {1.0f, 1.0f, 1.0f};
    const ddVec3 textPos2D = {10.0f, 15.0f, 0.0f};
    dd::screenText(td.ddContext, "[SPACE]  to toggle labels and grid on/off",
                   textPos2D, textColor, 0.55f);
    const ddVec3 displayColor = {0.9f, 0.9f, 0.9f};
    const ddVec3 displayPos2D = {10.0f, 45.0f, 0.0f};

    std::string display_text;
    auto ptr_disp_msg = PROTOCOL_POINTER->get_LatestDisplayContent();
    if (!ptr_disp_msg) {
      display_text = "System OffLine";
    } else {
      // "AutoDriving\nSpeed(kph): 11.0\nSteer(deg): 11.0"
      std::string auto_info{"Offline"};
      std::string display_speed{"Speed(kph): "};
      std::string display_steer{"Steer(deg): "};
      std::string display_des_acc{"DeAcc(ms2): "};
      std::string display_act_acc{"AtAcc(ms2): "};
      std::string display_DTC{"DTC code: "};
      std::string display_setting_spd{"DeSpd(kph): "};
      for (auto single_content : ptr_disp_msg->int_vars()) {
        auto content_name = single_content.name();
        if (content_name == DISPLAY_FLAG_AUTO) {
          int mode_code = single_content.data();
          if (mode_code == 5) {
            auto_info = "Acc";
          } else if (mode_code == 0) {
            auto_info = "Manual";
          } else if (mode_code == 2) {
            auto_info = "Auto";
          } else {
            auto_info = "Abnormal";
          }
        }
      }
      for (auto single_content : ptr_disp_msg->float_vars()) {
        auto content_name = single_content.name();
        if (content_name == DISPLAY_SPEED) {
          float spd = single_content.data();
          display_speed.append(std::to_string(spd * 3.6));
        } else if (content_name == DISPLAY_STEER) {
          display_steer.append(std::to_string(single_content.data()));
        } else if (content_name == DISPLAY_DESIRED_ACC) {
          display_des_acc.append(std::to_string(single_content.data()));
        } else if (content_name == DISPLAY_ACTUAL_ACC) {
          display_act_acc.append(std::to_string(single_content.data()));
        } else if (content_name == DISPLAY_DTC) {
          display_DTC.append(std::to_string(single_content.data()));
        } else if (content_name == DISPLAY_SETTING_SPEED) {
          display_setting_spd.append(std::to_string(single_content.data()));
        }
      }
      display_text.clear();
      display_text.append(auto_info);
      display_text.append("\n");
      display_text.append(display_setting_spd);
      display_text.append("\n");
      display_text.append(display_speed);
      display_text.append("\n");
      display_text.append(display_steer);
      display_text.append("\n");
      display_text.append(display_des_acc);
      display_text.append("\n");
      display_text.append(display_act_acc);
      display_text.append("\n");
      display_text.append(display_DTC);
    }

    dd::screenText(td.ddContext, display_text.c_str(), displayPos2D,
                   displayColor, 0.55f);
  }

  /**
   * @brief debug draw examples
   */
  static void drawFrustum(const ThreadData &td) {
    const ddVec3 color = {0.8f, 0.3f, 1.0f};
    const ddVec3 origin = {-8.0f, 0.5f, 14.0f};
    drawLabel(td, origin, "frustum + axes");

    // The fsrustum will depict a fake camera:
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
    const Matrix4 transform =
        Matrix4::translation(Vector3(-8.0f, 0.5f, 14.0f)) *
        Matrix4::rotationZ(degToRad(60.0f));
    dd::axisTriad(td.ddContext, toFloatPtr(transform), 0.3f, 2.0f);
  }

 private:
  /**
   * @brief box data structure converter, convert standard heading included box
   * data to 8 conner points
   */
  static void draw_OrientedRect(const ThreadData &ctx, ddVec3 origin,
                                const float width, const float height,
                                const float deepth, const float heading,
                                const ddVec3 &color) {
    float heading_angle = 3.14159265f / 2.0f + heading;
    float box_width = width / 2.0f;
    float box_height = height / 2.0f;
    ddVec3 corner_points[7];
    corner_points[0][0] = (origin[1] + box_width * sinf(heading_angle) +
                           box_height * cosf(heading_angle));
    corner_points[0][1] = origin[2];
    corner_points[0][2] = origin[0] + box_width * cosf(heading_angle) -
                          box_height * sinf(heading_angle);
    corner_points[1][0] = (origin[1] - box_width * sinf(heading_angle) +
                           box_height * cosf(heading_angle));
    corner_points[1][1] = origin[2];
    corner_points[1][2] = origin[0] - box_width * cosf(heading_angle) -
                          box_height * sinf(heading_angle);
    dd::line(ctx.ddContext, corner_points[0], corner_points[1], color);

    corner_points[2][0] = (origin[1] - box_width * sinf(heading_angle) -
                           box_height * cosf(heading_angle));
    corner_points[2][1] = origin[2];
    corner_points[2][2] = origin[0] - box_width * cosf(heading_angle) +
                          box_height * sinf(heading_angle);
    dd::line(ctx.ddContext, corner_points[1], corner_points[2], color);

    corner_points[3][0] = (origin[1] + box_width * sinf(heading_angle) -
                           box_height * cosf(heading_angle));
    corner_points[3][1] = origin[2];
    corner_points[3][2] = origin[0] + box_width * cosf(heading_angle) +
                          box_height * sinf(heading_angle);
    dd::line(ctx.ddContext, corner_points[2], corner_points[3], color);
    dd::line(ctx.ddContext, corner_points[3], corner_points[0], color);

    corner_points[4][0] = (corner_points[2][0] + corner_points[3][0]) * 0.5f;
    corner_points[4][1] = corner_points[0][1];
    corner_points[4][2] = (corner_points[2][2] + corner_points[3][2]) * 0.5f;

    corner_points[5][0] = (corner_points[2][0] + corner_points[1][0]) * 0.5f;
    corner_points[5][1] = corner_points[1][1];
    corner_points[5][2] = (corner_points[2][2] + corner_points[1][2]) * 0.5f;
    dd::line(ctx.ddContext, corner_points[4], corner_points[5], color);

    corner_points[6][0] = (corner_points[0][0] + corner_points[3][0]) * 0.5f;
    corner_points[6][1] = corner_points[2][1];
    corner_points[6][2] = (corner_points[0][2] + corner_points[3][2]) * 0.5f;
    dd::line(ctx.ddContext, corner_points[5], corner_points[6], color);
    dd::line(ctx.ddContext, corner_points[6], corner_points[4], color);
  }

  static void draw_Line(const ThreadData &handler,
                        std::function<float(int)> func_x,
                        std::function<float(int)> func_y,
                        std::function<float(int)> func_z, int size,
                        const std::string &info, const ddVec3 &color) {
    if (!size) {
      return;
    }
    for (uint32_t index = 0; index < (size - 1); index++) {
      ddVec3 seg_start{func_y(index), func_z(index), func_x(index)};
      ddVec3 seg_end{func_y(index + 1), func_z(index + 1), func_x(index + 1)};
      dd::line(handler.ddContext, seg_start, seg_end, color);
    }
    ddVec3 center{func_y(size / 2), func_z(size / 2), func_x(size / 2)};
    // drawLabel(handler, center, info.c_str());

    // dd::point(td.ddContext, point_ori, dd::colors::White, 5.0f);
  }
};
