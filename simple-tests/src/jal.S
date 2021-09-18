.globl _start
_start:
    addi x1, x0, 0x01
    addi x2, x0, 0x05
    addi x3, x0, 0x01
    jal  x4, JUMP
    addi x5, x0, 0x04
    addi x6, x0, 0x06
    addi x7, x0, 0x08
JUMP:
    addi x6, x0, 0x02
    addi x1, x0, 0x01

.word 0x0000006b