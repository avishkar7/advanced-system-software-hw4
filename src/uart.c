#include <stdint.h>

#define UART0_BASE 0x10000000UL
#define UART_RHR   0x0  /* receive holding register (read) */
#define UART_THR   0x0  /* transmit holding register (write) */
#define UART_LSR   0x5  /* line status register */
#define LSR_RX_READY 0x01
#define LSR_TX_IDLE  0x20

static inline volatile uint8_t *uart_reg(uint64_t off) {
    return (volatile uint8_t *)(UART0_BASE + off);
}

void uart_putc(char c) {
    /* wait until transmitter empty */
    while ( (*uart_reg(UART_LSR) & LSR_TX_IDLE) == 0 ) { }
    *uart_reg(UART_THR) = (uint8_t)c;
}

int uart_getc(void) {
    /* returns -1 if nothing available */
    if ( (*uart_reg(UART_LSR) & LSR_RX_READY) == 0 ) return -1;
    return (int)(*uart_reg(UART_RHR));
}

char uart_getc_blocking(void) {
    int c;
    while ((c = uart_getc()) < 0) { }
    return (char)c;
}

void uart_puts(const char *s) {
    while (*s) uart_putc(*s++);
}
