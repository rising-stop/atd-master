#include "modules/common/common_header.h"
#include "modules/neo_adp/opengl_implement/gl_implement.h"

using namespace atd::utility;
using namespace atd::protocol;

void init() {
  atd::utility::Singleton::try_register<Runtime_Calculator<>>();
  atd::utility::Singleton::try_register<Runtime_Counter>();
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

/* main function */
int main(int args, char** argv) {
  init();
  atd::utility::Singleton::instance<OpenGL_Frame>()->spin();
  exit(EXIT_SUCCESS);
}
