#pragma once

#include "common_frame.hpp"

static void init4Display() {
  atd::utility::Singleton::try_register<OpenGL_Frame>();

  atd::utility::Singleton::instance<OpenGL_Frame>()->register_CallBack(
      &drawGrid);
  atd::utility::Singleton::instance<OpenGL_Frame>()->register_CallBack(
      &drawMiscObjects);
  atd::utility::Singleton::instance<OpenGL_Frame>()->register_CallBack(
      &drawFrustum);
  atd::utility::Singleton::instance<OpenGL_Frame>()->register_CallBack(
      &drawText);
}

static void drawLabel(const ThreadData &td, ddVec3_In pos, const char *name) {
  if (!keys.showLabels) {
    return;
  }

  // Only draw labels inside the camera frustum.
  if (camera.isPointInsideFrustum(pos[0], pos[1], pos[2])) {
    const ddVec3 textColor = {0.8f, 0.8f, 1.0f};
    dd::projectedText(td.ddContext, name, pos, textColor,
                      toFloatPtr(camera.vpMatrix), 0, 0, WindowWidth,
                      WindowHeight, 0.5f);
  }
}

static void drawGrid(const ThreadData &td) {
  if (keys.showGrid) {
    dd::xzSquareGrid(
        td.ddContext, -50.0f, 50.0f, 0.0f, 1.0f,
        dd::colors::DarkGreen);  // Grid from -50 to +50 in both X & Z
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
  dd::screenText(
      td.ddContext,
      "Welcome to the multi-threaded Core OpenGL Debug Draw demo.\n\n"
      "[SPACE]  to toggle labels on/off\n"
      "[RETURN] to toggle grid on/off",
      textPos2D, textColor, 0.55f);
}
