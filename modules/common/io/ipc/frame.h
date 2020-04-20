/*
 * \File
 * frame.h
 * \Brief
 *
 */
#ifndef __FRAME_H__
#define __FRAME_H__

#include "shm_com.h"

#define PIX_VALUE 1

typedef enum { YUV420, YUV422, YUV444, RGB } frame_type_e;

typedef struct {
  int frm_w;  // width
  int frm_h;  // height
  frame_type_e frm_type;
  int frm_size;

  char* frm_comps;
} frame_t, *frame_p;

int init_frame(frame_p frame, int width, int height, frame_type_e type);
int free_frame(frame_p frame);

int read_frame_from_file(frame_p frame, FILE* fp);
int write_frame_into_file(FILE* fp, frame_p frame);

int read_frame_from_shm(frame_p frame, shared_use_st* shm);
int write_frame_into_shm(shared_use_st* shm, frame_p frame);

int crop_frame(frame_p frame, int l_offset, int t_offset, int c_w, int c_h);

#endif