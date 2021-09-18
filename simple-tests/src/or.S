.globl _start
_start:

addi x1, x0, 0x01
addi x2, x0, 0x02
or x3, x1, x2
.word 0x0000006b