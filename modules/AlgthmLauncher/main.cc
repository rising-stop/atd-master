#include <iostream>
#include <thread>

#include "modules/common/common_support/observer/ObservingLogging.h"
#include "modules/common/common_support/timer/timer.h"
#include "modules/common/utility/ipc/semaphore_mutex.h"
#include "modules/common/utility/ipc/shared_memory.h"
#include "modules/common/utility/ipc/shm_protocol.h"

int main(int argc, char* argv[]) {
  std::pair<int, size_t> dispatcher_info;
  while (1) {
    try {
      dispatcher_info = atd::common::utility::SharedMemory::try_get_ShmID(
          SHM_DIRECTOR_SEED, SHM_DIRECTOR_SIZE);
      break;
    } catch (const atd::common::utility::ShmException& e) {
      std::this_thread::sleep_for(std::chrono::milliseconds(1000));
      std::cerr << e.what() << std::endl;
    }
  }

  atd::common::utility::SharedMemory shm_distributor(dispatcher_info);

  atd::common::utility::SHM_DIRECTOR distributor;

  shm_distributor.read_Msg(distributor, sizeof(distributor));

  std::cout << "msg read from dispatcher: " << std::endl;
  std::cout << "  content:" << NUMINFO(distributor.flag_init_)
            << NUMINFO(distributor.dispatcher_id)
            << NUMINFO(distributor.dispatcher_size) << std::endl;

  TIMER->set_BeginStick("counter");

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