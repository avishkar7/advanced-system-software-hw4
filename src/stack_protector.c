#include <stdint.h>

void uart_puts(const char *s);

/* GCC stack protector expects these symbols.
 * We provide them for bare-metal builds. */
uintptr_t __stack_chk_guard = 0;

__attribute__((noreturn))
void __stack_chk_fail(void) {
    uart_puts("\n*** STACK SMASHING DETECTED (__stack_chk_fail) ***\n");
    while (1) { }
}
