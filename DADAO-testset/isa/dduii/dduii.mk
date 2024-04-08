#=======================================================================
# Makefrag for dduii tests
#-----------------------------------------------------------------------

dduii_sc_tests = \
	orrr-and \
	orrr-orr \
	orrr-xor \
	orrr-add \
	orrr-sub \
	orrr-cmp \
	orrr-cmps \
	orrr-cmpu \
	rrii-addi-rd \
	rrii-addi-rb \
	rrii-andi \
	rrii-cmpu \
	rrii-cmps \
	setzw \
	setow \
	andnw \
	orw \
	add \
	adrp \
	divs \
	divu \
	muls \
	mulu \
	sub \
	csn \
	csz \
	cseq \
	csne \
	csp \
	shlu \
	shru \
	shrs \
	rd2rd \
	rb2rb \
	rd2rb \
	rb2rd \
	exts \
	extz \

dduii_bare_tests = $(addprefix dduii-bare-, $(dduii_sc_tests))
dduii_qemu_tests = $(addprefix dduii-qemu-, $(dduii_sc_tests))
#dduii_v_tests = $(addprefix dduii-v-, $(dduii_sc_tests))
