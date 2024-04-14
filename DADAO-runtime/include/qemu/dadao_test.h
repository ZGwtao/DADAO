#ifndef _RUNTIME_QEMU_DDTEST_H_
#define _RUNTIME_QEMU_DDTEST_H_

#include "simrisc/ddtest.h"
#include "simrisc/encoding.h"

#define DDTEST_DDUII							\
	.text;								\
FAIL:									\
	.ascii  "FAIL\n";						\
	.equ FLEN, .-FAIL;						\
	.type FAIL, object;						\
	.size FAIL, FLEN;						\
	.text;								\
	.align 4;							\
PASS:									\
	.ascii  "PASS";							\
	.equ PLEN, .-PASS;						\
	.type PASS, object;						\
	.size PASS, PLEN;						\
	.text;								\
	.align 4;

#define DDTEST_CODE_BEGIN						\
	.text;								\
	.globl _start;							\
	.align 4;							\
_start:									\
	setrd	RD_FLAG, 0;					\
	setrd	RD_NUMR, 0;					\
	setrd	RD_PASS, 0;					\
	setrd	RD_EXP1, 0;					\
	setrd	RD_EXP2, 0;					\
	setrd	RD_RET1, 0;					\
	setrd	RD_RET2, 0;

#define DDTEST_CODE_END							\
	brnz	RD_FLAG, ___fail;					\
	/* SHOULD handle pass first */				\
	setrd	rd16, 1;							\
	setrd	rd17, PASS;							\
	setrd	rd18, PLEN;							\
	setrd	rd15, 64; /* write */				\
	trap	cp0, 0;								\
	setrd	rd16, 0;							\
	setrd	rd15, 93; /* exit */				\
	trap	cp0, 0;								\
___fail:										\
	setrd	rd16, 1;							\
	setrd	rd17, FAIL;							\
	setrd	rd18, FLEN;							\
	setrd	rd15, 64; /* write */				\
	trap	cp0, 0;								\
	rd2rd	rd16, TESTNUM, 1;					\
	setrd	rd15, 93; /* exit */				\
	trap	cp0, 0;

#define DDTEST_DATA_BEGIN						\
	.section data;								\

#define DDTEST_DATA_END

#endif
