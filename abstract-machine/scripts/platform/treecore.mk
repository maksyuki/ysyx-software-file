AM_SRCS := treecore/trm.c \
           treecore/ioe/ioe.c \
           treecore/ioe/timer.c \
           treecore/ioe/input.c \
           treecore/ioe/gpu.c \
           treecore/ioe/audio.c \
           treecore/isa/$(ISA)/cte.c \
           treecore/isa/$(ISA)/trap.S \
           treecore/isa/$(ISA)/vme.c \
           treecore/mpe.c \
           treecore/isa/$(ISA)/start.S

CFLAGS    += -fdata-sections -ffunction-sections
LDFLAGS   += -T $(AM_HOME)/scripts/platform/treecore.ld --defsym=_entry_offset=0x100000
LDFLAGS   += --gc-sections -e _start
NEMUFLAGS += -b -l $(shell dirname $(IMAGE).elf)/treecore-log.txt $(IMAGE).bin

CFLAGS += -DMAINARGS=\"$(mainargs)\"
CFLAGS += -I$(AM_HOME)/am/src/treecore/include
.PHONY: $(AM_HOME)/am/src/treecore/trm.c

image: $(IMAGE).elf
	@$(OBJDUMP) -d $(IMAGE).elf > $(IMAGE).txt
	@echo + OBJCOPY "->" $(IMAGE_REL).bin
	@$(OBJCOPY) -S --set-section-flags .bss=alloc,contents -O binary $(IMAGE).elf $(IMAGE).bin

run: image
	$(MAKE) -C $(NEMU_HOME) ISA=$(ISA) run ARGS="$(NEMUFLAGS)"

gdb: image
	$(MAKE) -C $(NEMU_HOME) ISA=$(ISA) gdb ARGS="$(NEMUFLAGS)"
