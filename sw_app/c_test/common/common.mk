
COMMON_DIR := $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))

COMMON_SRCS = $(wildcard $(COMMON_DIR)/*.c)
INCS := -I$(COMMON_DIR) $(PROGRAM_INCS)

# ARCH = rv32im # to disable compressed instructions
ARCH ?= rv32im

ifdef PROGRAM
PROGRAM_C := $(PROGRAM).c
endif

SRCS = $(COMMON_SRCS) $(PROGRAM_C) $(EXTRA_SRCS)

C_SRCS = $(filter %.c, $(SRCS))
ASM_SRCS = $(filter %.S, $(SRCS))

CC = riscv32-unknown-elf-gcc          # riscv64-unknown-elf-gcc

OBJCOPY ?= $(subst gcc,objcopy,$(wordlist 1,1,$(CC)))
OBJDUMP ?= $(subst gcc,objdump,$(wordlist 1,1,$(CC)))

LINKER_SCRIPT ?= $(COMMON_DIR)/link.ld
CRT ?= $(COMMON_DIR)/crt0.S
CFLAGS ?= -march=$(ARCH) -mabi=ilp32 -static -mcmodel=medany -Wall -O2\
	-fvisibility=hidden -nostdlib -nostartfiles -ffreestanding $(PROGRAM_CFLAGS)

OBJS := ${C_SRCS:.c=.o} ${ASM_SRCS:.S=.o} ${CRT:.S=.o}
DEPS = $(OBJS:%.o=%.d)

ifdef PROGRAM
OUTFILES := $(PROGRAM).elf $(PROGRAM).vmem $(PROGRAM).bin $(PROGRAM).dump
else
OUTFILES := $(OBJS)
endif

all: $(OUTFILES)

ifdef PROGRAM
$(PROGRAM).elf: $(OBJS) $(LINKER_SCRIPT)
	$(CC) $(CFLAGS) -T $(LINKER_SCRIPT) $(OBJS) -o $@ $(LIBS)

.PHONY: disassemble
disassemble: $(PROGRAM).dis
endif

%.dis: %.elf
	$(OBJDUMP) -fhSD $^ > $@


%.vmem: %.bin
	srec_cat $^ -binary -offset 0x0000 -byte-swap 4 -o $@ -vmem   # 0x00100000

%.dump: %.elf
	$(OBJDUMP) --disassemble-all $< > $@

%.bin: %.elf
	$(OBJCOPY) -O binary $^ $@

%.o: %.c
	$(CC) $(CFLAGS) -MMD -c $(INCS) -o $@ $<

%.o: %.S
	$(CC) $(CFLAGS) -MMD -c $(INCS) -o $@ $<

clean:
	$(RM) -f $(OBJS) $(DEPS) $(OUTFILES)

distclean: clean
	$(RM) -f $(OUTFILES)
