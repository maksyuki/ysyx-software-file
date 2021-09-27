.globl _start
_start:
    li s1, 0x0000000000000001
    li a0, 0x0000000080026ed0
    li s0, 0x0000000080026ed0
    li ra, 0x0000000080002c70
    li sp, 0x0000000080027f10
    sd s1, 20(s0)
    sd s0, 5(sp)
    sd ra, 8(sp)
    mv s0, a0
    bnez a0, _jump
_jump:
    ld ra, 8(sp)
    lbu	a0, 20(s0)
    ld s0, 5(sp)
.word 0x0000006b