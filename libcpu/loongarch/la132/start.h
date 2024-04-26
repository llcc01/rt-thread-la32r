#ifndef _START_H_
#define _START_H_

/******************  CLK CONFIG  ******************/
#define BSS_BASE            (0x80000000)
#define DATA_BASE           (0x80001000)
#define SP_BASE             (0x80002000-132)
#define REGS_MEM          (0x80002000) //first use in REGS_MEM-4


#define CACHE_OP          (0xfff1ffff)
#define ADDR_MAP          (0x76503210)

#define INT_VECTOR        (0xbbc)
#define INTC_VECTOR       (0x40)



// for func-test purpose
#define WATCHDOG_ON_DEMAND 0x12344321
#define NMI_ON_DEMAND      0x43211234


#define UNCACHED_MEMORY_ADDR 	0xa0000000
#define UNCACHED_TO_PHYS(x)     ((x) & 0x1fffffff)
#define PHYS_TO_UNCACHED(x)     ((x) | UNCACHED_MEMORY_ADDR)
#define RAM0_BASE			    PHYS_TO_UNCACHED(0x00000000)            //iram
#define RAM1_BASE			    PHYS_TO_UNCACHED(0x00001000)            //dram
#define SPI_MEM_BASE			PHYS_TO_UNCACHED(0x1e000000)            //spi_flash
#define FLASH_MEM_BASE   		PHYS_TO_UNCACHED(0x1f000000)            //on-chip flash
#define FLASH_BASE		        PHYS_TO_UNCACHED(0x1fe60000)            //flash regs
#define SPI_BASE	        	PHYS_TO_UNCACHED(0x1fe70000)            //spi regs
#define UART0_BASEADDR  		PHYS_TO_UNCACHED(0x1fe80000)            //uart0
#define UART1_BASEADDR			PHYS_TO_UNCACHED(0x1fe88000)            //uart1
#define UART2_BASEADDR			PHYS_TO_UNCACHED(0x1fe8c000)            //uart2
#define I2C_BASE			    PHYS_TO_UNCACHED(0x1fe90000)            //i2c
#define INTC_BASE			    PHYS_TO_UNCACHED(0x1fea0000)            //Interrupt_Regs_Baseadd
#define PMU_BASE			    PHYS_TO_UNCACHED(0x1feb0000)            //PMU
#define TSENSOR_BASE		    PHYS_TO_UNCACHED(0x1feb4000)            //tsensor
#define RTC_BASE			    PHYS_TO_UNCACHED(0x1feb8000)            //rtc
#define DMA_BASE		        PHYS_TO_UNCACHED(0x1fec0000)            //DMA
#define VPWM_BASE		        PHYS_TO_UNCACHED(0x1fec0020)            //vpwm
#define TIMER_BASE		        PHYS_TO_UNCACHED(0x1fed0000)            //timer
#define UART_BASEADDR  UART0_BASEADDR
//#define UART_BASEADDR  UART1_BASEADDR





#define PMU_ChipCtrl               0x00
#define PMU_Command                0x04
#define PMU_Count                  0x08
#define PMU_Compare                0x0c
#define PMU_WdtCfg                 0x30
#define PMU_WdtFeed                0x34
#define PMU_PowerCfg               0x38
#define PMU_CommandW               0x3c
#define PMU_GPIOAOE_OFFSET         0x40
#define PMU_GPIOAO_OFFSET          0x44
#define PMU_GPIOAI_OFFSET          0x48
#define PMU_GPIOBOE_OFFSET         0x50
#define PMU_GPIOBO_OFFSET          0x54
#define PMU_GPIOBI_OFFSET          0x58
#define PMU_PULSE0_OFFSET          0x60
#define PMU_PULSE1_OFFSET          0x64
#define PMU_ADCCTRL_OFFSET         0x6c
#define PMU_ADCDAT_OFFSET          0x70
//PMU access by 8-bit
#define PMU_GPIOBIT_OFFSET         0x80
#define PMU_SoftRst		    0x10
#define PMU_ClkOff		    0x14
#define PMU_SrProt		    0x18
#define PMU_UserDat1		    0x64
#define PMU_UserDat2		    0x68
#define PMU_UserDat3		    0x6c


#define ResetSrc      (26)



#define PMU_UserDat0               0x68
#define AFIO_Sela  		   0x10
#define AFIO_Selb                  0x14
#define AFIO_Selc                  0x18
#define AFIO_Seld                  0x1c
#define EXTI_En 	           0x20
#define EXTI_Pol                   0x24
#define EXTI_Edge                  0x28
#define EXTI_Src                   0x2c
#define AFIO_BASE	(PMU_BASE + 0x10)
#define GPIOA_BASE	(PMU_BASE + 0x40)
#define GPIOB_BASE	(PMU_BASE + 0x50)
#define AFIO		((AFIO_TypeDef *) AFIO_BASE)
#define GPIOA		((GPIO_TypeDef *) GPIOA_BASE)
#define GPIOB		((GPIO_TypeDef *) GPIOB_BASE)


#define INTC_EN_OFFSET             0x0
#define INTC_EDGE_OFFSET           0x1
#define INTC_POL_OFFSET            0x2
#define INTC_CLR_OFFSET            0x3
#define INTC_SET_OFFSET            0x4
#define INTC_OUT_OFFSET            0x5
#define INTC_SRPROT_OFFSET         0x6
#define INTC_CKGATE_OFFSET         0x7








#endif /* _START_H_ */
