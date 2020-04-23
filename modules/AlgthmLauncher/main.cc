/*
 * \File
 * consumer.c
 * \Brief
 *
 */
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <unistd.h>

#include <thread>

#include "modules/common/utility/ipc/common.h"
#include "modules/common/utility/ipc/frame.h"
#include "modules/common/utility/ipc/semaphore.h"
#include "modules/common/utility/ipc/shm_com.h"

int main(int argc, char* argv[]) {
  frame_t frame;
  int frame_cnt = 0;

  int shm_id;  // shared-memory id
  void* shared_memory = (void*)0;
  shared_use_st* shared_stuff;
  int end_flag = 0;

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

  shared_memory = shmat(shm_id, (void*)0, SHM_RDONLY);
  if (shared_memory == (void*)-1) {
    fprintf(stderr, "shmat failed.\n");
    exit(EXIT_FAILURE);
  }
  shared_stuff = (shared_use_st*)shared_memory;

  printf("FRAME_CNT: %d\n", frame_cnt);

  do {
    sem_mutex.shared_unlock();

    /* Read frame from shared-memory */
    read_frame_from_shm(&frame, shared_stuff);
    end_flag = shared_stuff->end_flag;

    // crop_frame(&frame, 10, 10, 40, 40);
    write_frame(&frame);

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    sem_mutex.shared_unlock();

    frame_cnt++;
    printf("FRAME_CNT: %d\n", frame_cnt);

  } while (!end_flag);

  /* Over */
  printf("\nConsumer over!\n");
  free_frame(&frame);
  if (shmdt(shared_memory) == -1) {
    fprintf(stderr, "shmdt failed.\n");
    exit(EXIT_FAILURE);
  }

  exit(EXIT_SUCCESS);
}