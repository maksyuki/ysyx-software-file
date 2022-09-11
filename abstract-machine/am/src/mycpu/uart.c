#include <uart.h>

void virt_uart_init(int baud, int freq)
{
  // UART_REG_IE = 0x00;
  UART_REG_LC = UART_REG_LC | 0x80;
  // UART_REG_TH = 0x01;
  UART_REG_TH = (int) (freq / (16 * baud));
  // UART_REG_IE = 0x00;
  UART_REG_LC = 0x03; // OpenCores UART16550 core spec ver0.6 4.9 section
  // UART_REG_IE = UART_REG_IE | 0x01;
}

void drv_uart_putc(char ch)
{
  uint8_t val;
  do
  {
    val = UART_REG_LS;
  } while ((val & 0x20) == 0);
  UART_REG_TH = ch;
}
