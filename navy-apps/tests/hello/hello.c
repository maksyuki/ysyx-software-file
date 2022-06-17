#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>

bool check(char *s1, char *s2) {
  int i = 0, j = 0;
  // for(i = 0, j = 0; s1[i] && s2[j]; ) {
  //   if(s1[i] == s2[j]) {
  //     ++i, ++j;
  //   } else {
  //     break;
  //   }
  // }

  // if (s1[0] == '-' && s1[1] == '-' && s1[2] == 'h') return true;
  return true;
  // else return false;
}

int main(int argc, char *argv[], char *envp[]) {
  printf("[hello]argc: %d\n", argc);
  // if (check(argv[0], "--h")) {
    // printf("help info\n");
  // }

  // for(int i = 0; i < argc; ++i) {
    // printf("[hello]argv[%d]: %s\n", i, argv[i]);
  // }

  write(1, "Hello World!\n", 13);
  int i = 2;
  volatile int j = 0;
  while (1) {
    j ++;
    if (j == 10000) {
      printf("Hello World from Navy-apps for the %dth time!\n", i ++);
      j = 0;
    }
  }
  return 0;
}
