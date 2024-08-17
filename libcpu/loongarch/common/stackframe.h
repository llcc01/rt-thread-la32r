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

	// .macro BACKUP_T0T1
	// csrwr	t0, CSR_SAVE0
	// csrwr	t1, CSR_SAVE1
	// .endm

	// .macro RELOAD_T0T1
	// csrrd   t0, CSR_SAVE0
	// csrrd   t1, CSR_SAVE1
	// .endm

	.macro	SAVE_TEMP
	// RELOAD_T0T1
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

	.macro	SAVE_SOME
	move	t0, sp
	addi.w	t0, t0, PT_SIZE
	cfi_st	t0, PT_R3
	cfi_st	zero, PT_R0

	csrrd	t0, CSR_CRMD
	cfi_st	t0, PT_CRMD
	csrrd	t0, CSR_PRMD
	cfi_st	t0, PT_PRMD
	csrrd	t0, CSR_EUEN
	cfi_st  t0, PT_EUEN
	csrrd	t0, CSR_ECFG
	cfi_st	t0, PT_ECFG
	csrrd   t0, CSR_ESTAT
	cfi_st	t0, PT_ESTAT
	csrrd   t0, CSR_ERA
	cfi_st	t0, PT_ERA
	csrrd   t0, CSR_BADV
	cfi_st	t0, PT_BADV

	cfi_st	ra, PT_R1
	cfi_st	a0, PT_R4
	cfi_st	a1, PT_R5
	cfi_st	a2, PT_R6
	cfi_st	a3, PT_R7
	cfi_st	a4, PT_R8
	cfi_st	a5, PT_R9
	cfi_st	a6, PT_R10
	cfi_st	a7, PT_R11
	cfi_st	tp, PT_R2
	cfi_st  x0, PT_R21
	cfi_st	fp, PT_R22
	.endm

	.macro	SAVE_ALL
	PTR_ADDIU sp, sp, -PT_SIZE
	SAVE_TEMP
	SAVE_SOME
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

	.macro	RESTORE_SOME prmd=0

	.if \prmd
	cfi_ld	t0, PT_CRMD
	csrwr	t0, CSR_CRMD
	cfi_ld	t0, PT_PRMD
	csrwr	t0, CSR_PRMD
	.endif

	cfi_ld	t0, PT_ERA
	csrwr	t0, CSR_ERA

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

	cfi_ld	fp, PT_R22
	.endm

	.macro	RESTORE_SP
	// cfi_ld	sp, PT_R3
	PTR_ADDIU sp, sp, PT_SIZE
	.endm

	.macro	RESTORE_SP_AND_RET
	RESTORE_SP
	ertn
	.endm

	.macro	RESTORE_ALL prmd=0
	RESTORE_SOME \prmd
	RESTORE_STATIC
	RESTORE_TEMP
	RESTORE_SP
	.endm

	.macro	RESTORE_ALL_AND_RET prmd=0
	RESTORE_ALL \prmd
	ertn
	.endm

// /* Move to kernel mode and disable interrupts. */
// 	.macro	CLI
// 	li.w	t0, 0x7
// 	csrxchg	zero, t0, CSR_CRMD
// 	csrrd	x0, PERCPU_BASE_KS
// 	.endm

// /* Move to kernel mode and enable interrupts. */
// 	.macro	STI
// 	li.w	t0, 0x7
// 	li.w	t1, (1 << 2)
// 	csrxchg	t1, t0, CSR_CRMD
// 	csrrd	x0, PERCPU_BASE_KS
// 	.endm

// /* Just move to kernel mode and leave interrupts as they are. */
// 	.macro	KMODE
// 	csrrd	x0, PERCPU_BASE_KS
// 	.endm

#endif /* _ASM_STACKFRAME_H */
