#include <am.h>
#include <treecore.h>

#define KEYDOWN_MASK 0x8000

void __am_input_keybrd(AM_INPUT_KEYBRD_T *kbd) {
  kbd->keydown = inb(KBD_ADDR);
  // kbd->keycode = inb(KBD_ADDR+1);
  kbd->keycode = 1;
}
