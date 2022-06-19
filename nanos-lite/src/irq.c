#include <common.h>
#include <sys/time.h>
#define STRACE

#ifdef STRACE
static void strace(char *type, Context *c)
{
  Log("[strace] type: [%s] param: [%d %d %d] ret: [%d]", type, c->GPR2, c->GPR3, c->GPR4, c->GPRx);
}
#else
static void strace(char *type, Context *c)
{
}
#endif

extern Context *schedule();
extern int fs_open();
extern int fs_close();
extern size_t fs_read();
extern size_t fs_write();
extern size_t fs_lseek();
extern int cust_execve();
static Context *do_event(Event e, Context *c)
{
  switch (e.event)
  {
    // NOTE: for context switch
  case EVENT_YIELD:
  {
    // Log("[nanos-lite](kernel yield)process context switch");
    return schedule(c);
  }
  break;
  case EVENT_SYSCALL:
  {
    switch (c->GPR1)
    {
    case 0:
    {
      strace("SYS_exit", c);
      halt(c->GPR2); // HACK: right pos, or handle in navy-apps?
    }
    break;
    case 1:
    {
      c->GPRx = 0;
      strace("SYS_yield", c);
    }
    break;
    case 2:
    {
      const char *pathname = (const char *)c->GPR2;
      c->GPRx = fs_open(pathname, 0, 0);
      strace("SYS_open", c);
    }
    break;
    case 3:
    {
      int fd = (int)c->GPR2;
      void *buf = (void *)c->GPR3;
      size_t count = c->GPR4;
      c->GPRx = fs_read(fd, buf, count);
      strace("SYS_read", c);
    }
    break;
    case 4:
    {
      strace("SYS_write", c);
      int fd = c->GPR2;
      void *buf = (void *)c->GPR3;
      size_t len = (size_t)c->GPR4;
      c->GPRx = fs_write(fd, buf, len); // NOTE: support vfs oper
    }
    break;
    case 7:
    {
      int fd = (int)c->GPR2;
      c->GPRx = fs_close(fd);
    }
    break;
    case 8:
    {
      int fd = (int)c->GPR2;
      size_t offset = (size_t)c->GPR3;
      int whence = (int)c->GPR4;
      c->GPRx = fs_lseek(fd, offset, whence);
      strace("SYS_lseek", c);
    }
    break;
    case 9:
    {
      strace("SYS_brk", c);
      c->GPRx = 0;
    }
    break;
    case 13:
    {
      strace("SYS_execve", c);
      // man execve: replace current process
      char *fname = (char *)c->GPR2;
      char **argv = (char **)c->GPR3;
      char **envp = (char **)c->GPR4;
      printf("[SYS_execve]fname: %s\n", fname);
      c->GPRx = cust_execve(fname, argv, envp);
      // c->GPRx = 0;
    }
    break;
    case 19:
    {
      strace("SYS_gettimeofday", c);
      // Log("tv: %p", c->GPR2);
      struct timeval *tv = (struct timeval *)c->GPR2;
      // Log("io read: %ld", io_read(AM_TIMER_UPTIME).us);
      // for (volatile int i = 0; i < 100; i++) ; // HACK: fake impl
      tv->tv_usec = io_read(AM_TIMER_UPTIME).us;
      // tv = NULL;
      // Log("tv->tv_usec: %ld", tv->tv_usec);
      c->GPRx = 0;
    }
    break;
    default:
      panic("Unhandled syscall type = %d", c->GPR1);
    }
    break;
  }
  default:
    panic("Unhandled event ID = %d", e.event);
  }

  return c;
}

void init_irq(void)
{
  Log("Initializing interrupt/exception handler...");
  cte_init(do_event);
}
