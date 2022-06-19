#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  int n = (argc >= 2 ? atoi(argv[1]) : 1);
  printf("[exec-test]%s: argv[1] = %d\n", argv[0], n);

  // stop recursive exec
  if (n == 2) return 0;
  char buf[16];
  int cnt = 0;
  while(n) {
    buf[cnt++] = n % 10 + '0';
    n /= 10;
  }

  buf[cnt++] = 0;
  printf("[exec-test]buf: %s\n", buf);
  // sprintf(buf, "%d", n + 1);
  execl(argv[0], argv[0], buf, NULL);
  return 0;
}
