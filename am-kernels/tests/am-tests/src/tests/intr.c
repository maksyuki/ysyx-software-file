#include <amtest.h>
#define CLINT_START 0x2000000
#define CLINT_MTIMECMP (CLINT_START + 0x4000)

Context *simple_trap(Event ev, Context *ctx) {
  switch(ev.event) {
    case EVENT_IRQ_TIMER:
      *((uint64_t *) CLINT_MTIMECMP) += 700000;
      putch('t'); break;
    case EVENT_IRQ_IODEV:
      putch('d'); break;
    case EVENT_YIELD:
      putch('y'); break;
    default:
      break;
  }
  return ctx;
}

void hello_intr() {
  printf("Hello, AM World @ " __ISA__ "\n");
  printf("  t = timer, d = device, y = yield\n");
  io_read(AM_INPUT_CONFIG);
  iset(1);
  while (1) {
    for (volatile int i = 0; i < 10000; i++) ;
    yield();
  }
}
