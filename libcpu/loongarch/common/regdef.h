/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (C) 2020-2021 Loongson Technology Corporation Limited
 */
#ifndef _ASM_REGDEF_H
#define _ASM_REGDEF_H

#define REG_ZERO 0
#define REG_RA 1
#define REG_TP 2
#define REG_SP 3
#define REG_A0 4
#define REG_A1 5
#define REG_A2 6
#define REG_A3 7
#define REG_A4 8
#define REG_A5 9
#define REG_A6 10
#define REG_A7 11
#define REG_T0 12
#define REG_T1 13
#define REG_T2 14
#define REG_T3 15
#define REG_T4 16
#define REG_T5 17
#define REG_T6 18
#define REG_T7 19
#define REG_T8 20
#define REG_X0 21
#define REG_FP 22
#define REG_S0 23
#define REG_S1 24
#define REG_S2 25
#define REG_S3 26
#define REG_S4 27
#define REG_S5 28
#define REG_S6 29
#define REG_S7 30
#define REG_S8 31

#ifdef __ASSEMBLY__

#define zero	$r0	/* wired zero */
#define ra	$r1	/* return address */
#define tp	$r2
#define sp	$r3	/* stack pointer */
#define v0	$r4	/* return value - caller saved */
#define v1	$r5
#define a0	$r4	/* argument registers */
#define a1	$r5
#define a2	$r6
#define a3	$r7
#define a4	$r8
#define a5	$r9
#define a6	$r10
#define a7	$r11
#define t0	$r12	/* caller saved */
#define t1	$r13
#define t2	$r14
#define t3	$r15
#define t4	$r16
#define t5	$r17
#define t6	$r18
#define t7	$r19
#define t8	$r20
#define x0	$r21
#define fp	$r22	/* frame pointer */
#define s0	$r23	/* callee saved */
#define s1	$r24
#define s2	$r25
#define s3	$r26
#define s4	$r27
#define s5	$r28
#define s6	$r29
#define s7	$r30
#define s8	$r31

#endif /* __ASSEMBLY__ */

#endif /* _ASM_REGDEF_H */
