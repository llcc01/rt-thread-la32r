/******************************************************************************
* Copyright 2018-2023 zhuangwei@nblink-tech.com
* FileName:      ls1c102.h
* Desc:
*
*
* Author:    zhuangwei
* Date:      2018/09/23
* Notes:
*
* -----------------------------------------------------------------
* Histroy: v1.0   2018/09/23, zhuangwei create this file
*
******************************************************************************/
#ifndef _LS1c102_H_
#define _LS1c102_H_


/*------------------------------- Includes ----------------------------------*/
#include "mipsregs.h"


/*----------------------------- Global Typedefs -----------------------------*/
#define __WEAK                      __attribute__((weak))

#ifndef NULL
#define NULL                        ((void *)0)
#endif

#define UNUSED(x)                   (void)x

#define EXT_OSC_8M                  8000000

/*----------------------------- Global Defines ------------------------------*/
/********************************地址空间********************************/
#define UNCACHED_MEMORY_ADDR 	    0xa0000000
#define UNCACHED_TO_PHYS(x)         ((x) & 0x1fffffff)
#define PHYS_TO_UNCACHED(x)         ((x) | UNCACHED_MEMORY_ADDR)

#define IRAM_BASEADDR			    PHYS_TO_UNCACHED(0x00000000)            //iram
#define DRAM_BASEADDR			    PHYS_TO_UNCACHED(0x00001000)            //dram
#define SPI_FLASH_BASEADDR		    PHYS_TO_UNCACHED(0x1e000000)            //spi_flash
#define FLASH_BASEADDR			    PHYS_TO_UNCACHED(0x1f000000)            //on-chip flash
#define BOOT_ADDR				    PHYS_TO_UNCACHED(0x1fc00000)            //spi_flash/on-chip flash
#define FLASH_REG_BASEADDR		    PHYS_TO_UNCACHED(0x1fe60000)            //flash regs
#define SPI_REG_BASEADDR		    PHYS_TO_UNCACHED(0x1fe70000)            //spi regs
#define UART0_BASEADDR  		    PHYS_TO_UNCACHED(0x1fe80000)            //uart0
#define UART1_BASEADDR			    PHYS_TO_UNCACHED(0x1fe88000)            //uart1
#define UART2_BASEADDR			    PHYS_TO_UNCACHED(0x1fe8c000)            //uart2
#define I2C_BASEADDR			    PHYS_TO_UNCACHED(0x1fe90000)            //i2c
#define REGS_BASEADDR			    PHYS_TO_UNCACHED(0x1fea0000)            //Interrupt_Regs_Baseadd
#define PMU_BASEADDR			    PHYS_TO_UNCACHED(0x1feb0000)            //PMU
#define TSENSOR_BASEADDR		    PHYS_TO_UNCACHED(0x1feb4000)            //tsensor
#define RTC_BASEADDR			    PHYS_TO_UNCACHED(0x1feb8000)            //rtc
#define DMA_BASEADDR		        PHYS_TO_UNCACHED(0x1fec0000)            //DMA
#define VPWM_BASEADDR		        PHYS_TO_UNCACHED(0x1fec0020)            //vpwm
#define TIMER_BASEADDR		        PHYS_TO_UNCACHED(0x1fed0000)            //timer

/********************************CLOCK REGS********************************/
#define CLOCK_8M_FREQ               *(volatile unsigned int *)(0xbf0201b0)           //内部8M时钟频率


/********************************PMU REGS********************************/
#define PMU_ChipCtrl  	            *(volatile unsigned int *)(PMU_BASEADDR+0x00)    //全局配置
#define PMU_CmdSts  	            *(volatile unsigned int *)(PMU_BASEADDR+0x04)    //命令与状态
#define PMU_Count   	            *(volatile unsigned int *)(PMU_BASEADDR+0x08)    //时间计数器
#define PMU_Compare  	            *(volatile unsigned int *)(PMU_BASEADDR+0x0c)    //唤醒时间配置
#define PMU_IOSEL0    	            *(volatile unsigned int *)(PMU_BASEADDR+0x10)    //IO复用选择0
#define PMU_IOSEL1  	            *(volatile unsigned int *)(PMU_BASEADDR+0x14)    //IO复用选择1
#define PMU_IOSEL2   	            *(volatile unsigned int *)(PMU_BASEADDR+0x18)    //IO复用选择2
#define PMU_IOSEL3 		            *(volatile unsigned int *)(PMU_BASEADDR+0x1c)    //IO复用选择3
#define PMU_ExintEn 	            *(volatile unsigned int *)(PMU_BASEADDR+0x20)    //外部中断使能
#define PMU_ExintPol                *(volatile unsigned int *)(PMU_BASEADDR+0x24)    //外部中断极性
#define PMU_ExintEdge               *(volatile unsigned int *)(PMU_BASEADDR+0x28)    //外部中断边沿
#define PMU_ExintSrc                *(volatile unsigned int *)(PMU_BASEADDR+0x2c)    //外部中断状态
#define PMU_WdtCfg                  *(volatile unsigned int *)(PMU_BASEADDR+0x30)    //看门狗配置
#define PMU_WdtFeed                 *(volatile unsigned int *)(PMU_BASEADDR+0x34)    //看门狗重置
#define PMU_PowerCfg 	            *(volatile unsigned int *)(PMU_BASEADDR+0x38)    //电源配置
#define PMU_GPIOA_OE 	            *(volatile unsigned int *)(PMU_BASEADDR+0x40)    //GPIOA输出使能
#define PMU_GPIOA_O		            *(volatile unsigned int *)(PMU_BASEADDR+0x44)    //GPIOA输出电平
#define PMU_GPIOA_I		            *(volatile unsigned int *)(PMU_BASEADDR+0x48)    //GPIOA输入电平
#define PMU_GPIOB_OE 	            *(volatile unsigned int *)(PMU_BASEADDR+0x50)    //GPIOB输出使能
#define PMU_GPIOB_O		            *(volatile unsigned int *)(PMU_BASEADDR+0x54)    //GPIOB输出电平
#define PMU_GPIOB_I		            *(volatile unsigned int *)(PMU_BASEADDR+0x58)    //GPIOB输入电平
#define PMU_Pulse0 		            *(volatile unsigned int *)(PMU_BASEADDR+0x60)    //脉冲输出配置0
#define PMU_Pulse1 		            *(volatile unsigned int *)(PMU_BASEADDR+0x64)    //脉冲输出配置1
#define PMU_UserDAT 	            *(volatile unsigned int *)(PMU_BASEADDR+0x68)    //用户数据
#define PMU_AdcCtrl 	            *(volatile unsigned int *)(PMU_BASEADDR+0x6c)    //ADC控制
#define PMU_AdcDat  	            *(volatile unsigned int *)(PMU_BASEADDR+0x70)    //ADC数据
#define PMU_GPIOBit(i) 	            *(volatile unsigned char *)(PMU_BASEADDR+0x80+i)   //GPIO位访问

#define PMU_CommandW                *(volatile unsigned int *) (PMU_BASEADDR+0x3c)   //Command写端口

/***************************INTERRUPT REGS******************************/
#define INT_EN	                    *(volatile unsigned char *)(REGS_BASEADDR+0x00)     //中断使能寄存器
#define INT_EGDE                    *(volatile unsigned char *)(REGS_BASEADDR+0x01)     //中断边沿寄存器
#define INT_POL                     *(volatile unsigned char *)(REGS_BASEADDR+0x02)     //中断极性寄存器
#define INT_CLR                     *(volatile unsigned char *)(REGS_BASEADDR+0x03)     //中断清除寄存器
#define INT_SET                     *(volatile unsigned char *)(REGS_BASEADDR+0x04)     //中断置位寄存器
#define INT_OUT                     *(volatile unsigned char *)(REGS_BASEADDR+0x05)     //中断输出寄存器
#define INT_SRPROT                  *(volatile unsigned char *)(REGS_BASEADDR+0x06)     //运行状态及保护寄存

/*******************************TIMER REGS*********************************/
#define TIMER_CFG  	 	            *(volatile unsigned int *)(TIMER_BASEADDR+0x00)   //配置寄存器
#define TIMER_CNT		            *(volatile unsigned int *)(TIMER_BASEADDR+0x04)   //计数值寄存器
#define TIMER_CMP		            *(volatile unsigned int *)(TIMER_BASEADDR+0x08)   //比较值寄存器
#define TIMER_STP                   *(volatile unsigned int *)(TIMER_BASEADDR+0x0C)   //步进值寄存器

/*******************************I2C REGS********************************/
#define IIC_PRERL	                *(volatile unsigned char *)(I2C_BASEADDR+0x00)     //分频值低字节寄存器
#define IIC_PRERH 	                *(volatile unsigned char *)(I2C_BASEADDR+0x01)     //分频值高字节寄存器
#define IIC_CTR   	                *(volatile unsigned char *)(I2C_BASEADDR+0x02)     //控制寄存器
#define IIC_DR   	                *(volatile unsigned char *)(I2C_BASEADDR+0x03)     //数据寄存器
#define IIC_CR		                *(volatile unsigned char *)(I2C_BASEADDR+0x04)     //命令寄存器
#define IIC_SR		                *(volatile unsigned char *)(I2C_BASEADDR+0x04)     //状态寄存器
#define IIC_BLTOP		            *(volatile unsigned char *)(I2C_BASEADDR+0x05)     //总线死锁时间寄存器
#define IIC_SADDR	                *(volatile unsigned char *)(I2C_BASEADDR+0x07)     //从模式地址寄存器

/*******************************SPI REGS********************************/
#define SPI_SPCR		            *(volatile unsigned char *)(SPI_REG_BASEADDR+0x00) //控制寄存器
#define SPI_SPSR		            *(volatile unsigned char *)(SPI_REG_BASEADDR+0x01) //状态寄存器
#define SPI_TxFIFO		            *(volatile unsigned char *)(SPI_REG_BASEADDR+0x02) //数据寄存器
#define SPI_RxFIFO		            *(volatile unsigned char *)(SPI_REG_BASEADDR+0x02) //数据寄存器
#define SPI_SPER		            *(volatile unsigned char *)(SPI_REG_BASEADDR+0x03) //外部寄存器
#define SPI_SFC_PARAM	            *(volatile unsigned char *)(SPI_REG_BASEADDR+0x04) //参数控制寄存器
#define SPI_SFC_SOFTCS	            *(volatile unsigned char *)(SPI_REG_BASEADDR+0x05) //片选控制寄存器
#define SPI_SFC_TIMING	            *(volatile unsigned char *)(SPI_REG_BASEADDR+0x06) //时序控制寄存器

/*****************************UART REGS*********************************/
#define UART_TxData(base)           *(volatile unsigned char *)(base+0x00)   //发送数据寄存器
#define UART_RxData(base)           *(volatile unsigned char *)(base+0x00)   //接收数据寄存器
#define UART_IER(base)              *(volatile unsigned char *)(base+0x01)   //中断使能寄存器
#define UART_IIR(base)   	        *(volatile unsigned char *)(base+0x02)   //中断状态寄存
#define UART_FCR(base)   	        *(volatile unsigned char *)(base+0x02)	//FIFO控制寄存器
#define UART_LCR(base)   	        *(volatile unsigned char *)(base+0x03)   //线路控制寄存器
#define UART_SAMPLE_CTRL(base)      *(volatile unsigned char *)(base+0x04)   //bit窗口划分和采样控制寄存器
#define UART_LSR(base)  	        *(volatile unsigned char *)(base+0x05)   //线路状态寄存器
#define UART_TF_CNT(base)   	    *(volatile unsigned char *)(base+0x06)   //发送队列数据存量
#define UART_STATUS(base)   	    *(volatile unsigned char *)(base+0x07)   //状态寄存器

#define UART_DL_L(base)   	        *(volatile unsigned char *)(base+0x00)   //分频值低字节寄存器
#define UART_DL_H(base)   	        *(volatile unsigned char *)(base+0x01)   //分频值高字节寄存器
#define UART_DL_D(base)   	        *(volatile unsigned char *)(base+0x02)   //分频值小数寄存器

/*****************************RTC REGS**********************************/
#define RTC_FREQ   	                *(volatile unsigned int *)(RTC_BASEADDR+0x00)    //分频值寄存器
#define RTC_CFG   	                *(volatile unsigned int *)(RTC_BASEADDR+0x04)    //配置寄存器
#define RTC_RTC0   	                *(volatile unsigned int *)(RTC_BASEADDR+0x08)    //时间值寄存器0
#define RTC_RTC1   	                *(volatile unsigned int *)(RTC_BASEADDR+0x0c)    //时间值寄存器1

/*****************************DMA REGS**********************************/
#define DMA_SOURCE                  *(volatile unsigned int *)(DMA_BASEADDR+0x00)    //DMA命令源地址写端口
#define DMA_COUNT                   *(volatile unsigned int *)(DMA_BASEADDR+0x04)    //DMA命令数据长度读写端口
#define DMA_CMD_STATUS              *(volatile unsigned int *)(DMA_BASEADDR+0x08)    //命令和状态寄存器
#define DMA_INT_STATUS              *(volatile unsigned int *)(DMA_BASEADDR+0x0c)    //中断和状态寄存器
#define DMA_SOURCE0   	            *(volatile unsigned int *)(DMA_BASEADDR+0x10)    //命令队列项0的源地址参数
#define DMA_SOURCE1  	            *(volatile unsigned int *)(DMA_BASEADDR+0x14)    //命令队列项1的源地址参数
#define DMA_COUNT0 	                *(volatile unsigned int *)(DMA_BASEADDR+0x18)    //命令队列项0的DMA长度参数
#define DMA_COUNT1 	                *(volatile unsigned int *)(DMA_BASEADDR+0x1c)    //命令队列项1的DMA长度参数

/*****************************VPWM REGS**********************************/
#define VPWM_CFG   	                *(volatile unsigned int *)(VPWM_BASEADDR+0x00)   //算法配置
#define VPWM_WPORT_STATUS           *(volatile unsigned int *)(VPWM_BASEADDR+0x08)   //数据写端口状态
#define VPWM_WPORT 	                *(volatile unsigned int *)(VPWM_BASEADDR+0x0c)   //数据写端口

/*****************************TSENSOR REGS**********************************/
#define TS_CTRL                     *(volatile unsigned int *)(TSENSOR_BASEADDR+0x00) //控制寄存器
#define TS_STAT          	        *(volatile unsigned int *)(TSENSOR_BASEADDR+0x04) //状态寄存器
#define TS_OSCCFG         	        *(volatile unsigned int *)(TSENSOR_BASEADDR+0x08) //环振配置寄存器
#define TS_POLLTIM                  *(volatile unsigned int *)(TSENSOR_BASEADDR+0x0c) //扫描时序寄存器
#define TS_DIFFTHRES                *(volatile unsigned int *)(TSENSOR_BASEADDR+0x10) //差异阀值寄存器
#define TS_CNTMAX   	            *(volatile unsigned int *)(TSENSOR_BASEADDR+0x14) //最大计数值
#define TS_CNTMIN 	                *(volatile unsigned int *)(TSENSOR_BASEADDR+0x18) //最小计数值
#define TS_CNTLOW 	                *(volatile unsigned int *)(TSENSOR_BASEADDR+0x1c) //第二小计数值
#define TS_CNTADJ(a)                *(volatile unsigned int *)(TSENSOR_BASEADDR+0x40+a*4) //计数修正值
#define TS_CNTRES(a)                *(volatile unsigned int *)(TSENSOR_BASEADDR+0x80+a*4) //计数结果

/*****************************FLASH REGS********************************/
#define FLASH_CMD_REG	            *(volatile unsigned int *)(FLASH_REG_BASEADDR+0x00)   //命令寄存器
#define FLASH_CAH_REG		        *(volatile unsigned int *)(FLASH_REG_BASEADDR+0x04)   //加密地址上界寄存器
#define FLASH_CAL_REG		        *(volatile unsigned int *)(FLASH_REG_BASEADDR+0x08)   //加密地址下界寄存器
#define FLASH_VRF_REG		        *(volatile unsigned int *)(FLASH_REG_BASEADDR+0x10)   //数据效验寄存器
#define FLASH_STS_REG		        *(volatile unsigned int *)(FLASH_REG_BASEADDR+0x14)   //状态寄存器
#define FLASH_PET_REG		        *(volatile unsigned int *)(FLASH_REG_BASEADDR+0x18)   //擦写时间寄存器


#define WDT_SET(time)     	        { PMU_WdtCfg = (((~time)<<16) | time);\
                                        PMU_WdtFeed = 0xa55a55aa;}

#define WDT_FEED 	     	        {PMU_WdtFeed = 0xa55a55aa;}


/*----------------------------- External Variables --------------------------*/




/*------------------------ Global Function Prototypes -----------------------*/



#endif //_LS1c102_H_
