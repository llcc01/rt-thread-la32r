/* SPDX-License-Identifier: GPL-2.0+ WITH Linux-syscall-note */
/*
 * Author: Hanlu Li <lihanlu@loongson.cn>
 *         Huacai Chen <chenhuacai@loongson.cn>
 *
 * Copyright (C) 2020-2021 Loongson Technology Corporation Limited
 */
#ifndef _UAPI_ASM_PTRACE_H
#define _UAPI_ASM_PTRACE_H



/* For PTRACE_{POKE,PEEK}USR. 0 - 31 are GPRs, 32 is PC, 33 is BADVADDR. */
#define GPR_BASE	0
#define GPR_NUM		32
#define GPR_END		(GPR_BASE + GPR_NUM - 1)
#define PC		(GPR_END + 1)
#define BADVADDR	(GPR_END + 2)

#ifndef __ASSEMBLY__

#include <rtthread.h>

struct pt_regs {
	/* Saved main processor registers. */
	unsigned long regs[32];

	/* Saved special registers. */
	unsigned long csr_crmd;
	unsigned long csr_prmd;
	unsigned long csr_euen;
	unsigned long csr_ecfg;
	unsigned long csr_estat;
	unsigned long csr_epc;
	unsigned long csr_badvaddr;
	unsigned long orig_a0;
	unsigned long __last[0];
} __attribute__((aligned(8)));

#endif /* !__ASSEMBLY__ */

#define PT_SIZE (LONGSIZE * (32+9))

#define PT_R0       (0 * LONGSIZE)    /* 0 */
#define PT_R1       ((PT_R0) + LONGSIZE)    /* 1 */
#define PT_R2       ((PT_R1) + LONGSIZE)    /* 2 */
#define PT_R3       ((PT_R2) + LONGSIZE)    /* 3 */
#define PT_R4       ((PT_R3) + LONGSIZE)    /* 4 */
#define PT_R5       ((PT_R4) + LONGSIZE)    /* 5 */
#define PT_R6       ((PT_R5) + LONGSIZE)    /* 6 */
#define PT_R7       ((PT_R6) + LONGSIZE)    /* 7 */
#define PT_R8       ((PT_R7) + LONGSIZE)    /* 8 */
#define PT_R9       ((PT_R8) + LONGSIZE)    /* 9 */
#define PT_R10      ((PT_R9) + LONGSIZE)    /* 10 */
#define PT_R11      ((PT_R10) + LONGSIZE)   /* 11 */
#define PT_R12      ((PT_R11) + LONGSIZE)   /* 12 */
#define PT_R13      ((PT_R12) + LONGSIZE)   /* 13 */
#define PT_R14      ((PT_R13) + LONGSIZE)   /* 14 */
#define PT_R15      ((PT_R14) + LONGSIZE)   /* 15 */
#define PT_R16      ((PT_R15) + LONGSIZE)   /* 16 */
#define PT_R17      ((PT_R16) + LONGSIZE)   /* 17 */
#define PT_R18      ((PT_R17) + LONGSIZE)   /* 18 */
#define PT_R19      ((PT_R18) + LONGSIZE)   /* 19 */
#define PT_R20      ((PT_R19) + LONGSIZE)   /* 20 */
#define PT_R21      ((PT_R20) + LONGSIZE)   /* 21 */
#define PT_R22      ((PT_R21) + LONGSIZE)   /* 22 */
#define PT_R23      ((PT_R22) + LONGSIZE)   /* 23 */
#define PT_R24      ((PT_R23) + LONGSIZE)   /* 24 */
#define PT_R25      ((PT_R24) + LONGSIZE)   /* 25 */
#define PT_R26      ((PT_R25) + LONGSIZE)   /* 26 */
#define PT_R27      ((PT_R26) + LONGSIZE)   /* 27 */
#define PT_R28      ((PT_R27) + LONGSIZE)   /* 28 */
#define PT_R29      ((PT_R28) + LONGSIZE)   /* 29 */
#define PT_R30      ((PT_R29) + LONGSIZE)   /* 30 */
#define PT_R31      ((PT_R30) + LONGSIZE)   /* 31 */

#define PT_CRMD     ((PT_R31) + LONGSIZE)   /* 32 */
#define PT_PRMD     ((PT_CRMD) + LONGSIZE)  /* 33 */
#define PT_EUEN     ((PT_PRMD) + LONGSIZE)  /* 34 */
#define PT_ECFG     ((PT_EUEN) + LONGSIZE)  /* 35 */
#define PT_ESTAT    ((PT_ECFG) + LONGSIZE)  /* 36 */
#define PT_EPC      ((PT_ESTAT) + LONGSIZE) /* 37 */
#define PT_BADVADDR ((PT_EPC) + LONGSIZE)   /* 38 */
#define PT_ORIG_A0  ((PT_BADVADDR) + LONGSIZE) /* 39 */


#endif /* _UAPI_ASM_PTRACE_H */
