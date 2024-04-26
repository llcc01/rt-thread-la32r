/******************************************************************************
* Copyright 2018-2023 zhuangwei@nblink-tech.com
* FileName: 	 ls1c102_clock.c
* Desc:
*
*
* Author: 	 zhuangwei
* Date: 	 2018-12-28
* Notes:
*
* -----------------------------------------------------------------
* Histroy: v1.0   2018-12-28, zhuangwei create this file
*
******************************************************************************/


/*------------------------------- Includes ----------------------------------*/
#include "ls1c102.h"

/*------------------- Global Definitions and Declarations -------------------*/


/*---------------------- Constant / Macro Definitions -----------------------*/


/*----------------------- Type Declarations ---------------------------------*/


/*----------------------- Variable Declarations -----------------------------*/


/*----------------------- Function Prototype --------------------------------*/


/*----------------------- Function Implement --------------------------------*/
/******************************************************************************
* Name: 	 ls1c102_clock_init
*
* Desc: 	 配置系统时钟源
* Param:     use_ext8m - 0:使用内部8M >0:使用外部8M
* Return:    无
* Global:
* Note:
* Author: 	 zhuangwei
* -------------------------------------
* Log: 	 2018/12/28, Create this function by zhuangwei
 ******************************************************************************/
void ls1c102_clock_init(unsigned int use_ext8m)
{
    //8M时钟选择位变化沿有效
	if(use_ext8m)
    {
        PMU_ChipCtrl &= ~(3<<8);
        PMU_ChipCtrl |= (1<<8);
        PMU_ChipCtrl &= ~(1<<7);
        PMU_ChipCtrl |= (1<<6);

        while((PMU_CmdSts&(1<<31)) == 1)
        {
            PMU_ChipCtrl &= ~(1<<7);
        }
        PMU_ChipCtrl|=(1<<7);
    }
    else
    {
        PMU_ChipCtrl |= (1<<7);
        PMU_ChipCtrl &= ~(1<<7);
    }
}

/******************************************************************************
* Name: 	 ls1c102_clock_use_ext32K
*
* Desc: 	 使用外部32K
* Param:     无
* Return:    无
* Global:
* Note:
* Author: 	 zhuangwei
* -------------------------------------
* Log: 	 2018/12/28, Create this function by zhuangwei
 ******************************************************************************/
void ls1c102_clock_use_ext32K(void)
{
    int cnt = 0;

	while((PMU_CmdSts&(1<<28)) == 0)
	{
        PMU_ChipCtrl &= ~(1<<5);
        if((cnt++) > 3000) break;
	}
	PMU_ChipCtrl |= (1<<5);
}

/******************************************************************************
* Name: 	 ls1c102_clock_get
*
* Desc: 	 获取系统时钟频率
* Param:     无
* Return:    无
* Global:
* Note:
* Author: 	 zhuangwei
* -------------------------------------
* Log: 	 2018/12/28, Create this function by zhuangwei
 ******************************************************************************/
unsigned int ls1c102_clock_get(void)
{
    if(PMU_CmdSts&(1<<30))
    {
        return EXT_OSC_8M;
    }
    else
    {
        return CLOCK_8M_FREQ*1000;
    }
}

/*---------------------------------------------------------------------------*/
