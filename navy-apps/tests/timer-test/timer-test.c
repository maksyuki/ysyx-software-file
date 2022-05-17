#include <unistd.h>
#include <stdio.h>
#include <NDL.h>

int main() {
  uint32_t pre, cur;
  // printf("pre: %p\n", &pre);
  // printf("pre: %p\n", &cur);
  pre = NDL_GetTicks();
  while(1) {
    cur = NDL_GetTicks();
    if(cur - pre >= 500 * 1000) {
      pre = cur;
      printf("timer test\n");
    }
  }
  return 0;
}
