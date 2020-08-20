// #include "modules/neo_adp/display_elements/display_implements.hpp"
// #include "../protobuf_msg/debug_monitor_protocol.pb.h"
#include "modules/common/common_header.h"

using namespace atd::utility;
using namespace atd::protocol;

void init() {
  atd::utility::Singleton::try_register<Runtime_Counter>();
  atd::utility::Singleton::try_register<
      Runtime_Calculator<std::chrono::milliseconds>>();
  atd::utility::Singleton::try_register<DebugLogging>();
}

/* main function */
int main(int args, char** argv) {
  init();
  //   init4Display();
  //   atd::utility::Singleton::instance<OpenGL_Frame>()->spin();

  // Proto_Messages<atd::protocol::LOG_CONTENT> pub_msg;

  // LCM_Proxy<Proto_Messages<atd::protocol::LOG_CONTENT>> msg_sender(
  //     LCM_MODE::SENDER, "test_msg");

  // CLASS_SINGLETON::try_register<Runtime_Calculator<>>();
  // CLASS_SINGLETON::instance<Runtime_Calculator<>>()->set_BeginStick(
  //     "test_stick");

  // uint64_t counter = 0;
  double arg1 = 0.1;
  double arg2 = 0.2;
  double arg3 = 0.3;
  double arg4 = 0.4;

  while (true) {
    atd::utility::Singleton::instance<DebugLogging>()->reset_Frame();
    LCM_LOG(atd::utility::SECURITY_INFO::INFO)
        .LogInfo("Test Str 1")
        .LogVar("Var1", arg1)
        .LogVar("Var2", arg2);
    atd::utility::Singleton::instance<DebugLogging>()->publish_Frame();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  if (args < 2) {
    std::cout << "log file name required" << std::endl;
    return EXIT_SUCCESS;
  }
  char* log_name = argv[1];
  std::string plan_log_name{"log_"};
  plan_log_name.append(log_name);

  PlanningLog log_parser(PlanningLog::FILE_MODE::WRITE, plan_log_name.c_str(),
                         "");

  try {
    log_parser.Log_Parse(log_name);
  } catch (const CommonException& e) {
    std::cout << e.what() << std::endl;
    return EXIT_SUCCESS;
  }

  // while (true) {
  //   counter++;
  //   CLASS_SINGLETON::instance<Runtime_Calculator<>>()->wait_and_set_EndStick(
  //       "test_stick");
  //   uint64_t mile_stone = CLASS_SINGLETON::instance<Runtime_Calculator<>>()
  //                             ->get_TimeGap("test_stick")
  //                             .first;

  //   pub_msg.Clear();
  //   pub_msg.mutable_title()->set_counter_no(counter);
  //   pub_msg.mutable_title()->set_time_stamp(mile_stone);
  //   auto ptr_neo_content = pub_msg.add_content();
  //   ptr_neo_content->set_level(atd::protocol::SECURITY_INFO::INFO);
  //   ptr_neo_content->add_str_msg("TestLine 1.");
  //   auto ptr_neo_var = ptr_neo_content->add_variables();
  //   ptr_neo_var->set_name("TestVar");
  //   ptr_neo_var->set_data("0.02");

  //   // msg_sender.publish(pub_msg);

  //   std::this_thread::sleep_for(std::chrono::milliseconds(100));
  // }

  exit(EXIT_SUCCESS);
}
