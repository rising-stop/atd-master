/*
 * \File
 * shm_com.h
 * \Brief
 */
#ifndef __SHM_COM_H__
#define __SHM_COM_H__

#define SHM_SEED 1001
#define MAX_SHM_SIZE 2048 * 2048 * 3

typedef struct {
  int end_flag;  //用来标记进程间的内存共享是否结束: 0, 未结束； 1， 结束
  char shm_sp[MAX_SHM_SIZE];  //共享内存的空间
} shared_use_st;

#endif