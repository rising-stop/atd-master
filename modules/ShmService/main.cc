/*
 * \File
 * producer.c
 * \Brief
 * Test shared-memory and message-queue
 */
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <unistd.h>

#include <iostream>
#include <thread>

#include "modules/common/common_support/timer/timer.h"
#include "modules/common/utility/ipc/common.h"
#include "modules/common/utility/ipc/frame.h"
#include "modules/common/utility/ipc/semaphore.h"
#include "modules/common/utility/ipc/shm_com.h"

#define SEM_SEED 1000

void CtlC_Handler(int sig) {
  atd::common::utility::SemMutex::Release_Sem();
  exit(EXIT_SUCCESS);
}

int main(int argc, char* argv[]) {
  signal(SIGINT, CtlC_Handler);
  frame_t frame;
  int frame_cnt = 0;

  int shm_id;  // shared-memory id
  void* shared_memory = (void*)0;
  shared_use_st* shared_stuff;

  /* Init frame */
  init_frame(&frame, FYUV_WIDTH, FYUV_HEIGHT, YUV420);
  printf("FRAME: w = %d, h = %d\n", frame.frm_w, frame.frm_h);

  atd::common::utility::Shared_SemMutex sem_mutex((key_t)SEM_SEED);

  /* Init shared-memory */
  shm_id = shmget((key_t)SHM_SEED, sizeof(shared_use_st), 0666 | IPC_CREAT);
  if (shm_id == -1) {
    fprintf(stderr, "shmget failed.\n");
    exit(EXIT_FAILURE);
  }

  shared_memory = shmat(shm_id, (void*)0, SHM_RND);
  if (shared_memory == (void*)-1) {
    fprintf(stderr, "shmat failed.\n");
    exit(EXIT_FAILURE);
  }

  shared_stuff = (shared_use_st*)shared_memory;
  shared_stuff->end_flag = 0;

  printf("FRAME_CNT: %d\n", frame_cnt);

  TIMER->set_BeginStick("counter");
  while (read_frame(&frame) == 0) {
    /* Write it into shared memory */
    TIMER->set_EndStick("counter");
    TIMER->set_BeginStick("counter");

    atd::common::utility::shared_lock<atd::common::utility::Shared_SemMutex>
        auto_lock(sem_mutex);

    write_frame_into_shm(shared_stuff, &frame);
    shared_stuff->end_flag = 0;

    auto_lock.unlock();

    std::this_thread::sleep_for(std::chrono::milliseconds(3000));

    frame_cnt++;
    printf("FRAME_CNT: %d\n", frame_cnt);
    std::cout << "last step elapse "
              << static_cast<double>(TIMER->get_TimeGap("counter")) / 1000
              << "ms" << std::endl;
  }

  /* over */
  printf("\nProducer over!\n");
  free_frame(&frame);
  if (shmdt(shared_memory) == -1) {
    fprintf(stderr, "shmdt failed.\n");
    exit(EXIT_FAILURE);
  }

  exit(EXIT_SUCCESS);
}