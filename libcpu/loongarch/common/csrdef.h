#ifndef _CSRDEF_H
#define _CSRDEF_H

#define CSR_CRMD 0x0
#define CSR_PRMD 0x1
#define CSR_EUEN 0x2
#define CSR_MISC 0x3
#define CSR_ECFG 0x4
#define CSR_ESTAT 0x5
#define CSR_ERA 0x6
#define CSR_BADV 0x7
#define CSR_BADI 0x8
#define csr_EENTRY 0xc
#define CSR_ASID 0x18
#define CSR_CPUID 0x20
#define CSR_PRCFG1 0x21

#define CSR_SAVE0 0x30 // save t0
#define CSR_SAVE1 0x31 // save t1
// #define CSR_SAVE2 0x32 // save kernel sp
// #define CSR_SAVE3 0x33

#define CSR_TID 0x40
#define CSR_TCFG 0x41
#define CSR_TVAL 0x42
#define CSR_CNTC 0x43
#define CSR_TICLR 0x44
#define CSR_LLBCTL 0x60
#define CSR_GSConfig 0x80
#define CSR_ERRCTL 0x90
#define CSR_ERRINFO 0x91
#define CSR_ERRINFO1 0x92
#define CSR_ERRBase 0x93
#define CSR_ERREPC 0x94
#define CSR_CCSF 0x96  // only-132e
#define CSR_TAGLO 0x98
#define CSR_USERDEF 0xaf     // only-132e
#define CSR_ELUSION 0xff     // only-132e
#define CSR_MPU0_BASE 0x180  // only-132e
#define CSR_MPU0_MASK 0x181
#define CSR_MPU1_BASE 0x184
#define CSR_MPU1_MASK 0x185
#define CSR_MPU2_BASE 0x188
#define CSR_MPU2_MASK 0x189
#define CSR_MPU3_BASE 0x18c
#define CSR_MPU3_MASK 0x18d
#define CSR_SEGPA 0x190
#define CSR_SEGCA 0x191
#define CSR_DBConfig 0x300
#define CSR_DBState 0x301
#define CSR_DBA 0x310
#define CSR_DBM 0x311
#define CSR_DBC 0x312
#define CSR_DBT 0x313
#define CSR_IBConfig 0x380
#define CSR_IBState 0x381
#define CSR_IBA 0x390
#define CSR_IBM 0x391
#define CSR_IBC 0x392
#define CSR_IBT 0x393
#define CSR_DEBUG 0x500
#define CSR_DESAVE 0x501
#define CSR_DEPC 0x502
#define CSR_DEBUG2 0x508  // only-132e

// S_* menas shift
// W_* menas width
// M_* menas mask

// 0x0 CSR_CRMD
#define S_CSR_CRMD_PLV 0
#define W_CSR_CRMD_PLV 2
#define M_CSR_CRMD_PLV (((1 << W_CSR_CRMD_PLV) - 1) << S_CSR_CRMD_PLV)
#define S_CSR_CRMD_IE 2
#define W_CSR_CRMD_IE 1
#define M_CSR_CRMD_IE (((1 << W_CSR_CRMD_IE) - 1) << S_CSR_CRMD_IE)
#define S_CSR_CRMD_DA 3
#define W_CSR_CRMD_DA 1
#define M_CSR_CRMD_DA (((1 << W_CSR_CRMD_DA) - 1) << S_CSR_CRMD_DA)
#define S_CSR_CRMD_PG 4
#define W_CSR_CRMD_PG 1
#define M_CSR_CRMD_PG (((1 << W_CSR_CRMD_PG) - 1) << S_CSR_CRMD_PG)
#define S_CSR_CRMD_DACF 5
#define W_CSR_CRMD_DACF 2
#define M_CSR_CRMD_DACF (((1 << W_CSR_CRMD_DACF) - 1) << S_CSR_CRMD_DACF)
#define S_CSR_CRMD_DACM 7
#define W_CSR_CRMD_DACM 2
#define M_CSR_CRMD_DACM (((1 << W_CSR_CRMD_DACM) - 1) << S_CSR_CRMD_DACM)
#define S_CSR_CRMD_WE 9
#define W_CSR_CRMD_WE 1
#define M_CSR_CRMD_WE (((1 << W_CSR_CRMD_WE) - 1) << S_CSR_CRMD_WE)

// 0x1 CSR_PRMD
#define S_CSR_PRMD_PPLV 0
#define W_CSR_PRMD_PPLV 2
#define M_CSR_PRMD_PPLV (((1 << W_CSR_PRMD_PPLV) - 1) << S_CSR_PRMD_PPLV)
#define S_CSR_PRMD_PIE 2
#define M_CSR_PRMD_PIE 0x4
#define S_CSR_PRMD_PWE 3
#define M_CSR_PRMD_PWE 0x8

// 0x2
#define S_CSR_EUEN_FPen 0
#define S_CSR_EUEN_LSXen 1
#define S_CSR_EUEN_LASXen 2
#define S_CSR_EUEN_LBTen 3

// 0x3
#define S_CSR_MISC_DisRDT1 5
#define S_CSR_MISC_DisRDT2 6
#define S_CSR_MISC_DisRDT3 7
#define S_CSR_MISC_ALC0 12
#define S_CSR_MISC_ALC1 13
#define S_CSR_MISC_ALC2 14
#define S_CSR_MISC_ALC3 15
#define W_CSR_MISC_ALC0 1
#define W_CSR_MISC_ALC1 1
#define W_CSR_MISC_ALC2 1
#define W_CSR_MISC_ALC3 1
#define M_CSR_MISC_Short 0xf
#define M_CSR_MISC_ALC0 (((1 << W_CSR_MISC_ALC0) - 1) << S_CSR_MISC_ALC0)
#define M_CSR_MISC_ALC1 (((1 << W_CSR_MISC_ALC1) - 1) << S_CSR_MISC_ALC1)
#define M_CSR_MISC_ALC2 (((1 << W_CSR_MISC_ALC2) - 1) << S_CSR_MISC_ALC2)
#define M_CSR_MISC_ALC3 (((1 << W_CSR_MISC_ALC3) - 1) << S_CSR_MISC_ALC3)
#define S_CSR_MISC_NWP0 16
#define S_CSR_MISC_NWP1 17
#define S_CSR_MISC_NWP2 18

// 0x4
#define S_CSR_ECFG_IM 0
#define M_CSR_ECFG_IM 0x1fff
#define S_CSR_ECFG_IM0 0
#define S_CSR_ECFG_IM1 1
#define S_CSR_ECFG_IM2 2
#define S_CSR_ECFG_IM3 3
#define S_CSR_ECFG_IM4 4
#define S_CSR_ECFG_IM5 5
#define S_CSR_ECFG_IM6 6
#define S_CSR_ECFG_IM7 7
#define S_CSR_ECFG_IM8 8
#define S_CSR_ECFG_IM9 9
#define S_CSR_ECFG_IM10 10
#define S_CSR_ECFG_IM11 11
#define S_CSR_ECFG_IM13 13
#define S_CSR_ECFG_IM12 12
#define S_CSR_ECFG_IM_SW0 0
#define S_CSR_ECFG_IM_SW1 1
#define S_CSR_ECFG_IM_HW0 2
#define S_CSR_ECFG_IM_HW1 3
#define S_CSR_ECFG_IM_HW2 4
#define S_CSR_ECFG_IM_HW3 5
#define S_CSR_ECFG_IM_HW4 6
#define S_CSR_ECFG_IM_HW5 7
#define S_CSR_ECFG_IM_HW6 8
#define S_CSR_ECFG_IM_HW7 9
#define S_CSR_ECFG_IM_PCOV 10
#define S_CSR_ECFG_IM_TI 11
#define S_CSR_ECFG_IM_IPI 12
#define S_CSR_ECFG_IM_NMI 13

#define S_CSR_ECFG_VS 16
#define W_CSR_ECFG_VS 3
#define M_CSR_ECFG_VS (((1 << W_CSR_ECFG_VS) - 1) << S_CSR_ECFG_VS)

// 0x5 CSR_ESTAT
#define S_CSR_ESTAT_IS 0
#define W_CSR_ESTAT_IS 15
#define M_CSR_ESTAT_IS (((1 << W_CSR_ESTAT_IS) - 1) << S_CSR_ESTAT_IS)
#define S_CSR_ESTAT_IS0 0
#define M_CSR_ESTAT_IS0 (1 << S_CSR_ESTAT_IS0)
#define S_CSR_ESTAT_IS1 1
#define M_CSR_ESTAT_IS1 (1 << S_CSR_ESTAT_IS1)
#define S_CSR_ESTAT_IS2 2
#define M_CSR_ESTAT_IS2 (1 << S_CSR_ESTAT_IS2)
#define S_CSR_ESTAT_IS3 3
#define M_CSR_ESTAT_IS3 (1 << S_CSR_ESTAT_IS3)
#define S_CSR_ESTAT_IS4 4
#define M_CSR_ESTAT_IS4 (1 << S_CSR_ESTAT_IS4)
#define S_CSR_ESTAT_IS5 5
#define M_CSR_ESTAT_IS5 (1 << S_CSR_ESTAT_IS5)
#define S_CSR_ESTAT_IS6 6
#define M_CSR_ESTAT_IS6 (1 << S_CSR_ESTAT_IS6)
#define S_CSR_ESTAT_IS7 7
#define M_CSR_ESTAT_IS7 (1 << S_CSR_ESTAT_IS7)
#define S_CSR_ESTAT_IS8 8
#define M_CSR_ESTAT_IS8 (1 << S_CSR_ESTAT_IS8)
#define S_CSR_ESTAT_IS9 9
#define M_CSR_ESTAT_IS9 (1 << S_CSR_ESTAT_IS9)
#define S_CSR_ESTAT_IS10 10
#define M_CSR_ESTAT_IS10 (1 << S_CSR_ESTAT_IS10)
#define S_CSR_ESTAT_IS11 11
#define M_CSR_ESTAT_IS11 (1 << S_CSR_ESTAT_IS11)
#define S_CSR_ESTAT_IS12 12
#define M_CSR_ESTAT_IS12 (1 << S_CSR_ESTAT_IS12)
#define S_CSR_ESTAT_IS13 13
#define M_CSR_ESTAT_IS13 (1 << S_CSR_ESTAT_IS13)
#define S_CSR_ESTAT_SW0 0
#define M_CSR_ESTAT_SW0 (1 << S_CSR_ESTAT_SW0)
#define S_CSR_ESTAT_SW1 1
#define M_CSR_ESTAT_SW1 (1 << S_CSR_ESTAT_SW1)
#define S_CSR_ESTAT_HW0 2
#define M_CSR_ESTAT_HW0 (1 << S_CSR_ESTAT_HW0)
#define S_CSR_ESTAT_HW1 3
#define M_CSR_ESTAT_HW1 (1 << S_CSR_ESTAT_HW1)
#define S_CSR_ESTAT_HW2 4
#define M_CSR_ESTAT_HW2 (1 << S_CSR_ESTAT_HW2)
#define S_CSR_ESTAT_HW3 5
#define M_CSR_ESTAT_HW3 (1 << S_CSR_ESTAT_HW3)
#define S_CSR_ESTAT_HW4 6
#define M_CSR_ESTAT_HW4 (1 << S_CSR_ESTAT_HW4)
#define S_CSR_ESTAT_HW5 7
#define M_CSR_ESTAT_HW5 (1 << S_CSR_ESTAT_HW5)
#define S_CSR_ESTAT_HW6 8
#define M_CSR_ESTAT_HW6 (1 << S_CSR_ESTAT_HW6)
#define S_CSR_ESTAT_HW7 9
#define M_CSR_ESTAT_HW7 (1 << S_CSR_ESTAT_HW7)
#define S_CSR_ESTAT_PCOV 10
#define M_CSR_ESTAT_PCOV (1 << S_CSR_ESTAT_PCOV)
#define S_CSR_ESTAT_TI 11
#define M_CSR_ESTAT_TI (1 << S_CSR_ESTAT_TI)
#define S_CSR_ESTAT_IPI 12
#define M_CSR_ESTAT_IPI (1 << S_CSR_ESTAT_IPI)
#define S_CSR_ESTAT_NMI 13
#define M_CSR_ESTAT_NMI (1 << S_CSR_ESTAT_NMI)

#define S_CSR_ESTAT_Ecode 16
#define W_CSR_ESTAT_Ecode 6
#define M_CSR_ESTAT_Ecode \
  (((1 << W_CSR_ESTAT_Ecode) - 1) << S_CSR_ESTAT_Ecode)
#define S_CSR_ESTAT_EsubCode 22
#define W_CSR_ESTAT_EsubCode 9
#define M_CSR_ESTAT_EsubCode \
  (((1 << W_CSR_ESTAT_EsubCode) - 1) << S_CSR_ESTAT_EsubCode)

// 0x10 CSR_TLBIDX
#define S_CSR_TLBIDX_INDEX 0
#define W_CSR_TLBIDX_INDEX 12
#define M_CSR_TLBIDX_INDEX \
  (((1 << W_CSR_TLBIDX_INDEX) - 1) << S_CSR_TLBIDX_INDEX)
#define S_CSR_TLBIDX_PS 24
#define W_CSR_TLBIDX_PS 6
#define M_CSR_TLBIDX_PS (((1 << W_CSR_TLBIDX_PS) - 1) << S_CSR_TLBIDX_PS)
#define S_CSR_TLBIDX_V 31
#define W_CSR_TLBIDX_V 1
#define M_CSR_TLBIDX_V (((1 << W_CSR_TLBIDX_V) - 1) << S_CSR_TLBIDX_V)

// 0x11 CSR_TLBEHI
#define S_CSR_TLBEHI_VPN2 13
#define W_CSR_TLBEHI_VPN2 35
#define M_CSR_TLBEHI_VPN2 (((1 << W_CSR_TLBEHI_VPN2) - 1) << S_CSR_TLBEHI_VPN2)

// 0x15 CSR_GuestTLB
#define S_CSR_GuestTLB_GVTLB 0
#define W_CSR_GuestTLB_GVTLB 6
#define M_CSR_GuestTLB_GVTLB \
  (((1 << W_CSR_GuestTLB_GVTLB) - 1) << S_CSR_GuestTLB_GVTLB)
#define S_CSR_GuestTLB_use_rid 12
#define W_CSR_GuestTLB_use_rid 1
#define M_CSR_GuestTLB_use_rid \
  (((1 << W_CSR_GuestTLB_use_rid) - 1) << S_CSR_GuestTLB_use_rid)
#define S_CSR_GuestTLB_RID 16
#define W_CSR_GuestTLB_RID 8
#define M_CSR_GuestTLB_RID \
  (((1 << W_CSR_GuestTLB_RID) - 1) << S_CSR_GuestTLB_RID)

// 0x16 CSR_GTLBR
#define S_CSR_GTLBR_is_GTLB 0
#define W_CSR_GTLBR_is_GTLB 1
#define M_CSR_GTLBR_is_GTLB \
  (((1 << W_CSR_GTLBR_is_GTLB) - 1) << S_CSR_GTLBR_is_GTLB)
#define S_CSR_GTLBR_TLBR_RID 16
#define W_CSR_GTLBR_TLBR_RID 8
#define M_CSR_GTLBR_TLBR_RID \
  (((1 << W_CSR_GTLBR_TLBR_RID) - 1) << S_CSR_GTLBR_TLBR_RID)

// 0x18 CSR_ASID
#define S_CSR_ASID_ASID 0
#define W_CSR_ASID_ASID 10
#define M_CSR_ASID_ASID (((1 << W_CSR_ASID_ASID) - 1) << S_CSR_ASID_ASID)

// 0x41 CSR_TCFG
#define S_CSR_TCFG_En 0
#define S_CSR_TCFG_Period 1

// 0x50 CSR_GuestInfo
#define S_CSR_GuestInfo_PVM 1
#define W_CSR_GuestInfo_PVM 1
#define M_CSR_GuestInfo_PVM \
  (((1 << W_CSR_GuestInfo_PVM) - 1) << S_CSR_GuestInfo_PVM)
#define S_CSR_GuestInfo_GID 16
#define W_CSR_GuestInfo_GID 8
#define M_CSR_GuestInfo_GID \
  (((1 << W_CSR_GuestInfo_GID) - 1) << S_CSR_GuestInfo_GID)

// 0x51 CSR_GuestConfig
#define S_CSR_GuestConfig_GPERFNum 24
#define M_CSR_GuestConfig_GPERFNum 0x7000000

// 0x60
#define S_CSR_LLBCTL_ROLLB 0
#define S_CSR_LLBCTL_WCLLB 1
#define S_CSR_LLBCTL_KLO 2

// 0x80 CSR_GSConfig
#define S_CSR_GSConfig_Stfill 8
#define W_CSR_GSConfig_Stfill 1
#define M_CSR_GSConfig_Stfill \
  (((1 << W_CSR_GSConfig_Stfill) - 1) << S_CSR_GSConfig_Stfill)

// 0x81 CSR_Flush
#define M_CSR_Flush_VTLB 0x1
#define M_CSR_Flush_FTLB 0x2
#define M_CSR_Flush_DTLB 0x4
#define M_CSR_Flush_ITLB 0x8
#define M_CSR_Flush_BTAC 0x10

// 0x8a
#define S_CSR_RFEPC_IsTLBR 0
#define M_CSR_REEPC_EPC 0xfffffffffffffffc

// 0x8f
#define S_CSR_RFState_PPLV 0
#define S_CSR_RFState_PIE 2
#define S_CSR_RFState_PVM 3
#define S_CSR_RFState_PWE 4
#define M_CSR_RFState_PPLV 0x3
#define M_CSR_RFState_PIE 0x4
#define M_CSR_RFState_PVM 0x8
#define M_CSR_RFState_PWE 0x10

// 0x90
#define S_CSR_ERRCTL_ERRMode 0
#define S_CSR_ERRCTL_PPLV 2
#define S_CSR_ERRCTL_PIE 4
#define S_CSR_ERRCTL_PVM 5
#define S_CSR_ERRCTL_PWE 6
#define S_CSR_ERRCTL_PDA 7
#define S_CSR_ERRCTL_PPG 8
#define S_CSR_ERRCTL_PDACF 9
#define S_CSR_ERRCTL_PDACM 11
#define M_CSR_ERRCTL_PPLV 0xc
#define M_CSR_ERRCTL_PIE 0x10
#define M_CSR_ERRCTL_PVM 0x20
#define M_CSR_ERRCTL_PWE 0x40
#define M_CSR_ERRCTL_PDA 0x80
#define M_CSR_ERRCTL_PPG 0x100
#define M_CSR_ERRCTL_PDACF 0x600
#define M_CSR_ERRCTL_PDACM 0x1800

// 0xaf userdef
#define S_CSR_USERDEF_DC 4

// 0x200 CSR_PCCtl
#define S_CSR_PCCtl_EVENT 0
#define M_CSR_PCCtl_EVENT 0x3ff
#define S_CSR_PCCtl_PLV 16
#define M_CSR_PCCtl_PLV 0xf0000
#define S_CSR_PCCtl_GREN 21
#define M_CSR_PCCtl_GREN 0x600000

/*
 * The following macros are especially useful for __asm__
 * inline assembler.
 */
#ifndef __STR
#define __STR(x) #x
#endif
#ifndef STR
#define STR(x) __STR(x)
#endif

// #define __builtin_loongarch_csrrd_w __builtin_loongarch_csrrd
// #define __builtin_loongarch_csrwr_w __builtin_loongarch_csrwr
// #define __builtin_loongarch_csrxchg_w __builtin_loongarch_csrxchg

#endif /* _CSRDEF_H */
