#include <proc.h>

#define MAX_NR_PROC 4

static PCB pcb[MAX_NR_PROC] __attribute__((used)) = {};
static PCB pcb_boot = {};
PCB *current = NULL;

void switch_boot_pcb() {
  current = &pcb_boot;
}

void hello_fun(void *arg) {
  int j = 1;
  while (1) {
    Log("Hello World from Nanos-lite with arg '%p' for the %dth time!", (uintptr_t)arg, j);
    j ++;
    yield();
  }
}

void context_kload(PCB *pcb, void (*entry)(void *), void *args) {
  Area kra;
  kra.start = &pcb->cp;
  kra.end = kra.start + STACK_SIZE;
  pcb->cp = kcontext(kra, entry, args);
}

extern void naive_uload();
void init_proc() {
  context_kload(&pcb[0], hello_fun, NULL);
  switch_boot_pcb();
  yield(); // NOTE: for test
  Log("Initializing processes...");

  // load program here
  naive_uload(NULL, "/bin/timer-test");
}

Context* schedule(Context *prev) {
  current->cp = prev;
  current = &pcb[0];
  return current->cp;
}
