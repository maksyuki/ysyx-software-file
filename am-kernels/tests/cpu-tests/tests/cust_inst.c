#include "trap.h"

char constChar = '@';

int main() {
    
    putch(constChar);
    check(constChar == '@');

    return 0;
}