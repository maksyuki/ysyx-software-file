.globl _start
_start:
    addi x1, x0, 0x01
    addi x2, x0, 0x05
    addi x3, x0, 0x01
    addi x3, x0, 0x01
    addi x3, x0, 0x01
    bne  x1, x2, JUMP
    addi x2, x0, 0x05
    addi x1, x0, 0x01
JUMP:
    addi x2, x0, 0x05
    addi x1, x0, 0x01
.word 0x0000006b