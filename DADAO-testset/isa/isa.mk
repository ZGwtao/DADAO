#=======================================================================
# Makefile for DADAO-testset/isa
#-----------------------------------------------------------------------

src_dir ?= .

include $(src_dir)/dduii/dduii.mk
include $(src_dir)/ddfii/ddfii.mk

default: all

vpath %.S $(src_dir)

#------------------------------------------------------------
# Build assembly tests

%.dump: %
	$(DADAO_ELF_OBJDUMP) $(DADAO_ELF_OBJDUMP_FLAGS) $< > $@

dduii-bare%.out: dduii-bare%
	@echo -n "	$<  	"
	@make -C $(DIR_DADAO_TOP) RUNTIME_BARE_BINARY=$(shell pwd)/$< runtime-bare-run-binary	> /dev/null
	@tail -n 1 $(src_dir)/output/chipyard.TestHarness.$(RUNTIME_BARE_CONFIG)/$@

dduii-qemu%.out: dduii-qemu%
	@echo -n "	$<  	"
	$(DADAO_QEMU_USER) $< 2> $@
	@echo

ddfii-bare%.out: ddfii-bare%
	@echo -n "	$<  	"
	@make -C $(DIR_DADAO_TOP) RUNTIME_BARE_BINARY=$(shell pwd)/$< runtime-bare-run-binary   > /dev/null
	@tail -n 1 $(src_dir)/output/chipyard.TestHarness.$(RUNTIME_BARE_CONFIG)/$@

ddfii-qemu%.out: ddfii-qemu%
	@echo -n "	$<  	"
	$(DADAO_QEMU_USER) $< 2> $@
	@echo


define compile_template

$$($(1)_bare_tests): $(1)-bare-%: $(1)/%.S
	$$(DADAO_ELF_GCC) $(2) $$(DADAO_ELF_GCC_FLAGS) -I$(DIR_DADAO_TARGET)/include -I$(DIR_DADAO_TARGET)/include/bare -I$(src_dir)/macros/scalar -T$(DIR_DADAO_TARGET)/include/bare/link.ld $$< -o $$@
$(1)_tests += $$($(1)_bare_tests)

$$($(1)_qemu_tests): $(1)-qemu-%: $(1)/%.S
	$$(DADAO_ELF_GCC) $(2) $$(DADAO_ELF_GCC_FLAGS) -I$(DIR_DADAO_TARGET)/include -I$(DIR_DADAO_TARGET)/include/qemu -I$(src_dir)/macros/scalar -T$(DIR_DADAO_TARGET)/include/qemu/link.ld $$< -o $$@
$(1)_tests += $$($(1)_qemu_tests)

$(1)_tests_dump = $$(addsuffix .dump, $$($(1)_tests))

$(1): $$($(1)_tests_dump)

$(1)_bare_tests_dump = $$(addsuffix .dump, $$($(1)_bare_tests))

$(1)-bare: $$($(1)_bare_tests_dump)

$(1)_qemu_tests_dump = $$(addsuffix .dump, $$($(1)_qemu_tests))

$(1)-qemu: $$($(1)_qemu_tests_dump)

.PHONY: $(1)

COMPILER_SUPPORTS_$(1) := $$(shell $$(DADAO_ELF_GCC) $(2) -c -x c /dev/null -o /dev/null 2> /dev/null; echo $$$$?)

ifeq ($$(COMPILER_SUPPORTS_$(1)),0)
tests += $$($(1)_tests)
endif

endef

$(eval $(call compile_template,dduii, -Xassembler --multiple-to-single ))
$(eval $(call compile_template,ddfii, -Xassembler --multiple-to-single ))

tests_dump = $(addsuffix .dump, $(tests))
tests_hex = $(addsuffix .hex, $(tests))
dduii_bare_out = $(addsuffix .out, $(filter dduii-bare%,$(tests)))
dduii_qemu_out = $(addsuffix .out, $(filter dduii-qemu%,$(tests)))
ddfii_bare_out = $(addsuffix .out, $(filter ddfii-bare%,$(tests)))
ddfii_qemu_out = $(addsuffix .out, $(filter ddfii-qemu%,$(tests)))

run-bare: $(dduii_bare_out) $(ddfii_bare_out)
run-qemu: $(dduii_qemu_out) $(ddfii_qemu_out)


junk += $(tests) $(tests_dump) $(tests_hex) $(dduii_bare_out) $(dduii_qemu_out) $(ddfii_bare_out) $(ddfii_qemu_out)

#------------------------------------------------------------
# Default

all: $(tests_dump)

#------------------------------------------------------------
# Clean up

clean:
	rm -rf $(junk)
