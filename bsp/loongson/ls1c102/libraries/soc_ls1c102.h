#ifndef SOC_LS1C102_H_
#define SOC_LS1C102_H_

#ifdef __cplusplus
extern "C" {
#endif 

#include <stdint.h>



// ================================================================
#define LS1C102_PMU_BASE           0xBFEB0000      // 电源管理单元寄存器基地址

typedef struct 
{
    volatile unsigned int ChipCtrl;             // 0x00 全局配置
    volatile unsigned int CmdSts;               // 0x04 命令与状态
    volatile unsigned int Count;                // 0x08 时间计数器
    volatile unsigned int Compare;              // 0x0c 唤醒时间配置
    volatile unsigned int IOSEL0;               // 0x10 IO复用选择 0
    volatile unsigned int IOSEL1;               // 0x14 IO复用选择 1
    volatile unsigned int IOSEL2;               // 0x18 IO复用选择 2
    volatile unsigned int IOSEL3;               // 0x1c IO复用选择 3
    volatile unsigned int ExIntEn;              // 0x20 外部中断使能
    volatile unsigned int ExIntPol;             // 0x24 外部中断极性
    volatile unsigned int ExIntEdge;            // 0x28 外部中断边沿
    volatile unsigned int ExIntSrc;             // 0x2c 外部中断状态
    volatile unsigned int WdtCfg;               // 0x30 看门狗配置
    volatile unsigned int WdtFeed;              // 0x34 喂狗
    volatile unsigned int PowerCfg;             // 0x38 电源配置
    volatile unsigned int CommandW;             // 0x3C XXX datasheet上没有描述这个寄存器, == CmdSts?
    volatile unsigned int GPIOA_OE;             // 0x40 GPIOA 输出使能
    volatile unsigned int GPIOA_O;              // 0x44 GPIOA 输出
    volatile unsigned int GPIOA_I;              // 0x48 GPIOA 输入
    volatile unsigned int rsv1;                 // 0x4C
    volatile unsigned int GPIOB_OE;             // 0x50 GPIOB 输出使能
    volatile unsigned int GPIOB_O;              // 0x54 GPIOB 输出
    volatile unsigned int GPIOB_I;              // 0x58 GPIOB 输入
    volatile unsigned int rsv2;                 // 0x5C
    volatile unsigned int Pulse0;               // 0x60 脉冲输出配置 0
    volatile unsigned int Pulse1;               // 0x64 脉冲输出配置 1
    volatile unsigned int UserDat;              // 0x68 用户数据
    volatile unsigned int AdcCtrl;              // 0x6c ADC 控制
    volatile unsigned int AdcDat;               // 0x70 ADC 数据
    volatile unsigned int rsv3[3];              // 0x74/0x78/0x7C
    volatile unsigned char GPIOBit[0x40];       // 0x80~0xbf GPIO 位访问
} HW_PMU_t;

#define g_pmu ((HW_PMU_t *) LS1C102_PMU_BASE)
// ================================================================



// ================================================================
#define LS1C102_TIMER_BASE         0xBFED0000      // 定时器寄存器基地址

typedef struct 
{
    volatile unsigned int cfg;                  // 0x00 配置寄存器
    volatile unsigned int cnt;                  // 0x04 计数值寄存器
    volatile unsigned int cmp;                  // 0x08 比较值寄存器
    volatile unsigned int step;                 // 0x0c 步进值寄存器
} HW_TIMER_t;

#define g_timer ((HW_TIMER_t *)LS1C102_TIMER_BASE)// 获取定时器寄存器
// ================================================================



// ================================================================
#define LS1C102_INTC_BASE          0xBFEA0000      // 中断控制寄存器基地址

typedef struct
{
    volatile unsigned char en;     // 0x00 中断使能寄存器. 1=中断使能
    volatile unsigned char edge;   // 0x01 中断边沿寄存器. 1=边沿触发, 0=电平触发
    volatile unsigned char pol;    // 0x02 中断极性寄存器. 1=高电平/上升沿触发
    volatile unsigned char clr;    // 0x03 中断清除寄存器. 写1清除中断状态
    volatile unsigned char set;    // 0x04 中断置位寄存器. 写1置中断触发模式的中断状态
    volatile unsigned char out;    // 0x05 中断输出寄存器. 1=中断触发
    volatile unsigned char srprot; // 0x06 运行状态及保护寄存器
} HW_INTC_t;

#define intc ((HW_INTC_t *)LS1C102_INTC_BASE)// 获取中断寄存器
// ================================================================



// ================================================================
// typedef enum {DISABLE = 0, ENABLE = !DISABLE} FunctionalState;

// #define I2C                 ((I2C_TypeDef *) I2C_BASE)

// typedef struct
// {
// 	uint32_t I2C_ClockSpeed;         /*!< Specifies the clock frequency.
//                                         This parameter must be set to a value lower than 400kHz */

// 	uint8_t I2C_Mode;                /*!< Specifies the I2C mode.
//                                         This parameter can be a value of @ref I2C_mode */

// 	uint8_t I2C_OwnAddress1;         /*!< Specifies the first device own address.
//                                         This parameter can be a 7-bit or 10-bit address. */

// 	uint8_t I2C_BuslockCheckEn;    	 /*!< Set the I2C buslock check function*/


// 	uint8_t I2C_SlvAutoresetEn;		 /*!< Set the I2C autoreset check function*/

// }I2C_InitTypeDef;
// ================================================================




#ifdef __cplusplus
}
#endif 

#endif // SOC_LS1C102_H_


