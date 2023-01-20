
DIR_DADAO_INSTALL		?= ~/DADAO/__install/

DADAO_ELF_PREFIX		?= $(DIR_DADAO_INSTALL)/bin/dadao-unknown-elf-
DADAO_ELF_GAS			:= $(DADAO_ELF_PREFIX)as
DADAO_ELF_READELF		:= $(DADAO_ELF_PREFIX)readelf
DADAO_ELF_OBJDUMP		:= $(DADAO_ELF_PREFIX)objdump
DADAO_ELF_OBJCOPY		:= $(DADAO_ELF_PREFIX)objcopy
DADAO_ELF_GCC			:= $(DADAO_ELF_PREFIX)gcc

DADAO_GNU_PREFIX		?= $(DIR_DADAO_INSTALL)/bin/dadao-linux-gnu-
DADAO_GNU_GAS			:= $(DADAO_GNU_PREFIX)as
DADAO_GNU_READELF		:= $(DADAO_GNU_PREFIX)readelf
DADAO_GNU_OBJDUMP		:= $(DADAO_GNU_PREFIX)objdump
DADAO_GNU_OBJCOPY		:= $(DADAO_GNU_PREFIX)objcopy
DADAO_GNU_GCC			:= $(DADAO_GNU_PREFIX)gcc

DADAO_QEMU_USER			?= $(DIR_DADAO_INSTALL)/bin/qemu-dadao
DADAO_QEMU_SYS			?= $(DIR_DADAO_INSTALL)/bin/qemu-system-dadao

DADAO_ELF_LINK_FLAGS		?= -static -nostdlib -nostartfiles $(DADAO_ELF_LINK_EXTRA_FLAGS)
DADAO_ELF_GCC_FLAGS		?= -static -fvisibility=hidden $(DADAO_ELF_GCC_EXTRA_FLAGS) $(DADAO_ELF_LINK_FLAGS)
DADAO_ELF_OBJDUMP_FLAGS		?= --disassemble-all --disassemble-zeroes --section=.text --section=.text.startup --section=.text.init --section=.data
