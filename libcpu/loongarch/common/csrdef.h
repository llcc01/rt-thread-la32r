#ifndef _CSRDEF_H
#define _CSRDEF_H

#define CSR_CRMD          0x0   
#define CSR_PRMD          0x1   
#define CSR_CU            0x2   
#define CSR_Config        0x3   
#define CSR_ExConfig      0x4   
#define CSR_ExStatus      0x5   
#define CSR_EPC           0x6   
#define CSR_BadVAddr      0x7   
#define CSR_EBase         0xc   
#define CSR_ASID          0x18  
#define CSR_CPUN          0x20  
#define CSR_Config1       0x21  
#define CSR_KScratch0     0x30  
#define CSR_KScratch1     0x31  
#define CSR_TimerID       0x40  
#define CSR_TimerConfig   0x41  
#define CSR_TimerTicks    0x42  
#define CSR_TimerOffset   0x43  
#define CSR_TimerClear    0x44  
#define CSR_LLBit         0x60  
#define CSR_GSConfig      0x80  
#define CSR_ERRCTL        0x90  
#define CSR_ERRINFO       0x91  
#define CSR_ERRINFO1      0x92  
#define CSR_ERRBase       0x93  
#define CSR_ERREPC        0x94  
#define CSR_CCSF          0x96  //only-132e
#define CSR_TAGLO         0x98
#define CSR_USERDEF       0xaf  //only-132e
#define CSR_ELUSION       0xff  //only-132e
#define CSR_MPU0_BASE     0x180 //only-132e
#define CSR_MPU0_MASK     0x181 
#define CSR_MPU1_BASE     0x184
#define CSR_MPU1_MASK     0x185 
#define CSR_MPU2_BASE     0x188 
#define CSR_MPU2_MASK     0x189 
#define CSR_MPU3_BASE     0x18c 
#define CSR_MPU3_MASK     0x18d 
#define CSR_SEGPA         0x190
#define CSR_SEGCA         0x191 
#define CSR_DBConfig      0x300 
#define CSR_DBState       0x301 
#define CSR_DBA           0x310 
#define CSR_DBM           0x311 
#define CSR_DBC           0x312 
#define CSR_DBT           0x313 
#define CSR_IBConfig      0x380 
#define CSR_IBState       0x381 
#define CSR_IBA           0x390 
#define CSR_IBM           0x391 
#define CSR_IBC           0x392 
#define CSR_IBT           0x393 
#define CSR_DEBUG         0x500 
#define CSR_DESAVE        0x501 
#define CSR_DEPC          0x502 
#define CSR_DEBUG2        0x508 //only-132e

//S_* menas shift
//W_* menas width
//M_* menas mask

//0x0 CSR_CRMD
#define S_CSR_CRMD_PLV      0
#define W_CSR_CRMD_PLV      2
#define M_CSR_CRMD_PLV      (((1<<W_CSR_CRMD_PLV)-1)<<S_CSR_CRMD_PLV)
#define S_CSR_CRMD_IE       2
#define W_CSR_CRMD_IE       1
#define M_CSR_CRMD_IE       (((1<<W_CSR_CRMD_IE)-1)<<S_CSR_CRMD_IE)
#define S_CSR_CRMD_DA       3
#define W_CSR_CRMD_DA       1
#define M_CSR_CRMD_DA       (((1<<W_CSR_CRMD_DA)-1)<<S_CSR_CRMD_DA)
#define S_CSR_CRMD_PG       4
#define W_CSR_CRMD_PG       1
#define M_CSR_CRMD_PG       (((1<<W_CSR_CRMD_PG)-1)<<S_CSR_CRMD_PG)
#define S_CSR_CRMD_DACF     5
#define W_CSR_CRMD_DACF     2
#define M_CSR_CRMD_DACF     (((1<<W_CSR_CRMD_DACF)-1)<<S_CSR_CRMD_DACF)
#define S_CSR_CRMD_DACM     7
#define W_CSR_CRMD_DACM     2
#define M_CSR_CRMD_DACM     (((1<<W_CSR_CRMD_DACM)-1)<<S_CSR_CRMD_DACM)
#define S_CSR_CRMD_WE       9
#define W_CSR_CRMD_WE       1
#define M_CSR_CRMD_WE       (((1<<W_CSR_CRMD_WE)-1)<<S_CSR_CRMD_WE)

//0x1 CSR_PRMD
#define S_CSR_PRMD_PPLV     0
#define W_CSR_PRMD_PPLV     2
#define M_CSR_PRMD_PPLV     (((1<<W_CSR_PRMD_PPLV)-1)<<S_CSR_PRMD_PPLV)
#define S_CSR_PRMD_PIE      2
#define M_CSR_PRMD_PIE      0x4
#define S_CSR_PRMD_PWE      3
#define M_CSR_PRMD_PWE      0x8

//0x2
#define S_CSR_CU_FPen      0
#define S_CSR_CU_LSXen     1
#define S_CSR_CU_LASXen    2
#define S_CSR_CU_LBTen     3

//0x3  
#define S_CSR_Config_DisRDT1	5
#define S_CSR_Config_DisRDT2	6
#define S_CSR_Config_DisRDT3	7
#define S_CSR_Config_ALC0  12
#define S_CSR_Config_ALC1  13
#define S_CSR_Config_ALC2  14
#define S_CSR_Config_ALC3  15
#define W_CSR_Config_ALC0  1
#define W_CSR_Config_ALC1  1
#define W_CSR_Config_ALC2  1
#define W_CSR_Config_ALC3  1
#define M_CSR_Config_Short 0xf
#define M_CSR_Config_ALC0     (((1<<W_CSR_Config_ALC0)-1)<<S_CSR_Config_ALC0)
#define M_CSR_Config_ALC1     (((1<<W_CSR_Config_ALC1)-1)<<S_CSR_Config_ALC1)
#define M_CSR_Config_ALC2     (((1<<W_CSR_Config_ALC2)-1)<<S_CSR_Config_ALC2)
#define M_CSR_Config_ALC3     (((1<<W_CSR_Config_ALC3)-1)<<S_CSR_Config_ALC3)
#define S_CSR_Config_NWP0  16
#define S_CSR_Config_NWP1  17
#define S_CSR_Config_NWP2  18

//0x4
#define S_CSR_ExConfig_IM   0
#define M_CSR_ExConfig_IM   0x1fff
#define S_CSR_ExConfig_IM0  0
#define S_CSR_ExConfig_IM1  1
#define S_CSR_ExConfig_IM2  2
#define S_CSR_ExConfig_IM3  3
#define S_CSR_ExConfig_IM4  4
#define S_CSR_ExConfig_IM5  5
#define S_CSR_ExConfig_IM6  6
#define S_CSR_ExConfig_IM7  7
#define S_CSR_ExConfig_IM8  8
#define S_CSR_ExConfig_IM9  9
#define S_CSR_ExConfig_IM10 10
#define S_CSR_ExConfig_IM11 11
#define S_CSR_ExConfig_IM13 13
#define S_CSR_ExConfig_IM12 12
#define S_CSR_ExConfig_IM_SW0 0
#define S_CSR_ExConfig_IM_SW1 1
#define S_CSR_ExConfig_IM_HW0 2
#define S_CSR_ExConfig_IM_HW1 3
#define S_CSR_ExConfig_IM_HW2 4
#define S_CSR_ExConfig_IM_HW3 5
#define S_CSR_ExConfig_IM_HW4 6
#define S_CSR_ExConfig_IM_HW5 7
#define S_CSR_ExConfig_IM_HW6 8
#define S_CSR_ExConfig_IM_HW7 9
#define S_CSR_ExConfig_IM_PCOV 10
#define S_CSR_ExConfig_IM_TI   11
#define S_CSR_ExConfig_IM_IPI  12
#define S_CSR_ExConfig_IM_NMI  13

#define S_CSR_ExConfig_VS   16
#define W_CSR_ExConfig_VS   3
#define M_CSR_ExConfig_VS   (((1<<W_CSR_ExConfig_VS)-1)<<S_CSR_ExConfig_VS)

//0x5 CSR_ExStatus
#define S_CSR_ExStatus_IS       0
#define W_CSR_ExStatus_IS       15
#define S_CSR_ExStatus_IS0      0 
#define S_CSR_ExStatus_IS1      1 
#define S_CSR_ExStatus_IS2      2 
#define S_CSR_ExStatus_IS3      3 
#define S_CSR_ExStatus_IS4      4 
#define S_CSR_ExStatus_IS5      5 
#define S_CSR_ExStatus_IS6      6 
#define S_CSR_ExStatus_IS7      7 
#define S_CSR_ExStatus_IS8      8 
#define S_CSR_ExStatus_IS9      9 
#define S_CSR_ExStatus_IS10     10
#define S_CSR_ExStatus_IS11     11
#define S_CSR_ExStatus_IS12     12
#define S_CSR_ExStatus_IS13     13
#define S_CSR_ExStatus_SW0      0 
#define S_CSR_ExStatus_SW1      1 
#define S_CSR_ExStatus_HW0      2 
#define S_CSR_ExStatus_HW1      3 
#define S_CSR_ExStatus_HW2      4 
#define S_CSR_ExStatus_HW3      5 
#define S_CSR_ExStatus_HW4      6 
#define S_CSR_ExStatus_HW5      7 
#define S_CSR_ExStatus_HW6      8 
#define S_CSR_ExStatus_HW7      9 
#define S_CSR_ExStatus_PCOV     10
#define S_CSR_ExStatus_TI       11
#define S_CSR_ExStatus_IPI      12
#define S_CSR_ExStatus_NMI      13

#define S_CSR_ExStatus_Ecode    16
#define W_CSR_ExStatus_Ecode    6
#define S_CSR_ExStatus_EsubCode 22
#define W_CSR_ExStatus_EsubCode 9

//0x10 CSR_TLBIDX
#define S_CSR_TLBIDX_INDEX  0
#define W_CSR_TLBIDX_INDEX  12
#define M_CSR_TLBIDX_INDEX  (((1<<W_CSR_TLBIDX_INDEX)-1)<<S_CSR_TLBIDX_INDEX)
#define S_CSR_TLBIDX_PS     24
#define W_CSR_TLBIDX_PS     6
#define M_CSR_TLBIDX_PS     (((1<<W_CSR_TLBIDX_PS)-1)<<S_CSR_TLBIDX_PS)
#define S_CSR_TLBIDX_V      31
#define W_CSR_TLBIDX_V      1
#define M_CSR_TLBIDX_V      (((1<<W_CSR_TLBIDX_V)-1)<<S_CSR_TLBIDX_V)

//0x11 CSR_TLBEHI
#define S_CSR_TLBEHI_VPN2   13
#define W_CSR_TLBEHI_VPN2   35
#define M_CSR_TLBEHI_VPN2   (((1<<W_CSR_TLBEHI_VPN2)-1)<<S_CSR_TLBEHI_VPN2)

//0x15 CSR_GuestTLB
#define S_CSR_GuestTLB_GVTLB    0
#define W_CSR_GuestTLB_GVTLB    6
#define M_CSR_GuestTLB_GVTLB    (((1<<W_CSR_GuestTLB_GVTLB)-1)<<S_CSR_GuestTLB_GVTLB)
#define S_CSR_GuestTLB_use_rid  12
#define W_CSR_GuestTLB_use_rid  1
#define M_CSR_GuestTLB_use_rid  (((1<<W_CSR_GuestTLB_use_rid)-1)<<S_CSR_GuestTLB_use_rid)
#define S_CSR_GuestTLB_RID      16
#define W_CSR_GuestTLB_RID      8
#define M_CSR_GuestTLB_RID      (((1<<W_CSR_GuestTLB_RID)-1)<<S_CSR_GuestTLB_RID)

//0x16 CSR_GTLBR
#define S_CSR_GTLBR_is_GTLB     0
#define W_CSR_GTLBR_is_GTLB     1
#define M_CSR_GTLBR_is_GTLB     (((1<<W_CSR_GTLBR_is_GTLB)-1)<<S_CSR_GTLBR_is_GTLB)
#define S_CSR_GTLBR_TLBR_RID    16
#define W_CSR_GTLBR_TLBR_RID    8
#define M_CSR_GTLBR_TLBR_RID    (((1<<W_CSR_GTLBR_TLBR_RID)-1)<<S_CSR_GTLBR_TLBR_RID)

//0x18 CSR_ASID
#define S_CSR_ASID_ASID     0
#define W_CSR_ASID_ASID     10
#define M_CSR_ASID_ASID     (((1<<W_CSR_ASID_ASID)-1)<<S_CSR_ASID_ASID)

//0x41 CSR_TimerConfig
#define S_CSR_TimerConfig_En 		0
#define S_CSR_TimerConfig_Period 	1

//0x50 CSR_GuestInfo
#define S_CSR_GuestInfo_PVM     1
#define W_CSR_GuestInfo_PVM     1
#define M_CSR_GuestInfo_PVM     (((1<<W_CSR_GuestInfo_PVM)-1)<<S_CSR_GuestInfo_PVM)
#define S_CSR_GuestInfo_GID     16
#define W_CSR_GuestInfo_GID     8
#define M_CSR_GuestInfo_GID     (((1<<W_CSR_GuestInfo_GID)-1)<<S_CSR_GuestInfo_GID)

//0x51 CSR_GuestConfig
#define S_CSR_GuestConfig_GPERFNum 24
#define M_CSR_GuestConfig_GPERFNum 0x7000000

//0x60
#define S_CSR_LLBit_ROLLB  0
#define S_CSR_LLBit_WCLLB  1
#define S_CSR_LLBit_KLO    2

//0x80 CSR_GSConfig
#define S_CSR_GSConfig_Stfill   8
#define W_CSR_GSConfig_Stfill   1
#define M_CSR_GSConfig_Stfill   (((1<<W_CSR_GSConfig_Stfill)-1)<<S_CSR_GSConfig_Stfill)

//0x81 CSR_Flush
#define M_CSR_Flush_VTLB    0x1
#define M_CSR_Flush_FTLB    0x2
#define M_CSR_Flush_DTLB    0x4
#define M_CSR_Flush_ITLB    0x8
#define M_CSR_Flush_BTAC    0x10

//0x8a
#define S_CSR_RFEPC_IsTLBR  0
#define M_CSR_REEPC_EPC     0xfffffffffffffffc

//0x8f
#define S_CSR_RFState_PPLV 0
#define S_CSR_RFState_PIE  2
#define S_CSR_RFState_PVM  3
#define S_CSR_RFState_PWE  4
#define M_CSR_RFState_PPLV 0x3
#define M_CSR_RFState_PIE  0x4
#define M_CSR_RFState_PVM  0x8
#define M_CSR_RFState_PWE  0x10

//0x90
#define S_CSR_ERRCTL_ERRMode 0
#define S_CSR_ERRCTL_PPLV    2
#define S_CSR_ERRCTL_PIE     4
#define S_CSR_ERRCTL_PVM     5
#define S_CSR_ERRCTL_PWE     6
#define S_CSR_ERRCTL_PDA     7
#define S_CSR_ERRCTL_PPG     8
#define S_CSR_ERRCTL_PDACF   9
#define S_CSR_ERRCTL_PDACM   11
#define M_CSR_ERRCTL_PPLV    0xc
#define M_CSR_ERRCTL_PIE     0x10
#define M_CSR_ERRCTL_PVM     0x20
#define M_CSR_ERRCTL_PWE     0x40
#define M_CSR_ERRCTL_PDA     0x80
#define M_CSR_ERRCTL_PPG     0x100
#define M_CSR_ERRCTL_PDACF   0x600
#define M_CSR_ERRCTL_PDACM   0x1800

//0xaf userdef
#define S_CSR_USERDEF_DC     4

//0x200 CSR_PCCtl
#define S_CSR_PCCtl_EVENT 0
#define M_CSR_PCCtl_EVENT 0x3ff
#define S_CSR_PCCtl_PLV   16
#define M_CSR_PCCtl_PLV   0xf0000
#define S_CSR_PCCtl_GREN  21
#define M_CSR_PCCtl_GREN  0x600000

#endif /* _CSRDEF_H */
