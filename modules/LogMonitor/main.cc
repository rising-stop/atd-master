#include <cmath>
#include <iostream>

#include "modules/common/common_header.h"
#include "protobuf_msg/planning_log.pb.h"

using namespace Themis::utility;
using namespace Themis::protocol;

/* main function */
int main(int args, char** argv) {
  atd::utility::Singleton::try_register<atd::utility::Runtime_Calculator<>>();
  atd::utility::Singleton::try_register<atd::utility::Runtime_Counter>();
  atd::utility::Singleton::try_register<atd::utility::DebugLogging>();

  int counter = 0;

  while (true) {
    atd::utility::Singleton::instance<atd::utility::DebugLogging>()
        ->reset_Frame();

    float signal_float = sin(static_cast<float>(counter) / 10.0f);
    int signal_int = static_cast<int>(counter);
    uint32_t signal_uint = signal_int > 0 ? signal_int : -signal_int;
    std::cout << "signal_float = " << signal_float << std::endl;
    std::cout << "signal_int = " << signal_int << std::endl;
    std::cout << "signal_uint = " << signal_uint << std::endl;

    LCM_LOG_INFO.LogInfo("LogTest: Log title")
        .LogFloat("LogFloat_1", signal_float)
        .LogInt("LogInt_1", signal_int)
        .LogUInt("LogUint_1", signal_uint);

    REGISTER_FLOAT_AS_CALIBRATION(calib_float, 2.0f, 0.0f, 1.0f);
    REGISTER_INT_AS_CALIBRATION(calib_int, 10, -10, 1);
    REGISTER_UINT_AS_CALIBRATION(calib_uint, 10, 0, 1);

    auto disp_float = GET_FLOAT_DISPLAY_POINTER;
    disp_float->set_name("float_display_1");
    disp_float->set_data(signal_float);

    auto disp_int = GET_INT_DISPLAY_POINTER;
    disp_int->set_name("int_display_1");
    disp_int->set_data(signal_int);

    auto disp_uint = GET_UINT_DISPLAY_POINTER;
    disp_uint->set_name("uint_display_1");
    disp_uint->set_data(signal_uint);

    if (++counter > 47) {
      counter = 0;
    }

    atd::utility::Singleton::instance<atd::utility::DebugLogging>()
        ->publish_Frame();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  exit(EXIT_SUCCESS);
}
