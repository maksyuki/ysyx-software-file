#ifndef __UART_H
#define __UART_H

#include <stdio.h>
#include <am.h>
#include <klib-macros.h>

#define UART_REG_RB *((volatile uint8_t *)0x10000000)
#define UART_REG_TH *((volatile uint8_t *)0x10000000)
#define UART_REG_IE *((volatile uint8_t *)0x10000001)
#define UART_REG_II *((volatile uint8_t *)0x10000002)
#define UART_REG_FC *((volatile uint8_t *)0x10000002)
#define UART_REG_LC *((volatile uint8_t *)0x10000003)
#define UART_REG_MC *((volatile uint8_t *)0x10000004)
#define UART_REG_LS *((volatile uint8_t *)0x10000005)
#define UART_REG_MS *((volatile uint8_t *)0x10000006)

void virt_uart_init(int baud, int freq);
void drv_uart_putc(char ch);

#endif /* end __UART_H */