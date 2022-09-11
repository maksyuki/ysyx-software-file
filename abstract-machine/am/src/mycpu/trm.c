#include <am.h>
#include <klib-macros.h>
#include <uart.h>

extern char _heap_start;
int main(const char *args);

extern char _pmem_start;
#define PMEM_SIZE (128 * 1024 * 1024)
#define PMEM_END ((uintptr_t)&_pmem_start + PMEM_SIZE)

Area heap = RANGE(&_heap_start, PMEM_END);
#ifndef MAINARGS
#define MAINARGS ""
#endif
static const char mainargs[] = MAINARGS;

#define SOC_ENV
// NOTE: comment for soc prj, because soc need uart to putch
void putch(char ch)
{
#ifdef SOC_ENV
  drv_uart_putc(ch);
#else
  asm volatile("mv a0, %0; .word 0x0000007b"
               :
               : "r"(ch));
#endif
}

void halt(int code)
{
#ifdef SOC_ENV
  asm volatile("nop");
#else
  asm volatile("mv a0, %0; .word 0x0000006b"
               :
               : "r"(code));
#endif
  while (1)
    ;
}

void _trm_init()
{
#ifdef SOC_ENV
  virt_uart_init(115200, 25000000);
#endif
  int ret = main(mainargs);
  halt(ret);
}
