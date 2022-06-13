#include <common.h>

#if defined(MULTIPROGRAM) && !defined(TIME_SHARING)
# define MULTIPROGRAM_YIELD() yield()
#else
# define MULTIPROGRAM_YIELD()
#endif

#define NAME(key) \
  [AM_KEY_##key] = #key,

static const char *keyname[256] __attribute__((used)) = {
  [AM_KEY_NONE] = "NONE",
  AM_KEYS(NAME)
};

size_t serial_write(const void *buf, size_t offset, size_t len) {
  yield();
  char *arr = (char *)buf;
  for(int i = 0; i < len; ++i) {
    putch(arr[i]);
  }
  return len;
}

static uint8_t web_code_map[] = {
  0, 0, 0, 0, 0, 0, 0, 0, 27, 28, 0, 0, 0, 54, 0, 0, 55, 67, 69, 0,
  42, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 70, 81, 82, 80, 79, 75, 73, 76,
  74, 0, 0, 0, 0, 77, 78, 0, 24, 15, 16, 17, 18, 19, 20, 21, 22, 23, 0, 0,
  0, 0, 0, 0, 0, 43, 60, 58, 45, 31, 46, 47, 48, 36, 49, 50, 51, 62, 61, 37,
  38, 29, 32, 44, 33, 35, 59, 30, 57, 34, 56, 68, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 3, 4, 5, 6, 7, 8, 9,
  10, 11, 12, 13, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 52, 26, 63, 25, 64, 65, 14, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 39,
  41, 40, 53
};

size_t events_read(void *buf, size_t offset, size_t len) {
  yield();
  AM_INPUT_KEYBRD_T ev = io_read(AM_INPUT_KEYBRD);
  char *tmp = "";
  strcpy(tmp, keyname[web_code_map[ev.keycode]]);

  if (ev.keycode == AM_KEY_NONE) {
    strcpy(buf, tmp);
    return 0;
  }
  if (ev.keydown) strcat("kd", tmp);
  else strcat("ku", tmp);

  strcpy(buf, tmp);
  return strlen(buf);
}

size_t dispinfo_read(void *buf, size_t offset, size_t len) {
  return 0;
}

size_t fb_write(const void *buf, size_t offset, size_t len) {
  return 0;
}

void init_device() {
  Log("Initializing devices...");
  ioe_init();
}
