.globl _start
_start:

addi x1, x0, 1
addi x2, x0, 2
slt x3, x1, x2
.word 0x0000006b