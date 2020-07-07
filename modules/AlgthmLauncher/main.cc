#include <iostream>
#include <thread>

#include "modules/common/common_support/observer/ObservingLogging.h"
#include "modules/common/common_support/timer/timer.h"
#include "modules/common/utility/ipc/lcm_dispatcher/lcm_dispatcher.h"
#include "protobuf_msg/shm_dispatcher.pb.h"

using namespace atd::utility;

int main(int argc, char* argv[]) {
  LCM_Proxy msg_sender{LCM_Proxy::SENDER, "TEST_SENDER"};
  LCM_Proxy msg_reader{LCM_Proxy::READER, "TEST_READER"};

  atd::common::msg::shminfo test_msg;
  test_msg.set_sem_id(100);
  ProtoLite_Messages proto_msg{test_msg};
  msg_sender.publish(proto_msg);

  while (1) {
    /* Write it into shared memory */
    TIMER->set_EndStick("counter");
    TIMER->set_BeginStick("counter");

    std::this_thread::sleep_for(std::chrono::milliseconds(3000));

    std::cout << "last step elapse "
              << static_cast<double>(TIMER->get_TimeGap("counter")) / 1000
              << "ms" << std::endl;
  }

  exit(EXIT_SUCCESS);
}