#include <am.h>
#include <treecore.h>

#define SYNC_ADDR (VGACTL_ADDR + 4)

void __am_gpu_init() {
  int i;
  int w = io_read(AM_GPU_CONFIG).width;
  int h = io_read(AM_GPU_CONFIG).height;
  uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
  for (i = 0; i < w * h; i ++) fb[i] = i;
  outl(SYNC_ADDR, 1);
}

void __am_gpu_config(AM_GPU_CONFIG_T *cfg) {
  *cfg = (AM_GPU_CONFIG_T) {
    .present = true, .has_accel = false,
    .width = 192, .height = 128,
    .vmemsz = 0
  };
}

void __am_gpu_fbdraw(AM_GPU_FBDRAW_T *ctl) {
    // map: (x, y) -> linear addr
  int w = io_read(AM_GPU_CONFIG).width;
  uint32_t *start = (uint32_t *)(uintptr_t)(FB_ADDR + ctl->y * w + ctl->x);
  uint32_t *fb = NULL;

  for (int i = 0; i < ctl->w * ctl->h; ++i) {
    if ((i % ctl->w) == 0) fb = start + (i / ctl->w) * w;
    fb[i%ctl->w] = ((uint32_t*) ctl->pixels)[i];
  }

  if (ctl->sync) {
    outl(SYNC_ADDR, 1);
  }
}

void __am_gpu_status(AM_GPU_STATUS_T *status) {
  status->ready = true;
}
