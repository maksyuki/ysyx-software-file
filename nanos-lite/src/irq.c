#include <common.h>

// #define STRACE

#ifdef STRACE
static void strace(char *type, Context *c) {
  Log("[strace] type: [%s] param: [%d %d %d] ret: [%d]", type, c->GPR2, c->GPR3, c->GPR4, c->GPRx);
}
#else
static void strace(char *type, Context *c) {
}
#endif

static Context* do_event(Event e, Context* c) {
  switch (e.event) {
    case EVENT_YIELD:
      Log("[nanos-lite] ======YIELD TEST RESULT======"); break;
    case EVENT_SYSCALL:
    {
      switch(c->GPR1) {
        case 0: {
          strace("SYS_exit", c);
          halt(c->GPR2); // HACK: right pos, or handle in navy-apps?
          break;
        }
        case 1: {
          c->GPRx = 0;
          strace("SYS_yield", c);
          break;
        }
        case 4: {
          strace("SYS_write", c);
          if (c->GPR2 == 1 || c->GPR2 == 2) {
            char *buf = (char *) c->GPR3;
            for(int i = 0; i < c->GPR4; ++i) {
              putch(buf[i]);
            }
          }
          c->GPRx = c->GPR4;
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
