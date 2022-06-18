#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

int main(int argc, char *argv[], char *envp[]);
extern char **environ;
void call_main(uintptr_t *args) {
  printf("[call_main]args: %p\n", args);
  uintptr_t *p = args;
  int argc = *(int *)p;
  printf("p addr: %p\n", p);
  p = (uintptr_t *)((int *)p + 1);
  printf("p addr: %p\n", p);

  char *argv[argc];
  char *envp[] =  { NULL };
  for(int i = 0; i < argc; ++i) {
    argv[i] = (char *)(*(uintptr_t *)p);
    printf("p addr: %p, %lx\n", p, *(uintptr_t *)p);
    ++p;
  }

  for(int i = 0; i < argc; ++i) {
    printf("[call_main]argv[%d]: %s\n", i, argv[i]);
  }

  environ = envp;
  exit(main(argc, argv, envp));
  assert(0);
}
