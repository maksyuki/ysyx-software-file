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
    // Log("Hello World from Nanos-lite with arg '%s' for the %dth time!", (char *)arg, j);
    printf("[hello_fun] Hello World from Nanos-lite with arg '%s' for the %dth time!\n", (char *)arg, j);
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
extern void context_uload();
void init_proc() {
  context_kload(&pcb[0], hello_fun, "1");
  // context_kload(&pcb[1], hello_fun, "2");
  char *empty[] =  {NULL};
  // char *demo[] = {"--h", "-t", NULL};
  // context_uload(&pcb[1], "/bin/hello", empty, empty);
  context_uload(&pcb[1], "/bin/exec-test", empty, empty);
  switch_boot_pcb();
  yield(); // NOTE: for test context switch
  Log("Initializing processes...");

  // load program here
  // naive_uload(NULL, "/bin/dummy");
}

Context* schedule(Context *prev) {
  current->cp = prev;
  current = (current == &pcb[0] ? &pcb[1] : &pcb[0]);
  printf("[schedule]: switch to pcb[%d] cp: %p\n", current == &pcb[0]? 0 : 1, current->cp);
  return current->cp;
}

int cust_execve(const char *fname, char * const argv[], char *const envp[]) {
  context_uload((current == &pcb[0]) ? &pcb[1] : &pcb[0], fname, argv, envp);
  switch_boot_pcb();
  printf("[cust_execve]\n");
  yield();
  return 0;
}
