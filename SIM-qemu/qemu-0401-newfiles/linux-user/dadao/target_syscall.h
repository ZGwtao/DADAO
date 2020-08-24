/*
 * This struct defines the way the registers are stored on the
 *  stack during a system call.
 */

#ifndef LINUX_USER_DADAO_TARGET_SYSCALL_H
#define LINUX_USER_DADAO_TARGET_SYSCALL_H

struct target_pt_regs {
	abi_long __rg[64];
	abi_long __rp[64];
	abi_long __rf[64];
	abi_long __rv[64];
};

#define UNAME_MACHINE "WUXING"
#define UNAME_MINIMUM_RELEASE "5.0.0"

#define TARGET_MINSIGSTKSZ 2048
#define TARGET_MLOCKALL_MCL_CURRENT 1
#define TARGET_MLOCKALL_MCL_FUTURE  2

#endif
