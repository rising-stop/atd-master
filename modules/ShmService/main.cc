/*
 * \File
 * producer.c
 * \Brief
 * Test shared-memory and message-queue
 */
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <unistd.h>

#include "modules/common/io/ipc/common.h"
#include "modules/common/io/ipc/frame.h"
#include "modules/common/io/ipc/semaphore.h"
#include "modules/common/io/ipc/shm_com.h"

int main(char argc, char* argv[]) {
  FILE* fp_in = NULL;
  frame_t frame;
  int frame_cnt = 0;

  int sem_id;  // semaphore id

  int shm_id;  // shared-memory id
  void* shared_memory = (void*)0;
  shared_use_st* shared_stuff;

  /* Open input file */
  if ((fp_in = fopen(TEST_FILE, "rb")) < 0) {
    printf("Open input file failed: %s\n", TEST_FILE);
    exit(EXIT_FAILURE);
  }

  /* Init frame */
  init_frame(&frame, FYUV_WIDTH, FYUV_HEIGHT, YUV420);
  printf("FRAME: w = %d, h = %d\n", frame.frm_w, frame.frm_h);

  /* Create and init semaphore */
  sem_id = semget((key_t)SEM_SEED, 1, 0666 | IPC_CREAT);
  if (sem_id == -1) {
    fprintf(stderr, "semget failed.\n");
    exit(EXIT_FAILURE);
  }

  /* Init shared-memory */
  shm_id =
      shmget((key_t)SHM_SEED, sizeof(shared_use_st), 0666 | IPC_CREAT);
  if (shm_id == -1) {
    fprintf(stderr, "shmget failed.\n");
    exit(EXIT_FAILURE);
  }

  shared_memory = shmat(shm_id, (void*)0, 0);
  if (shared_memory == (void*)-1) {
    fprintf(stderr, "shmat failed.\n");
    exit(EXIT_FAILURE);
  }

  shared_stuff = (shared_use_st*)shared_memory;
  shared_stuff->end_flag = 0;

  printf("FRAME_CNT: %d\n", frame_cnt);
  set_semvalue(sem_id, 1);

  while (read_frame_from_file(&frame, fp_in) == 0) {
    semaphore_p(sem_id);

    /* Write it into shared memory */
    write_frame_into_shm(shared_stuff, &frame);
    shared_stuff->end_flag = 0;

    semaphore_v(sem_id);

    frame_cnt++;
    printf("FRAME_CNT: %d\n", frame_cnt);
  }
  semaphore_p(sem_id);
  shared_stuff->end_flag = 1;
  semaphore_v(sem_id);

  /* over */
  printf("\nProducer over!\n");
  fclose(fp_in);
  free_frame(&frame);
  del_semvalue(sem_id);
  if (shmdt(shared_memory) == -1) {
    fprintf(stderr, "shmdt failed.\n");
    exit(EXIT_FAILURE);
  }

  exit(EXIT_SUCCESS);
}