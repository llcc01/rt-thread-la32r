/* SPDX-License-Identifier: GPL-2.0 */
/*
 * asmmacro.h: Assembler macros to make things easier to read.
 *
 * Copyright (C) 2020-2021 Loongson Technology Corporation Limited
 */
#ifndef _ASM_ASMMACRO_32_H
#define _ASM_ASMMACRO_32_H

#include "regdef.h"
#include "fpregdef.h"
#include "csrdef.h"
#include "ptrace.h"

	.macro	cpu_save_nonscratch thread
	st.w	s0, \thread, PT_R23
	st.w	s1, \thread, PT_R24
	st.w	s2, \thread, PT_R25
	st.w	s3, \thread, PT_R26
	st.w	s4, \thread, PT_R27
	st.w	s5, \thread, PT_R28
	st.w	s6, \thread, PT_R29
	st.w	s7, \thread, PT_R30
	st.w	s8, \thread, PT_R31
	st.w	sp, \thread, PT_R3
	st.w	fp, \thread, PT_R22
	.endm

	.macro	cpu_restore_nonscratch thread
	ld.w	s0, \thread, PT_R23
	ld.w	s1, \thread, PT_R24
	ld.w	s2, \thread, PT_R25
	ld.w	s3, \thread, PT_R26
	ld.w	s4, \thread, PT_R27
	ld.w	s5, \thread, PT_R28
	ld.w	s6, \thread, PT_R29
	ld.w	s7, \thread, PT_R30
	ld.w	s8, \thread, PT_R31
	ld.w	sp, \thread, PT_R3
	ld.w	fp, \thread, PT_R22
	ld.w	ra, \thread, PT_R1
	.endm

#endif /* _ASM_ASMMACRO_32_H */
