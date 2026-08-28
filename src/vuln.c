#include <stdint.h>
#include <stddef.h>

void uart_puts(const char *s);
void uart_putc(char c);
void print_hex64(uint64_t x);
uint32_t read_u32_le_blocking(void);
void read_bytes_blocking(uint8_t *dst, uint32_t n);
void *memcpy_unsafe(void *dst, const void *src, size_t n);


/* If built with `make SP=1`, we seed GCC's stack canary guard. */
#ifdef USE_STACK_PROTECTOR
extern uintptr_t __stack_chk_guard;
static inline uint64_t rdcycle64(void) {
    uint64_t x;
    __asm__ volatile("rdcycle %0" : "=r"(x));
    return x;
}
#endif

extern uintptr_t __stack_shadow_chk_guard;

/* ---------------------------
 * "Win" and "Shell" payloads
 * --------------------------- */

__attribute__((noinline))
void win(void) {
    uart_puts("\n*** WIN(): control flow hijacked! ***\n");
    uart_puts("If this were a real system, attacker code would run now.\n");
    while (1) { }
}

/* ---------------------------
 * Vulnerable function
 * --------------------------- */

__attribute__((noinline))
static void vuln_copy(const uint8_t *in, uint32_t len) {
    /* Classic stack buffer overflow: local buffer is 64B, but len is attacker-controlled. */
    volatile uint8_t buf[64];

    /* Print some useful addresses for learning/debugging */
    uart_puts("[vuln_copy] buf @ "); print_hex64((uint64_t)(uintptr_t)buf); uart_puts("\n");

    /* BUG: no bounds check */
    memcpy_unsafe((void*)buf, in, (size_t)len);

    /* Use buf so it won't be optimized away */
    uart_puts("[vuln_copy] done, first byte = "); uart_putc((char)buf[0]); uart_puts("\n");
}

/* ---------------------------
 * Entry point
 * --------------------------- */

int main(void) {
    // baremetal RISCV doesn't initialize the stack canary guard for us, so we do it here if needed.
#ifdef USE_STACK_PROTECTOR
    __stack_chk_guard = (uintptr_t)(rdcycle64() ^ 0x9e3779b97f4a7c15ULL);
#endif


    uart_puts("\nInput format: [4-byte little-endian length L] [L bytes payload]\n");
    uart_puts("Send payload.bin via stdin redirection:  make run < payload.bin\n\n");

    uint32_t L = read_u32_le_blocking();
    if (L > 256) {
        uart_puts("[main] Length too large; clamping to 256\n");
        L = 256;
    }

    static uint8_t inbuf[256];
    read_bytes_blocking(inbuf, L);

    uart_puts("[main] calling vuln_copy(len=");
    print_hex64((uint64_t)L);
    uart_puts(")\n");

    vuln_copy(inbuf, L);

    uart_puts("[main] returned normally (exploit failed)\n");
    while (1) { }
}
