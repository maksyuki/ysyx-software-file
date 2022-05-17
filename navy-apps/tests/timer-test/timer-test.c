#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>

int main() {
  struct timeval cur_tv, nxt_tv;
  // printf("cur: %p\n", &cur_tv);
  // printf("cur: %p\n", &nxt_tv);
  assert(!gettimeofday(&cur_tv, NULL));
  while(1) {
    assert(!gettimeofday(&nxt_tv, NULL));
    // printf("cur's tv_usec: %ld\n", cur_tv.tv_usec);
    // printf("nxt's tv_usec: %ld\n", nxt_tv.tv_usec);
    // printf("\n");
    if(nxt_tv.tv_usec - cur_tv.tv_usec >= 500 * 1000) {
      cur_tv = nxt_tv;
      printf("timer test\n");
    }
  }
  return 0;
}
