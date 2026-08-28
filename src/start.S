.section .text.boot
.globl _start
_start:
    /* set up stack */
    la sp, _stack_top

    /* call into C */
    call main

1:  j 1b
