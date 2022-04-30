#include "trap.h"

char constChar = '@';

int main() {
    
    for (int i = 0; i < 10; ++i) {
        printf("[%d]: ", i);
        putch(constChar);
        putch('\n');
    }
    
    printf("hello world\n");
    printf("I am maksyuki\n");
    check(constChar == '@');

    return 0;
}