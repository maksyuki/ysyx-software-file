#ifndef ARCH_H__
#define ARCH_H__

struct Context {
  // uintptr_t epc, cause, gpr[32], status;
  uintptr_t gpr[32], cause, status, epc;
  void *pdir;
};

#define GPR1 gpr[17] // a7 for 'c->GPR1 == -1' check in cte.c
#define GPR2 gpr[10]
#define GPR3 gpr[11]
#define GPR4 gpr[12]
#define GPRx gpr[10]

#endif
