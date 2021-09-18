.globl _start
_start:

addi x1, x0, 0x3
addi x2, x0, 0x6
addi x2, x0, 0x1
addi x3, x0, 0x4
csrr t0, mcycle
addi x2, x0, 0x8
addi x2, x0, 0x8
addi x2, x0, 0x8
addi x2, x0, 0x8
csrr t0, mcycle
.word 0x0000006b