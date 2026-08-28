#include <stdint.h>
#include <stddef.h>

void uart_putc(char c);
void uart_puts(const char *s);

static const char hexchars[] = "0123456789abcdef";

void print_hex64(uint64_t x) {
    uart_puts("0x");
    for (int i = 15; i >= 0; i--) {
        uart_putc(hexchars[(x >> (i*4)) & 0xF]);
    }
}

uint32_t read_u32_le_blocking(void) {
    extern char uart_getc_blocking(void);
    uint32_t v = 0;
    for (int i = 0; i < 4; i++) {
        v |= ((uint32_t)(uint8_t)uart_getc_blocking()) << (8*i);
    }
    return v;
}

void read_bytes_blocking(uint8_t *dst, uint32_t n) {
    extern char uart_getc_blocking(void);
    for (uint32_t i = 0; i < n; i++) dst[i] = (uint8_t)uart_getc_blocking();
}

void *memcpy_unsafe(void *dst, const void *src, size_t n) {
    uint8_t *d = (uint8_t*)dst;
    const uint8_t *s = (const uint8_t*)src;
    for (size_t i = 0; i < n; i++) d[i] = s[i];
    return dst;
}
