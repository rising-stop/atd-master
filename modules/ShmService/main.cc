#include <signal.h>

#include <iostream>
#include <thread>

#include "modules/common/common_support/observer/ObservingLogging.h"
#include "modules/common/common_support/timer/timer.h"
#include "modules/common/utility/ipc/semaphore_mutex.h"
#include "modules/common/utility/ipc/shared_memory.h"
#include "modules/common/utility/ipc/shm_protocol.h"

void CtlC_Handler(int sig) {
  GET_SHMDISPATCHER->release_all_shm();
  GET_SEMDISPATCHER->release_all_sem();
  exit(EXIT_SUCCESS);
}

int main(int argc, char* argv[]) {
  signal(SIGINT, CtlC_Handler);
  GET_SHMDISPATCHER->register_shm(SHM_DIRECTOR_SEED, SHM_DIRECTOR_SIZE);

  atd::common::utility::SHM_DIRECTOR dir;
  dir.flag_init_ = true;
  dir.dispatcher_id = GET_SHMDISPATCHER->get_ShmInfo(SHM_DIRECTOR_SEED).first;
  dir.dispatcher_size = SHM_DIRECTOR_SIZE;

  atd::common::utility::SharedMemory test_mem(
      GET_SHMDISPATCHER->get_ShmInfo(SHM_DIRECTOR_SEED));
  test_mem.send_Msg(dir, SHM_DIRECTOR_SIZE);

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