#include "../protobuf_msg/debug_monitor_protocol.pb.h"
#include "modules/common/common_header.h"
#include "modules/neo_adp/display_elements/display_implements.hpp"

using namespace atd::utility;
using namespace atd::protocol;

/* main function */
int main(int args, char** argv) {
  init4Display();
  atd::utility::Singleton::instance<OpenGL_Frame>()->spin();

  while (true) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  exit(EXIT_SUCCESS);
}
