#include "modules/neo_adp/display_elements/display_implements.hpp"

/* main function */
int main(int args, char** argv) {
  atd::utility::Singleton::try_register<OpenGL_Frame>();
  
  atd::utility::Singleton::instance<OpenGL_Frame>()->register_CallBack(
      &drawGrid);
  atd::utility::Singleton::instance<OpenGL_Frame>()->register_CallBack(
      &drawMiscObjects);
  atd::utility::Singleton::instance<OpenGL_Frame>()->register_CallBack(
      &drawFrustum);
  atd::utility::Singleton::instance<OpenGL_Frame>()->register_CallBack(
      &drawText);

  atd::utility::Singleton::instance<OpenGL_Frame>()->spin();

  exit(EXIT_SUCCESS);
}
