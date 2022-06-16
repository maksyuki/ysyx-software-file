#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

int main(int argc, char *argv[], char *envp[]);
extern char **environ;
void call_main(uintptr_t *args) {
  printf("[call_main]args: %p %d\n", args, *(int *)args);
  int argc = *(int *)args;
  char *empty[] =  {NULL };
  environ = empty;
  exit(main(argc, empty, empty));
  assert(0);
}
