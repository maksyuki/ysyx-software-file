#include <am.h>
#include <treecore.h>
#include <klib.h>

static Context* (*user_handler)(Event, Context*) = NULL;

Context* __am_irq_handle(Context *c) {
  if (user_handler) {
    Event ev = {0};
    switch (c->cause) {
      case 0x8000000000000007:
        ev.event = EVENT_IRQ_TIMER;
        break;
      case 11:
        if (c->GPR1 == -1) {
          ev.event = EVENT_YIELD;
          c->epc += 4; // important!!!
        } else {
          ev.event = EVENT_SYSCALL;
          c->epc += 4; // important!!!
        }
        break;
      default:
        ev.event = EVENT_ERROR;
        break;
    }

    c = user_handler(ev, c);
    assert(c != NULL);
  }

  return c;
}

extern void __am_asm_trap(void);

bool cte_init(Context*(*handler)(Event, Context*)) {
  // initialize exception entry
  // asm volatile("csrw stvec, %0" : : "r"(__am_asm_trap));
  asm volatile("csrw mtvec, %0" : : "r"(__am_asm_trap)); // NOTE: just test only machine mode!

  // register event handler
  user_handler = handler;

  return true;
}

Context *kcontext(Area kstack, void (*entry)(void *), void *arg) {
  Context *res = kstack.end - sizeof(Context) - 4;
  res->status = 0x1800; // NOTE: only exec once
  res->epc = (uintptr_t)entry;
  res->gpr[10] = (uintptr_t) arg; // calling convention->a0
  // printf("res->status: %d", res->status);
  return res;
}

void yield() {
  asm volatile("li a7, -1; ecall");
}

bool ienabled() {
  return false;
}

// interrupt set, just for interrupt test
void iset(bool enable) {
  // uint64_t mie_mtie = 0x080;
  // if (enable) {
    // asm volatile("csrsi mstatus, 8");
    // asm volatile("csrs mie, %0" : : "r"(mie_mtie));
  // } else {
    // asm volatile("csrci mstatus, 8");
    // asm volatile("csrc mie, %0" : : "r"(mie_mtie));
  // }
}