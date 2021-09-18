.globl _start
_start:

auipc ra, 0x0
addi ra, ra, 1104 # 80000450 <_etext>
li a0, 40
sb a0, 0(ra)
li a1, 45
lb t0, 0(ra)

li a0, 41
sb a0, 1(ra)
li a1, 46
lb t0, 1(ra)

li a0, 42
sb a0, 2(ra)
li a1, 47
lb t0, 2(ra)

lui	a0, 0xfffff
addiw	a0,a0,-96
# li a0, 43
sb a0, 3(ra)
li a1, 48
lb t0, 3(ra)
.word 0x0000006b
