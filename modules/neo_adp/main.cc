// #include "modules/neo_adp/display_elements/display_implements.hpp"
#include "../protobuf_msg/debug_monitor_protocol.pb.h"
#include "modules/common/common_header.h"

using namespace atd::utility;
using namespace atd::protocol;

/* main function */
int main(int args, char** argv) {
  //   init4Display();
  //   atd::utility::Singleton::instance<OpenGL_Frame>()->spin();
  ProtoLite_Messages<Monitor_Message> send_msg;
  static uint64_t counter = 0;
  ProtoLite_Messages<Monitor_Message> receive_msg;

  LCM_Proxy sender(LCM_Proxy::SENDER, "Test_Channel");
  LCM_Proxy receiver(LCM_Proxy::READER, "Test_Channel");

  while (true) {
    send_msg.mutable_msg_header()->set_msg_counter(counter++);
    sender.publish(send_msg);
    if (receiver.subscribe(receive_msg)) {
      std::cout << "receive res = " << receive_msg.msg_header().msg_counter()
                << std::endl;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  exit(EXIT_SUCCESS);
}
