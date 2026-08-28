OUTPUT_ARCH("riscv")
ENTRY(_start)

SECTIONS
{
  /* QEMU virt RAM starts at 0x80000000 */
  . = 0x80000000;

  .text : {
    *(.text.boot)
    *(.text*)
  }

  .rodata : { *(.rodata*) }
  .data   : { *(.data*) }
  .bss    : {
    *(.bss*)
    *(COMMON)
  }

  . = ALIGN(16);
  _stack_bottom = .;
  . = . + 0x4000;  /* 16 KiB stack */
  _stack_top = .;
}
