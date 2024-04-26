/******************************************************************************
* Copyright 2018-2023 zhuangwei@nblink-tech.com
* FileName: 	 ls1c102_clock.h
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
#ifndef _LS1c102_CLOCK_H_
#define _LS1c102_CLOCK_H_

/*------------------------------- Includes ----------------------------------*/


/*----------------------------- Global Typedefs ------------------------------*/


/*----------------------------- Global Defines -----------------------------*/


/*----------------------------- External Variables --------------------------*/


/*------------------------ Global Function Prototypes -----------------------*/
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
void ls1c102_clock_init(unsigned int use_ext8m);

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
void ls1c102_clock_use_ext32K(void);

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
unsigned int ls1c102_clock_get(void);


#endif // LS1c102_CLOCK_H_INCLUDED
