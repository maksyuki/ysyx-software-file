#include <common.h>

static void strace(char *type, Context *c) {
  Log("[strace] type: [%s] param: [%d %d %d %d] ret: [%d]", type, c->GPR1, c->GPR2, c->GPR3, c->GPR4, c->GPRx);
}

static Context* do_event(Event e, Context* c) {
  switch (e.event) {
    case EVENT_YIELD:
      Log("[nanos-lite] ======YIELD TEST RESULT======"); break;
    case EVENT_SYSCALL:
    {
      switch(c->GPR1) {
        case 0: {
          // Log("[nanos-lite] ======SYSCALL EXIT RESULT======");
          strace("SYS_exit", c);
          halt(c->GPR2); // HACK: right pos?
          break;
          }
        case 1: {
          // Log("[nanos-lite] ======SYSCALL YIELD RESULT======");
          c->GPRx = 0;
          strace("SYS_yield", c);
          break;
          }
        default: panic("Unhandled syscall type = %d", c->GPR1);
      }
      break;
    }

    default: panic("Unhandled event ID = %d", e.event);
  }

  return c;
}

void init_irq(void) {
  Log("Initializing interrupt/exception handler...");
  cte_init(do_event);
}
