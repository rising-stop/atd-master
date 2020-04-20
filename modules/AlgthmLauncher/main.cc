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

#include "modules/common/io/ipc/common.h"
#include "modules/common/io/ipc/frame.h"
#include "modules/common/io/ipc/semaphore.h"
#include "modules/common/io/ipc/shm_com.h"

int main(char argc, char* argv[]) {
  FILE* fp_out = NULL;
  frame_t frame;
  int frame_cnt = 0;

  int sem_id;  // semaphore id

  int shm_id;  // shared-memory id
  void* shared_memory = (void*)0;
  shared_use_st* shared_stuff;
  int end_flag = 0;

  /* Open output file */
  if ((fp_out = fopen(OUTPUT_FILE, "wb")) < 0) {
    printf("Open output file failed: %s\n", OUTPUT_FILE);
    exit(EXIT_FAILURE);
  }

  /* Init frame */
  init_frame(&frame, FYUV_WIDTH, FYUV_HEIGHT, YUV420);
  printf("FRAME: w = %d, h = %d\n", frame.frm_w, frame.frm_h);

  /* Create semaphore */
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

  printf("FRAME_CNT: %d\n", frame_cnt);
  /*
   * 必须先置0，
   * 否则会因生产者进程的异常退出未释放信号量而导致程序出错
   */
  set_semvalue(sem_id, 0);

  do {
    semaphore_p(sem_id);

    /* Read frame from shared-memory */
    read_frame_from_shm(&frame, shared_stuff);
    end_flag = shared_stuff->end_flag;

    crop_frame(&frame, 10, 10, 40, 40);
    write_frame_into_file(fp_out, &frame);

    semaphore_v(sem_id);

    frame_cnt++;
    printf("FRAME_CNT: %d\n", frame_cnt);
  } while (!end_flag);

  /* Over */
  printf("\nConsumer over!\n");
  fclose(fp_out);
  free_frame(&frame);
  if (shmdt(shared_memory) == -1) {
    fprintf(stderr, "shmdt failed.\n");
    exit(EXIT_FAILURE);
  }

  exit(EXIT_SUCCESS);
}