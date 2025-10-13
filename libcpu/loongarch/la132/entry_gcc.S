#define __ASSEMBLY__

#include "regdef.h"
#include "csrdef.h"
#include "start.h"
#include "rtconfig.h"



.globl _RAM1
_RAM1 = RAM1_BASE
.globl _RAM2
_RAM2 = RAM2_BASE

.text
.section ".startup", "ax"
.globl _start
_start:
    /* copy data section */
        move     t1 , zero 
        b    LoopCopyDataInit
CopyDataInit:
        la.abs   t3, _sidata
        add.w    t4, t3, t1  
        ld.w     t3, t4, 0x0
        add.w    t4, t0, t1  
        st.w     t3, t4, 0x0
        addi.w   t1, t1, 0x4
LoopCopyDataInit:
        la.abs   t0, _sdata
        la.abs   t3, _edata
        add.w    t2, t0, t1
        bne      t2, t3, CopyDataInit

    /* clear bss */
    la.abs      t0, __bss_start
    la.abs      t1, __bss_end
LoopFillZerobss:
    st.w    zero, t0, 0
    addi.w  t0, t0, 4
    blt     t0, t1, LoopFillZerobss

// set up basic system function
        /* cpu init*/
cpu_init_start:
// reset normal exception base and Ex config
        la.abs    t0, general_exception
        csrwr     t0, csr_EENTRY

        li.w      t0, (0<<S_CSR_ECFG_VS)|(0x0000<<S_CSR_ECFG_IM) // 4 instruction gap
        csrwr     t0, CSR_ECFG

        // set int masks, all disabled
        li.w      t0, 0x0
        csrxchg t0, t0, CSR_ECFG

    // set VSEG4(0x80000000~0x9fffffff) to cached, other VSEG to uncached
        li.w      t0, CACHE_OP
        csrwr     t0, CSR_SEGCA

    // set VSEG4(0x80000000~0x9fffffff) to PSEG0(0x00000000~0x1fffffff), other VSEG unchanged
        li.w      t0, ADDR_MAP
        csrwr     t0, CSR_SEGPA

    // set DA=0(change DA-mode to AD-mode)
        li.w      t0, 0x8
        csrxchg   zero, t0, CSR_CRMD

        la.abs     sp, _system_stack

        move     x0, zero

	    // bl      main
        /* jump to RT-Thread kernel */
        bl      rtthread_startup

        /* never reach here */
        jirl      zero, ra, 0

        .globl cpu_wait
cpu_wait:
        idle 0
        jr      ra


//             .org 0x1000
// DEFAULT_INT_HANDLER:
// 	SAVE_REGS_FAST(REGS_MEM)
//         csrrd   t0, CSR_ESTAT
//         andi    t1, t0, INT_VECTOR # t1 = t0 & 0b1011_1011_1100
//         beqz    t1, exception_core_check
// exception_pmu:
//         andi    t1,t0,0x4
//         bnez    t1,wake_label
//         andi    t1,t0,0x8
//         bnez    t1,touch_label
//         andi    t1,t0,0x10
//         bnez    t1,uart2_label
//         andi    t1,t0,0x20
//         bnez    t1,bcc_label
//         andi    t1,t0,0x80
//         bnez    t1,exint_label
//         andi    t1,t0,0x800
//         bnez    t1,timer_label
// wake_label:
//         # bl      TIMER_WAKE_INT 
//         b       exception_exit
// touch_label:
//         # bl      TOUCH
//         b       exception_exit
// uart2_label:
//         # bl      UART2_INT
//         b       exception_exit
// bcc_label:
//         # bl      BAT_FAIL
//         b       exception_exit
// exint_label:
//         # bl      ext_handler
//         b       exception_exit
// timer_label:
//         # bl      TIMER_HANDLER
//         b       exception_exit
// exception_core_check:
//         andi    t1, t0, INTC_VECTOR # t1 = t0 & 0b100_0000
//         beqz    t1, exception_exit
//         # bl      intc_handler
//         b       exception_exit
  
// exception_exit:
// 	RESTORE_REGS_FAST(REGS_MEM)
//         ertn
