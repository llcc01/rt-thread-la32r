#ifndef _TOOLS_ASM_H_
#define _TOOLS_ASM_H_

#include "start.h"

#define  PRINT_STR(string)      \
        la      a0, 1f        ; \
        .section .rodata       ; \
1:      .asciz string         ; \
        .text                 ; \
        bl      outputstring

#define  PRINT_WORD(rs)         \
        or      a0, $r0, rs    ; \
        bl      outputaddr 

#define REG_READ_W(addr, rd)    \
        la      t0, addr      ; \
        ld.w    rd, t0, 0
#define REG_READ_H(addr, rd)    \
        la      t0, addr      ; \
        ld.h    rd, t0, 0
#define REG_READ_B(addr, rd)    \
        la      t0, addr      ; \
        ld.b    rd, t0, 0

#define REG_WRITE_W(addr, rs)   \
        la      t0, addr      ; \
        st.w    rs, t0, 0
#define REG_WRITE_H(addr, rs)   \
        la      t0, addr      ; \
        st.h    rs, t0, 0
#define REG_WRITE_B(addr, rs)   \
        la      t0, addr      ; \
        st.b    rs, t0, 0
/* write imm */        
#define REG_WRITE_Wi(addr, imm) \
        la      t0, addr      ; \
        li.w      AT, imm       ; \
        st.w    AT, t0, 0
#define REG_WRITE_Hi(addr, imm) \
        la      t0, addr      ; \
        li.w      AT, imm       ; \
        st.h    AT, t0, 0
#define REG_WRITE_Bi(addr, imm) \
        la      t0, addr      ; \
        li.w      AT, imm       ; \
        st.b    AT, t0, 0
        
#define PUSH_W(rs)              \
        addi.w  sp, sp, -4    ; \
        st.w    rs, sp, 0

#define PUSH_2W(a, b)           \
        addi.w  sp, sp, -8    ; \
        st.w    a, (sp), 0x0  ; \
        st.w    b, (sp), 0x4

#define PUSH_3W(a, b, c)        \
        addiu   sp, sp, -12   ; \
        st.w    a, sp, 0x0    ; \
        st.w    b, sp, 0x4    ; \
        st.w    c, sp, 0x8

#define POP_W(rd)               \
        ld.w    rd, sp, 0     ; \
        addi.w  sp, sp, 4

#define POP_2W(a, b)            \
        ld.w    a, sp, 0x0    ; \
        ld.w    b, sp, 0x4    ; \
        addi.w  sp, sp, 8

#define POP_3W(a, b, c)         \
        ld.w    a, sp, 0x0    ; \
        ld.w    b, sp, 0x4    ; \
        ld.w    c, sp, 0x8    ; \
        addi.w  sp, sp, 12


#define RET()                   \
        lw      ra, sp, 0x0   ; \
        addi.w  sp, sp, 4     ; \
        jr      ra


#define WRITE_CMREG32(reg, BASE, OFFSET, reg_temp) \
        li.w    reg_temp, BASE+OFFSET; \
        st.w  reg, reg_temp, 0
#define WRITE_CMREG8(reg, BASE, OFFSET, reg_temp) \
        li.w    reg_temp, BASE+OFFSET; \
        st.b  reg, reg_temp, 0
#define READ_CMREG32(reg, BASE, OFFSET, reg_temp) \
        li.w    reg_temp, BASE+OFFSET; \
        ld.w  reg, reg_temp, 0
#define READ_CMREG8(reg, BASE, OFFSET, reg_temp) \
        li.w    reg_temp, BASE+OFFSET; \
        ld.b  reg, reg_temp, 0


#define SAVE_REGS_ALL(BASE) \
	csrwr   tp, CSR_KScratch1; \
        li.w    tp, BASE;\
        st.w    t0, tp, -0x4 ;\
        st.w    t1, tp, -0x8 ;\
        st.w    t2, tp, -0xc ;\
        st.w    t3, tp, -0x10;\
        st.w    t4, tp, -0x14;\
        st.w    t5, tp, -0x18;\
        st.w    t6, tp, -0x1c;\
        st.w    t7, tp, -0x20;\
        st.w    t8, tp, -0x24;\
        st.w    s0, tp, -0x28;\
        st.w    s1, tp, -0x2c;\
        st.w    s2, tp, -0x30;\
        st.w    s3, tp, -0x34;\
        st.w    s4, tp, -0x38;\
        st.w    s5, tp, -0x3c;\
        st.w    s6, tp, -0x40;\
        st.w    s7, tp, -0x44;\
        st.w    s8, tp, -0x48;\
        st.w    a0, tp, -0x4c;\
        st.w    a1, tp, -0x50;\
        st.w    a2, tp, -0x54;\
        st.w    a3, tp, -0x58;\
        st.w    a4, tp, -0x5c;\
        st.w    a5, tp, -0x60;\
        st.w    a6, tp, -0x64;\
        st.w    a7, tp, -0x68;\
        st.w    ra, tp, -0x6c;\
        st.w    sp, tp, -0x70;\
        st.w    gp, tp, -0x74;\
        st.w    fp, tp, -0x78;\
	addi.w  t0, tp, 0    ;\
	csrrd   tp, CSR_KScratch1; \
	st.w    tp, t0, -0x7c;


#define RESTORE_REGS_ALL(BASE) \
        li.w    tp, BASE;\
        ld.w    t0, tp, -0x4 ;\
        ld.w    t1, tp, -0x8 ;\
        ld.w    t2, tp, -0xc ;\
        ld.w    t3, tp, -0x10;\
        ld.w    t4, tp, -0x14;\
        ld.w    t5, tp, -0x18;\
        ld.w    t6, tp, -0x1c;\
        ld.w    t7, tp, -0x20;\
        ld.w    t8, tp, -0x24;\
        ld.w    s0, tp, -0x28;\
        ld.w    s1, tp, -0x2c;\
        ld.w    s2, tp, -0x30;\
        ld.w    s3, tp, -0x34;\
        ld.w    s4, tp, -0x38;\
        ld.w    s5, tp, -0x3c;\
        ld.w    s6, tp, -0x40;\
        ld.w    s7, tp, -0x44;\
        ld.w    s8, tp, -0x48;\
        ld.w    a0, tp, -0x4c;\
        ld.w    a1, tp, -0x50;\
        ld.w    a2, tp, -0x54;\
        ld.w    a3, tp, -0x58;\
        ld.w    a4, tp, -0x5c;\
        ld.w    a5, tp, -0x60;\
        ld.w    a6, tp, -0x64;\
        ld.w    a7, tp, -0x68;\
        ld.w    ra, tp, -0x6c;\
        ld.w    sp, tp, -0x70;\
        ld.w    gp, tp, -0x74;\
        ld.w    fp, tp, -0x78;\
	ld.w    tp, tp, -0x7c;

//in future, SAVE/RESTORE_REGS_FAST can no care s0~s8
#define SAVE_REGS_FAST(BASE) \
	csrwr   tp, CSR_KScratch1; \
        li.w    tp, BASE;\
        st.w    t0, tp, -0x4 ;\
        st.w    t1, tp, -0x8 ;\
        st.w    t2, tp, -0xc ;\
        st.w    t3, tp, -0x10;\
        st.w    t4, tp, -0x14;\
        st.w    t5, tp, -0x18;\
        st.w    t6, tp, -0x1c;\
        st.w    t7, tp, -0x20;\
        st.w    t8, tp, -0x24;\
        /*st.w    s0, tp, -0x28;*/\
        /*st.w    s1, tp, -0x2c;*/\
        /*st.w    s2, tp, -0x30;*/\
        /*st.w    s3, tp, -0x34;*/\
        /*st.w    s4, tp, -0x38;*/\
        /*st.w    s5, tp, -0x3c;*/\
        /*st.w    s6, tp, -0x40;*/\
        /*st.w    s7, tp, -0x44;*/\
        /*st.w    s8, tp, -0x48;*/\
        st.w    a0, tp, -0x4c;\
        st.w    a1, tp, -0x50;\
        st.w    a2, tp, -0x54;\
        st.w    a3, tp, -0x58;\
        st.w    a4, tp, -0x5c;\
        st.w    a5, tp, -0x60;\
        st.w    a6, tp, -0x64;\
        st.w    a7, tp, -0x68;\
        st.w    ra, tp, -0x6c;\
        st.w    sp, tp, -0x70;\
        /*st.w    gp, tp, -0x74;*/\
        /*st.w    fp, tp, -0x78;*/\


#define RESTORE_REGS_FAST(BASE) \
        li.w    tp, BASE;\
        ld.w    t0, tp, -0x4 ;\
        ld.w    t1, tp, -0x8 ;\
        ld.w    t2, tp, -0xc ;\
        ld.w    t3, tp, -0x10;\
        ld.w    t4, tp, -0x14;\
        ld.w    t5, tp, -0x18;\
        ld.w    t6, tp, -0x1c;\
        ld.w    t7, tp, -0x20;\
        ld.w    t8, tp, -0x24;\
        /*ld.w    s0, tp, -0x28;*/\
        /*ld.w    s1, tp, -0x2c;*/\
        /*ld.w    s2, tp, -0x30;*/\
        /*ld.w    s3, tp, -0x34;*/\
        /*ld.w    s4, tp, -0x38;*/\
        /*ld.w    s5, tp, -0x3c;*/\
        /*ld.w    s6, tp, -0x40;*/\
        /*ld.w    s7, tp, -0x44;*/\
        /*ld.w    s8, tp, -0x48;*/\
        ld.w    a0, tp, -0x4c;\
        ld.w    a1, tp, -0x50;\
        ld.w    a2, tp, -0x54;\
        ld.w    a3, tp, -0x58;\
        ld.w    a4, tp, -0x5c;\
        ld.w    a5, tp, -0x60;\
        ld.w    a6, tp, -0x64;\
        ld.w    a7, tp, -0x68;\
        ld.w    ra, tp, -0x6c;\
        ld.w    sp, tp, -0x70;\
        /*ld.w    gp, tp, -0x74;*/\
        /*ld.w    fp, tp, -0x78;*/\
	csrrd   tp, CSR_KScratch1;


#define SET_SEG(seg, plv3, phy, cca)       \
    Save_two_regs(t0,t1);                   \
    li.w      t0  , ((plv3&1)<<3|(phy&7))<<(seg*4);\
    li.w      t1  , 0xf<<(seg*4);      \
    csrxchg t0  , t1 , CSR_SEGPA;\
    li.w      t0  , (cca&0xf)<<(seg*4);\
    li.w      t1  , 0xf<<(seg*4);      \
    csrxchg t0  , t1 , CSR_SEGCA;\
    Restore_two_regs(t0,t1)

#define SET_MPU(num, base, calg, plv3, plv0, mask) \
    Save_one_reg(t0);                   \
    li.w      t0  , (((base)&0xfffff000)|(calg&0xf)<<4|(plv3&1)<<3|(plv0&1));\
    csrwr   t0  , CSR_MPU0_BASE|num<<2; \
    li.w      t0  , ((mask)&0xfffff000);\
    csrwr   t0  , CSR_MPU0_MASK|num<<2; \
    Restore_one_reg(t0)

//return a0: 0, no; 1: implement.
#define CHECK_MPU_IMPL(num) \
    li.w      a0, -1; \
    csrwr   a0, CSR_MPU0_BASE|num<<2; \
    csrrd   a0, CSR_MPU0_BASE|num<<2; \
//Save & Restore
#define Save_one_reg(reg0) \
    addi.w  sp  , sp, -4;\
    st.w    reg0, sp,  0

#define Restore_one_reg(reg0) \
    ld.w    reg0, sp,  0 ;\
    addi.w  sp  , sp,  4

#define Save_two_regs(reg0,reg1) \
    addi.w  sp  , sp,-8;\
    st.w    reg0, sp, 0;\
    st.w    reg1, sp, 4

#define Restore_two_regs(reg0,reg1) \
    ld.w    reg0, sp, 0;\
    ld.w    reg1, sp, 4;\
    addi.w  sp  , sp, 8


#define Save_four_regs(reg0,reg1,reg2,reg3) \
    addi.w  sp  , sp,-16;\
    st.w    reg0, sp,  0;\
    st.w    reg1, sp,  4;\
    st.w    reg2, sp,  8;\
    st.w    reg3, sp, 12

#define Restore_four_regs(reg0,reg1,reg2,reg3) \
    ld.w    reg0, sp,  0;\
    ld.w    reg1, sp,  4;\
    ld.w    reg2, sp,  8;\
    ld.w    reg3, sp, 12;\
    addi.w  sp  , sp, 16

#endif /* _TOOLS_ASM_H_ */
