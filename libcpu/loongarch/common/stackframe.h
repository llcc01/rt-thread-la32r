/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (C) 2020-2021 Loongson Technology Corporation Limited
 */
#ifndef _ASM_STACKFRAME_H
#define _ASM_STACKFRAME_H

#include "asm.h"
// #include "asmmacro.h"
#include "regdef.h"
#include "csrdef.h"
#include "ptrace.h"

/* Make the addition of cfi info a little easier. */

	.macro cfi_st reg offset=0
	LONG_S	\reg, sp, \offset
	.endm

	.macro cfi_ld reg offset=0
	LONG_L	\reg, sp, \offset
	.endm

	.macro BACKUP_T0T1
	csrwr	t0, CSR_KScratch0
	csrwr	t1, CSR_KScratch1
	.endm

	.macro RELOAD_T0T1
	csrrd   t0, CSR_KScratch0
	csrrd   t1, CSR_KScratch1
	.endm

	.macro	SAVE_TEMP
	RELOAD_T0T1
	cfi_st	t0, PT_R12
	cfi_st	t1, PT_R13
	cfi_st	t2, PT_R14
	cfi_st	t3, PT_R15
	cfi_st	t4, PT_R16
	cfi_st	t5, PT_R17
	cfi_st	t6, PT_R18
	cfi_st	t7, PT_R19
	cfi_st	t8, PT_R20
	.endm

	.macro	SAVE_STATIC
	cfi_st	s0, PT_R23
	cfi_st	s1, PT_R24
	cfi_st	s2, PT_R25
	cfi_st	s3, PT_R26
	cfi_st	s4, PT_R27
	cfi_st	s5, PT_R28
	cfi_st	s6, PT_R29
	cfi_st	s7, PT_R30
	cfi_st	s8, PT_R31
	.endm

/*
 * get_saved_sp returns the SP for the current CPU by looking in the
 * kernelsp array for it.  If tosp is set, it stores the current sp in
 * t0 and loads the new value in sp.  If not, it clobbers t0 and
 * stores the new value in t1, leaving sp unaffected.
 */
// #ifdef CONFIG_SMP
// 	/* SMP variation */
// 	.macro	get_saved_sp docfi=0 tosp=0
// 	csrrd	t0, PERCPU_BASE_KS
// 	la.abs	t1, kernelsp
// 	LONG_ADDU	t1, t1, t0
// 	.if \tosp
// 	or	t0, sp, zero
// 	LONG_L	sp, t1, 0
// 	.endif
// 	.endm

// 	.macro	set_saved_sp stackp temp temp2
// 	la.abs	\temp, kernelsp
// 	LONG_ADDU	\temp, \temp, x0
// 	LONG_S	\stackp, \temp, 0
// 	.endm
// #else /* !CONFIG_SMP */
// 	/* Uniprocessor variation */
// 	.macro	get_saved_sp docfi=0 tosp=0
// 	la.abs	t1, kernelsp
// 	.if \tosp
// 	move	t0, sp
// 	.if \docfi
// 	.cfi_register sp, t0
// 	.endif
// 	LONG_L	sp, t1, 0
// 	.else
// 	LONG_L	t1, t1, 0
// 	.endif
// 	.endm

// 	.macro	set_saved_sp stackp temp temp2
// 	la.abs	\temp, kernelsp
// 	LONG_S	\stackp, \temp, 0
// 	.endm
// #endif

	.macro	SAVE_SOME
	move	t0, sp
	PTR_ADDIU sp, sp, -PT_SIZE
	cfi_st	t0, PT_R3
	LONG_S	zero, sp, PT_R0
	csrrd	t0, CSR_PRMD
	LONG_S	t0, sp, PT_PRMD
	csrrd	t0, CSR_CRMD
	LONG_S	t0, sp, PT_CRMD
	csrrd	t0, CSR_ExConfig
	LONG_S	t0, sp, PT_ECFG
	csrrd	t0, CSR_CU
	LONG_S  t0, sp, PT_EUEN
	cfi_st	ra, PT_R1
	cfi_st	a0, PT_R4
	cfi_st	a1, PT_R5
	cfi_st	a2, PT_R6
	cfi_st	a3, PT_R7
	cfi_st	a4, PT_R8
	cfi_st	a5, PT_R9
	cfi_st	a6, PT_R10
	cfi_st	a7, PT_R11
	csrrd	ra, CSR_EPC
	LONG_S	ra, sp, PT_EPC
	cfi_st	tp, PT_R2
	cfi_st  x0, PT_R21
	cfi_st	fp, PT_R22
	.endm

	.macro	SAVE_ALL
	SAVE_SOME
	SAVE_TEMP
	SAVE_STATIC
	.endm

	.macro	RESTORE_TEMP
	cfi_ld	t0, PT_R12
	cfi_ld	t1, PT_R13
	cfi_ld	t2, PT_R14
	cfi_ld	t3, PT_R15
	cfi_ld	t4, PT_R16
	cfi_ld	t5, PT_R17
	cfi_ld	t6, PT_R18
	cfi_ld	t7, PT_R19
	cfi_ld	t8, PT_R20
	.endm

	.macro	RESTORE_STATIC
	cfi_ld	s0, PT_R23
	cfi_ld	s1, PT_R24
	cfi_ld	s2, PT_R25
	cfi_ld	s3, PT_R26
	cfi_ld	s4, PT_R27
	cfi_ld	s5, PT_R28
	cfi_ld	s6, PT_R29
	cfi_ld	s7, PT_R30
	cfi_ld	s8, PT_R31
	.endm

	.macro	RESTORE_SOME
	LONG_L	v0, sp, PT_PRMD
	csrwr	v0, CSR_PRMD
	LONG_L	v0, sp, PT_EPC
	csrwr	v0, CSR_EPC
	cfi_ld	ra, PT_R1
	cfi_ld	a0, PT_R4
	cfi_ld	a1, PT_R5
	cfi_ld	a2, PT_R6
	cfi_ld	a3, PT_R7
	cfi_ld	a4, PT_R8
	cfi_ld	a5, PT_R9
	cfi_ld	a6, PT_R10
	cfi_ld	a7, PT_R11
	cfi_ld	tp, PT_R2
	cfi_ld  x0, PT_R21
	cfi_ld	fp, PT_R22
	.endm

	.macro	RESTORE_SP
	cfi_ld	sp, PT_R3
	.endm

	.macro	RESTORE_SP_AND_RET
	RESTORE_SP
	ertn
	.endm

	.macro	RESTORE_ALL
	RESTORE_TEMP
	RESTORE_STATIC
	RESTORE_SOME
	RESTORE_SP
	.endm

	.macro	RESTORE_ALL_AND_RET
	RESTORE_ALL
	ertn
	.endm

/* Move to kernel mode and disable interrupts. */
	.macro	CLI
	li.w	t0, 0x7
	csrxchg	zero, t0, CSR_CRMD
	csrrd	x0, PERCPU_BASE_KS
	.endm

/* Move to kernel mode and enable interrupts. */
	.macro	STI
	li.w	t0, 0x7
	li.w	t1, (1 << 2)
	csrxchg	t1, t0, CSR_CRMD
	csrrd	x0, PERCPU_BASE_KS
	.endm

/* Just move to kernel mode and leave interrupts as they are. */
	.macro	KMODE
	csrrd	x0, PERCPU_BASE_KS
	.endm

#endif /* _ASM_STACKFRAME_H */
