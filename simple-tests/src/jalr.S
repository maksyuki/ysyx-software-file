.globl _start
_start:
    auipc x1, 0
    addi x5, x0, 0x02
    jalr x6, 0x18(x1)
    addi x4, x0, 0x05
    addi x5, x0, 0x01
    addi x6, x0, 0x05
    addi x1, x0, 0x29 # 0x18
    addi x7, x0, 0x01
.word 0x0000006b