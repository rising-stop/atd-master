/*
 * \File
 * frame.c
 */
#include "frame.h"

#include <assert.h>
#include <errno.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <iostream>

/*
 * \Brief
 * init frame
 */
int init_frame(frame_p frame, int width, int height, frame_type_e type) {
  assert(frame != NULL);

  frame->frm_w = width;
  frame->frm_h = height;
  frame->frm_type = type;

  switch (frame->frm_type) {
    case YUV420:
      frame->frm_size = (frame->frm_w * frame->frm_h * 3) / 2;
      break;
    case YUV422:
      frame->frm_size = frame->frm_w * frame->frm_h * 2;
      break;
    case YUV444:
      frame->frm_size = frame->frm_w * frame->frm_h * 3;
      break;
    case RGB:
      frame->frm_size = frame->frm_w * frame->frm_h * 3;
      break;
    default:
      fprintf(stderr, "frame type is invalid.");
      return 1;
  }

  if ((frame->frm_comps = (char*)calloc(frame->frm_size, sizeof(char))) ==
      NULL) {
    fprintf(stderr, "calloc failed.");
    return 1;
  }

  return 0;
}

/*
 * \Brief
 * init frame
 */
int free_frame(frame_p frame) {
  assert(frame != NULL);

  free(frame->frm_comps);
  return 0;
}

/*
 * \Brief
 * read a frame from file
 */
int read_frame(frame_p frame) {
  int ret;
  assert(frame != NULL);

  for (int index4loop = 0; index4loop < frame->frm_size; index4loop++) {
    if (index4loop == frame->frm_size - 1) {
      frame->frm_comps[index4loop] = '\0';
    } else {
      frame->frm_comps[index4loop] = 'a';
    }
  }

  return 0;
}

void write_frame(frame_p frame) {
  std::cout << "frame length = " << frame->frm_size
            << ", 10th element contents: " << frame->frm_comps[10] << std::endl;
}

/*
 * \Brief
 * read a frame from shared-memory
 */
int read_frame_from_shm(frame_p frame, shared_use_st* shm) {
  assert(frame != NULL && shm != NULL);

  memcpy((char*)frame->frm_comps, (char*)shm->shm_sp, frame->frm_size);

  return 0;
}

/*
 * \Brief
 * write a frame into shared-memory
 */
int write_frame_into_shm(shared_use_st* shm, frame_p frame) {
  assert(frame != NULL && shm != NULL);

  memcpy((char*)shm->shm_sp, (char*)frame->frm_comps, frame->frm_size);

  return 0;
}

/*
 * \Brief
 * crop frame
 */
int crop_frame(frame_p frame, int l_offset, int t_offset, int c_w, int c_h) {
  char* crop_loc;
  int index_h;

  assert(frame != NULL);

  if ((l_offset + c_w) > frame->frm_w) {
    printf("Crop width is out of range.\n");
    return 1;
  }
  if ((t_offset + c_h) > frame->frm_h) {
    printf("Crop height is out of range.\n");
    return 1;
  }

  crop_loc = frame->frm_comps + (t_offset * frame->frm_w) + l_offset;
  for (index_h = 0; index_h < c_h; index_h++) {
    memset(crop_loc, PIX_VALUE, c_w);

    crop_loc += frame->frm_w;
  }

  return 0;
}