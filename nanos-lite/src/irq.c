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

extern int fs_open();
extern int fs_close();
extern size_t fs_read();
extern size_t fs_write();
extern size_t fs_lseek();
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
        case 2: {
          const char *pathname = (const char *)c->GPR2;
          c->GPRx = fs_open(pathname, 0, 0);
          strace("SYS_open", c);
          break;
        }
        case 3: {
          int fd = (int)c->GPR2;
          void *buf = (void *)c->GPR3;
          size_t count = c->GPR4;
          c->GPRx = fs_read(fd, buf, count);
          strace("SYS_read", c);
          break;
        }
        case 4: {
          strace("SYS_write", c);
          if (c->GPR2 == 1 || c->GPR2 == 2) {
            char *buf = (char *) c->GPR3;
            for(int i = 0; i < c->GPR4; ++i) {
              putch(buf[i]);
            }
            c->GPRx = c->GPR4;
          } else {
            int fd = c->GPR2;
            void *buf = (void *) c->GPR3;
            size_t len = (size_t)c->GPR4;
            c->GPRx = fs_write(fd, buf, len);
          }
          break;
        }
        case 7: {
          int fd = (int)c->GPR2;
          c->GPRx = fs_close(fd);

        }
        case 8: {
          int fd = (int)c->GPR2;
          size_t offset = (size_t)c->GPR3;
          int whence = (int)c->GPR4;
          c->GPRx = fs_lseek(fd, offset, whence);
          strace("SYS_lseek", c);
          break;
        }
        case 9: {
          strace("SYS_brk", c);
          c->GPRx = 0;
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
