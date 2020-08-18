#include <signal.h>

#include <iostream>
#include <thread>

#include "modules/common/utility/ipc/semaphore_mutex.h"
#include "modules/common/utility/ipc/shared_memory.h"
#include "modules/common/utility/ipc/shm_protocol.h"

using namespace atd::utility;

void CtlC_Handler(int sig) {
  Singleton::instance<ShmDispatcher>()->release_all_shm();
  Singleton::instance<SemDispatcher>()->release_all_sem();
  exit(EXIT_SUCCESS);
}

int main(int argc, char* argv[]) {
  signal(SIGINT, CtlC_Handler);
  Singleton::instance<ShmDispatcher>()->register_shm(SHM_DIRECTOR_SEED,
                                                     SHM_DIRECTOR_SIZE);

  atd::common::utility::SHM_DIRECTOR dir;
  dir.flag_init_ = true;
  dir.dispatcher_id = Singleton::instance<ShmDispatcher>()
                          ->get_ShmInfo(SHM_DIRECTOR_SEED)
                          .first;
  dir.dispatcher_size = SHM_DIRECTOR_SIZE;

  atd::common::utility::SharedMemory test_mem(
      Singleton::instance<ShmDispatcher>()->get_ShmInfo(SHM_DIRECTOR_SEED));
  test_mem.write_Msg(dir, SHM_DIRECTOR_SIZE);
  ` while (1) {
    /* Write it into shared memory */

    std::this_thread::sleep_for(std::chrono::milliseconds(3000));

    std::cout << "last step elapse "
              << ""
              << "ms" << std::endl;
  }

  exit(EXIT_SUCCESS);
}