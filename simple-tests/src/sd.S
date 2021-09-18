.globl _start
_start:
    addi x1, x0, 0x24
    sd x2, 0x0(x1)
.word 0x0000006b