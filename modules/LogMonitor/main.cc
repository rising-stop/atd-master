#include <cmath>
#include <iostream>

#include "modules/common/common_header.h"
#include "protobuf_msg/planning_log.pb.h"

using namespace atd::utility;
using namespace atd::protocol;

/* main function */
int main(int args, char** argv) {
  Proto_Messages<MONITOR_MSG> frame;
  LCM_Proxy<Proto_Messages<MONITOR_MSG>> sender_(LCM_MODE::SENDER,
                                                 "PlanningLog");

  uint32_t counter = 0;

  while (true) {
    ++counter;
    frame.Clear();
    frame.mutable_title()->set_time_stamp(counter);
    frame.mutable_title()->set_counter_no(counter);

    /* calibration test */
    auto ptr_float_1 = frame.mutable_calibrations()->add_calib_float();
    ptr_float_1->set_data(1.0f);
    ptr_float_1->set_data_init(1.0f);
    ptr_float_1->set_data_lower_bound(0.0f);
    ptr_float_1->set_data_upper_bound(2.0f);
    ptr_float_1->set_name("calib float 1");

    auto ptr_float_2 = frame.mutable_calibrations()->add_calib_float();
    ptr_float_2->set_data(2.0f);
    ptr_float_2->set_data_init(2.0f);
    ptr_float_2->set_data_lower_bound(0.0f);
    ptr_float_2->set_data_upper_bound(4.0f);
    ptr_float_2->set_name("calib float 2");

    auto ptr_int_1 = frame.mutable_calibrations()->add_calib_int();
    ptr_int_1->set_data(2);
    ptr_int_1->set_data_init(2);
    ptr_int_1->set_data_lower_bound(0);
    ptr_int_1->set_data_upper_bound(4);
    ptr_int_1->set_name("calib int 1");

    auto ptr_disp_float = frame.mutable_display_element()->add_float_vars();
    ptr_disp_float->set_name("disp float 1");
    ptr_disp_float->set_data(sin(static_cast<float>(counter % 125) / 3.0f));

    auto ptr_disp_int = frame.mutable_display_element()->add_int_vars();
    ptr_disp_int->set_name("disp int 1");
    ptr_disp_int->set_data(counter % 16);

    auto single_log_frame = frame.mutable_log()->add_content();
    single_log_frame->add_str_msg("test str");
    auto int_var = single_log_frame->add_int_vars();
    int_var->set_name("test log int");
    int_var->set_data(counter % 16);

    auto single_log_frame_2 = frame.mutable_log()->add_content();
    single_log_frame_2->add_str_msg("test str");
    auto int_var_2 = single_log_frame_2->add_int_vars();
    int_var_2->set_name("test log int 2");
    int_var_2->set_data(counter % 26);

    sender_.publish(frame);

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  exit(EXIT_SUCCESS);
}
