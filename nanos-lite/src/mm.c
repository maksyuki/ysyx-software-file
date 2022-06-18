#include <memory.h>

static void *pf = NULL;

void* new_page(size_t nr_page) {
  // BUG: maybe need to check mem bound, now dont need to impl func
  pf += nr_page * PGSIZE;
  return pf - nr_page * PGSIZE;
}

static inline void* pg_alloc(int n) {
  return NULL;
}

void free_page(void *p) {
  panic("not implement yet");
}

/* The brk() system call handler. */
int mm_brk(uintptr_t brk) {
  return 0;
}

void init_mm() {
  pf = (void *)ROUNDUP(heap.start, PGSIZE);
  Log("free physical pages starting from %p to %p", pf, heap.end);

#ifdef HAS_VME
  vme_init(pg_alloc, free_page);
#endif
}
