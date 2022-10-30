#ifndef __TEST_MACROS_SCALAR_H
#define __TEST_MACROS_SCALAR_H

#-----------------------------------------------------------------------
# Helper macros
#-----------------------------------------------------------------------

#define TEST_CASE( testnum, testreg, correctval, code... )	\
test_ ## testnum:						\
	move	TESTNUM, testnum;				\
	code;							\
	move	rd7, correctval;				\
	cmps	rd7, testreg, rd7;				\
	brnz	rd7, fail;

#-----------------------------------------------------------------------
# DADAO MACROS
#-----------------------------------------------------------------------

#-----------------------------------------------------------------------
# Tests for an instruction with register-register operands
#-----------------------------------------------------------------------

#define TEST_ORRR_OP( testnum, inst, result, val1, val2 )		\
    TEST_CASE( testnum, rd14, result,					\
	move	rd1, val1;						\
	move	rd2, val2;						\
	inst	rd14, rd1, rd2;						\
    )

#define TEST_ORRR_SRC1_EQ_DEST( testnum, inst, result, val1, val2 )	\
    TEST_CASE( testnum, rd1, result,					\
	move	rd1, val1;						\
	move	rd2, val2;						\
	inst	rd1, rd1, rd2;						\
    )

#define TEST_ORRR_SRC2_EQ_DEST( testnum, inst, result, val1, val2 )	\
    TEST_CASE( testnum, rd2, result,					\
	move	rd1, val1;						\
	move	rd2, val2;						\
	inst	rd2, rd1, rd2;						\
    )

#define TEST_ORRR_SRC12_EQ_DEST( testnum, inst, result, val1 )		\
    TEST_CASE( testnum, rd1, result,					\
	move	rd1, val1;						\
	inst	rd1, rd1, rd1;						\
    )

#define TEST_ORRR_DEST_BYPASS( testnum, swym_cycles, inst, result, val1, val2 )	\
    TEST_CASE( testnum, rd6, result,					\
	move	rd4, 0;							\
1:	move	rd1, val1;						\
	move	rd2, val2;						\
	inst	rd14, rd1, rd2;						\
	.rept	swym_cycles						\
		swym;							\
	.endr								\
	rd2rd	rd6, rd14, 0;						\
	addi	rd4, 1;							\
	cmps	rd5, rd4, 2;						\
	brnz	rd5, 1b							\
    )

#define TEST_ORRR_SRC12_BYPASS( testnum, src1_swyms, src2_swyms, inst, result, val1, val2 ) \
    TEST_CASE( testnum, rd14, result,					\
	move	rd4, 0;							\
1:	move	rd1, val1;						\
	.rept	src1_swyms						\
		swym;							\
	.endr								\
	move	rd2, val2;						\
	.rept	src2_swyms						\
		swym;							\
	.endr								\
	inst	rd14, rd1, rd2;						\
	addi	rd4, 1;							\
	cmps	rd5, rd4, 2;						\
	brnz	rd5, 1b							\
    )

#define TEST_ORRR_SRC21_BYPASS( testnum, src1_swyms, src2_swyms, inst, result, val1, val2 ) \
    TEST_CASE( testnum, rd14, result,					\
	move	rd4, 0;							\
1:	move	rd2, val1;						\
	.rept	src1_swyms						\
		swym;							\
	.endr								\
	move	rd1, val2;						\
	.rept	src2_swyms						\
		swym;							\
	.endr								\
	inst	rd14, rd1, rd2;						\
	addi	rd4, 1;							\
	cmps	rd5, rd4, 2;						\
	brnz	rd5, 1b							\
    )

#define TEST_ORRR_ZEROSRC1( testnum, inst, result, val )		\
    TEST_CASE( testnum, rd2, result,					\
	move	rd1, val;						\
	inst	rd2, rd0, rd1;						\
    )

#define TEST_ORRR_ZEROSRC2( testnum, inst, result, val )		\
    TEST_CASE( testnum, rd2, result,					\
	move	rd1, val;						\
	inst	rd2, rd1, rd0;						\
    )

#define TEST_ORRR_ZEROSRC12( testnum, inst, result )			\
    TEST_CASE( testnum, rd1, result,					\
	inst	rd1, rd0, rd0;						\
    )

#define TEST_ORRR_ZERODEST( testnum, inst, val1, val2 )			\
    TEST_CASE( testnum, rd0, 0,						\
	move	rd1, val1;						\
	move	rd2, val2;						\
	inst	rd0, rd1, rd2;						\
    )

#-----------------------------------------------------------------------
# Pass and fail code (assumes test num is in TESTNUM)
#-----------------------------------------------------------------------

#define TEST_PASSFAIL						\
	brnz  TESTNUM, pass;					\
fail:								\
	DDTEST_FAIL;						\
pass:								\
	DDTEST_PASS;						\

#-----------------------------------------------------------------------
# Test data section
#-----------------------------------------------------------------------

#define TEST_DATA

#endif /* __TEST_MACROS_SCALAR_H */
