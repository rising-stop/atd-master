#include <signal.h>

#include <iostream>
#include <thread>

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
  std::cout << "start" << std::endl;
  signal(SIGINT, CtlC_Handler);
  std::cout << "signal set" << std::endl;
  GET_SHMDISPATCHER->register_shm(SHM_DISTRIBUTOR_SEED, SHM_DISTRIBUTOR_SIZE);
  std::cout << "shm registered" << std::endl;
  GET_SEMDISPATCHER->register_sem(SHM_DISTRIBUTOR_SEED, SEM_MUTEX_SEMNUM);
  std::cout << "sem registered" << std::endl;

  atd::common::utility::SharedMemory shm_distributor(
      GET_SHMDISPATCHER->get_ShmInfo(SHM_DISTRIBUTOR_SEED));
  std::cout << "shm created" << std::endl;

  atd::common::utility::SHM_DISTRIBUTOR distributor;
  auto ptr_comp = distributor.add_ID("unknow_Process");
  auto ptr_module = ptr_comp->add_ID("receiver_1");
  ptr_module->sem_.is_available_ = true;
  ptr_module->sem_.id_ =
      GET_SEMDISPATCHER->get_SemInfo(SHM_DISTRIBUTOR_SEED).first;
  ptr_module->sem_.num_ =
      GET_SEMDISPATCHER->get_SemInfo(SHM_DISTRIBUTOR_SEED).second;
  ptr_module->shm_.is_available_ = true;
  ptr_module->shm_.id_ =
      GET_SHMDISPATCHER->get_ShmInfo(SHM_DISTRIBUTOR_SEED).first;
  ptr_module->shm_.size_ =
      GET_SHMDISPATCHER->get_ShmInfo(SHM_DISTRIBUTOR_SEED).second;
  std::cout << "distributor set" << std::endl;

  shm_distributor.send_Msg(distributor, sizeof(distributor));
  std::cout << "msg send" << std::endl;

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