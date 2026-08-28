# Advanced System Software — HW4: Stack Buffer Overflow & ret2win on RISC-V

> Part of the [Advanced System Software](https://github.com/avishkar7/advanced-system-software) portfolio.

## Assignment

Explore stack buffer overflow vulnerabilities on a bare-metal RISC-V system
running under QEMU, across four tasks:

1. Study the RISC-V calling convention and stack-frame layout.
2. Trigger a buffer overflow and observe its effects in GDB.
3. Craft an exploit payload that hijacks control flow (`ret2win`).
4. Enable and evaluate a compiler-based defense (GCC stack protector).

## Approach

- **The vulnerability.** `vuln_copy()` in [`src/vuln.c`](src/vuln.c) copies an
  attacker-controlled number of bytes into a fixed 64-byte stack buffer via
  `memcpy_unsafe()` with no bounds check. Input arrives as a 4-byte
  little-endian length followed by that many payload bytes.
- **Finding the offset.** Disassembly and GDB were used to measure the distance
  from the buffer to the saved return address (`ra`) on the stack — 72 bytes.
  The disassembly evidence is captured under [`analysis/`](analysis/)
  (with- and without-stack-protector dumps of `main` and `memcpy`).
- **ret2win.** [`exploit.py`](exploit.py) emits `72 × "A"` padding followed by
  the little-endian address of `win()` (`0x8000029e`), prefixed with the 4-byte
  length header, redirecting control flow into `win()` on function return.
- **Defense.** Rebuilding with `make SP=1` compiles in
  `-fstack-protector-strong` and seeds `__stack_chk_guard` at startup
  (bare-metal has no libc to do it). The report evaluates how the canary
  detects the overwrite and aborts before the corrupted `ra` is used.

## Layout

| Path | Contents |
|------|----------|
| `src/vuln.c` | Vulnerable program: `vuln_copy`, `win`, input handling |
| `src/uart.c`, `src/util.c`, `src/start.S` | UART I/O and startup |
| `src/stack_protector.c` | Guard support compiled in under `SP=1` |
| `exploit.py` | Generates `payload.bin` (padding + `win` address) |
| `payload.bin` | Prebuilt exploit payload |
| `Makefile`, `linker.ld` | Build (`make`, `make SP=1`, `make run`) |
| `analysis/` | GDB/objdump evidence: disassembly and stack-frame notes |
| `report/report.pdf` | Full write-up with screenshots per task |
| `docs/handout.pdf` | Original assignment handout |

## Build & run

Requires the RISC-V toolchain (`riscv64-unknown-elf-*`) and
`qemu-system-riscv64`.

```bash
make                 # build vulnerable program (no protection)
python3 exploit.py > payload.bin
make run < payload.bin   # feed payload via stdin → *** WIN(): control flow hijacked! ***

make SP=1            # rebuild with GCC stack protector
make SP=1 run < payload.bin   # canary detects the overflow and aborts
```
