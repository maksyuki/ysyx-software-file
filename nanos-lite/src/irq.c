#include <common.h>

static Context* do_event(Event e, Context* c) {
  switch (e.event) {
    case EVENT_YIELD:
      Log("[nanos-lite] ======YIELD TEST RESULT======"); break;
    case EVENT_SYSCALL: break;
      // Log("[nanos-lite] ======SYSCALL TEST RESULT======"); break;
    default: panic("Unhandled event ID = %d", e.event);
  }

  return c;
}

void init_irq(void) {
  Log("Initializing interrupt/exception handler...");
  cte_init(do_event);
}
