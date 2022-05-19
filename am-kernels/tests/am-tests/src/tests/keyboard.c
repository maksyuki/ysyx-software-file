#include <amtest.h>

#define NAMEINIT(key)  [ AM_KEY_##key ] = #key,
static const char *names[] = {
  AM_KEYS(NAMEINIT)
};

// NOTE: web keycode map to am_keys
const uint8_t web_code_map[] = {
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

static bool has_uart, has_kbd;

static void drain_keys() {
  if (has_uart) {
    while (1) {
      char ch = io_read(AM_UART_RX).data;
      if (ch == -1) break;
      printf("Got (uart): %c (%d)\n", ch, ch & 0xff);
    }
  }

  if (has_kbd) {
    while (1) {
      AM_INPUT_KEYBRD_T ev = io_read(AM_INPUT_KEYBRD);
      if (ev.keycode == AM_KEY_NONE) break;
      printf("Got  (kbd): %s (%d) %s\n", names[web_code_map[ev.keycode]], ev.keycode, ev.keydown ? "DOWN" : "UP");
    }
  }
}

void keyboard_test() {
  printf("Try to press any key (uart or keyboard)...\n");
  has_uart = io_read(AM_UART_CONFIG).present;
  has_kbd  = io_read(AM_INPUT_CONFIG).present;
  while (1) {
    drain_keys();
  }
}
