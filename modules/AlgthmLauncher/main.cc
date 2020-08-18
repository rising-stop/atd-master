#include <iostream>
#include <thread>

#include "modules/common/utility/ipc/lcm_dispatcher/lcm_dispatcher.h"

using namespace atd::utility;

int main(int argc, char* argv[]) {
  while (1) {
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));

    std::cout << "last step elapse "
              << ""
              << "ms" << std::endl;
  }

  exit(EXIT_SUCCESS);
}