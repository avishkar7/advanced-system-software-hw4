# EECS211 HW4 starter Makefile (bare-metal RISC-V)

TOOLPREFIX ?= riscv64-unknown-elf-
CC      := $(TOOLPREFIX)gcc
OBJCOPY := $(TOOLPREFIX)objcopy
OBJDUMP := $(TOOLPREFIX)objdump
NM      := $(TOOLPREFIX)nm

SP ?= 0

CFLAGS  := -O0 -g -Wall -Wextra -ffreestanding -nostdlib -nostartfiles \
           -fno-omit-frame-pointer -mcmodel=medany \
           -march=rv64imac -mabi=lp64
ASFLAGS := -march=rv64imac -mabi=lp64
LDFLAGS := -T linker.ld -nostdlib -Wl,--no-warn-rwx-segments

SRCS := src/start.S src/uart.c src/util.c src/vuln.c

# Optional: build with GCC's stack protector instrumentation.
#   make SP=1
ifeq ($(SP),1)
CFLAGS += -fstack-protector-strong -DUSE_STACK_PROTECTOR
SRCS  += src/stack_protector.c
endif


OBJS := $(SRCS:.c=.o)
OBJS := $(OBJS:.S=.o)

all: program.elf program.img

program.elf: $(OBJS) linker.ld
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $@

program.img: program.elf
	$(OBJCOPY) $< -O binary $@

dump: program.elf
	$(OBJDUMP) -d $< | less

nm: program.elf
	$(NM) -n $< | less

run: program.img
	qemu-system-riscv64 -M virt -bios none -serial stdio -display none -kernel program.img

# GDB debugging (two-terminal workflow):
#   Terminal 1:  make qemu-gdb
#   Terminal 2:  make gdb
# To debug with payload input:
#   Terminal 1:  make qemu-gdb-input
#   Terminal 2:  make gdb
# If port 1234 is in use (shared server), override: make qemu-gdb GDB_PORT=2345
GDB_PORT ?= 1234

qemu-gdb: program.img
	qemu-system-riscv64 -M virt -bios none -serial stdio -display none -kernel program.img -S -gdb tcp::$(GDB_PORT)

qemu-gdb-input: program.img
	qemu-system-riscv64 -M virt -bios none -serial stdio -display none -kernel program.img -S -gdb tcp::$(GDB_PORT) < payload.bin

gdb: program.elf
	$(TOOLPREFIX)gdb -q $< -ex "target remote :$(GDB_PORT)"

clean:
	rm -f $(OBJS) program.elf program.img
