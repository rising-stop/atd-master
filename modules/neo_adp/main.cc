#include "modules/common/common_header.h"
#include "modules/neo_adp/display_elements/display_implements.hpp"

using namespace atd::utility;
using namespace atd::protocol;

/* main function */
int main(int args, char** argv) {
  init4Display();
  atd::utility::Singleton::instance<OpenGL_Frame>()->spin();
  exit(EXIT_SUCCESS);
}
