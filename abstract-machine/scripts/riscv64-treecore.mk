include $(AM_HOME)/scripts/isa/riscv64.mk
include $(AM_HOME)/scripts/platform/treecore.mk
LDFLAGS += --defsym=_pmem_start=0x80000000
