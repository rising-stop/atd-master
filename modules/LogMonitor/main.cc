// #include "modules/neo_adp/display_elements/display_implements.hpp"
// #include "../protobuf_msg/debug_monitor_protocol.pb.h"
#include "modules/common/common_header.h"

using namespace atd::utility;
using namespace atd::protocol;

#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */

/* main function */
int main(int args, char** argv) {
  Proto_Messages<LOG_CONTENT> frame;
  LCM_Proxy<Proto_Messages<LOG_CONTENT>> receiver_(LCM_MODE::READER,
                                                   "PlanningLog");

  while (true) {
    if (!receiver_.subscribe(frame)) {
      std::cout << "Fail Receiving Channel:  PlanningLog" << std::endl;
      std::this_thread::sleep_for(std::chrono::milliseconds(1000));
      continue;
    }

    std::cout << "########### "
              << "FRAME NO. " << frame.title().counter_no() << ", TIME STAMP "
              << frame.title().time_stamp() << " ###########"
              << "\n";
    for (uint32_t index = 0; index < frame.content_size(); index++) {
      switch (frame.content(index).level()) {
        case atd::protocol::SECURITY_INFO::INFO:
          std::cout << "> [INFO] > ";
          break;
        case atd::protocol::SECURITY_INFO::WARNING:
          std::cout << YELLOW << "> [WARNING] > " << RESET;
          break;
        case atd::protocol::SECURITY_INFO::ERROR:
          std::cout << RED << "> [ERROR] > " << RESET;
          break;
        default:
          std::cout << "> [UNKNOW] > ";
          break;
      }
      std::cout << frame.content(index).file_name() << " no. "
                << frame.content(index).line_no() << ":"
                << "\n";
      for (uint32_t str_index = 0;
           str_index < frame.content(index).str_msg_size(); str_index++) {
        std::cout << "   > " << frame.content(index).str_msg(str_index) << "\n";
      }
      for (uint32_t var_index = 0;
           var_index < frame.content(index).variables_size(); var_index++) {
        std::cout << "   > " << frame.content(index).variables(var_index).name()
                  << " = " << frame.content(index).variables(var_index).data()
                  << "\n";
      }
    }
    std::cout.flush();

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  exit(EXIT_SUCCESS);
}
