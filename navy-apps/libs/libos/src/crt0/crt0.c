#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[], char *envp[]);
extern char **environ;
void call_main(uintptr_t *args) {
  printf("[call_main]args: %p %d\n", args, *(int *)args);
  int argc = *(int *)args;
  char *argv[argc];
  char *envp[] =  { NULL };
  for(int i = 0; i < argc; ++i) {
    if (i == 0) {
      argv[i] = (char *)((int *)args + 1);
    } else {
      argv[i] = (char *)((int *)args + 1) + strlen(argv[i-1]) + 1;
    }
  }

  for(int i = 0; i < argc; ++i) {
    printf("[call_main]argv[%d]: %s\n", i, argv[i]);
  }

  environ = envp;
  exit(main(argc, argv, envp));
  assert(0);
}
