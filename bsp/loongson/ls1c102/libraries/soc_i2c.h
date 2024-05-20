#ifndef _SOC_I2C_H
#define _SOC_I2C_H

#ifdef __cplusplus
extern "C" {
#endif

#include <rtthread.h>

typedef enum {DISABLE = 0, ENABLE = !DISABLE} FunctionalState;

/**
 * @说明：I2C接口宏定义
 *
 */
typedef struct
{
  volatile uint8_t PRERL;			/* 分频值低字节寄存器 */
  volatile uint8_t PRERH;			/* 分频值高字节寄存器 */
  volatile uint8_t CTRL;			/* 控制寄存器 */
  volatile uint8_t DR;				/* 数据寄存器 */
  volatile uint8_t CR_SR;			/* 命令寄存器 兼 状态寄存器 */
  volatile uint8_t BLTOP;			/* 总线死锁时间寄存器 */
  volatile uint8_t RESERVED0;		/* 保留字节  */
  volatile uint8_t SADDR;			/* 从模式地址寄存器 */
} I2C_TypeDef;

#define I2C_BASE            0xbfe90000
#define I2C                 ((I2C_TypeDef *) I2C_BASE)

typedef struct
{
	uint32_t I2C_ClockSpeed;         /*!< Specifies the clock frequency.
                                        This parameter must be set to a value lower than 400kHz */

	uint8_t I2C_Mode;                /*!< Specifies the I2C mode.
                                        This parameter can be a value of @ref I2C_mode */

	uint8_t I2C_OwnAddress1;         /*!< Specifies the first device own address.
                                        This parameter can be a 7-bit or 10-bit address. */

	uint8_t I2C_BuslockCheckEn;    	 /*!< Set the I2C buslock check function*/


	uint8_t I2C_SlvAutoresetEn;		 /*!< Set the I2C autoreset check function*/

}I2C_InitTypeDef;

// #define IS_I2C_ALL_PERIPH(PERIPH) ((PERIPH) == I2C)

// #define IS_I2C_CLEAR_FLAG(FLAG) (((FLAG) & 0x01) == 0x01)
// /** @defgroup I2C_mode
//   * @{
//   */
// #define IS_I2C_CLOCK_SPEED(SPEED) (((SPEED) >= 0x1) && ((SPEED) <= 400000))

// #define IS_I2C_OWN_ADDRESS1(ADDRESS1) ((ADDRESS1) <= 0x3FF)

#define I2C_Mode_Slave            ((uint8_t)0x00)
#define I2C_Mode_Master           ((uint8_t)0x20)

// #define IS_I2C_MODE(MODE) (((MODE) == I2C_Mode_Slave) || \
// 						   ((MODE) == I2C_Mode_Master))
// /**
//   * @}
//   */
#define I2C_Buslock_Check_Enable                  ((uint8_t)0x02)
// #define I2C_Buslock_Check_Disable                 ((uint8_t)0x00)
// #define IS_I2C_BUSLOCK_CHECK_EN(STATE) (((STATE) == I2C_Buslock_Check_Enable) || \
// 										((STATE) == I2C_Buslock_Check_Disable))

// #define I2C_Slv_Autoreset_Enable                  ((uint8_t)0x01)
// #define I2C_Slv_Autoreset_Disable                 ((uint8_t)0x00)
// #define IS_I2C_SLV_AUTORESET_EN(STATE) (((STATE) == I2C_Slv_Autoreset_Enable) || \
// 										((STATE) == I2C_Slv_Autoreset_Disable))

// /** @defgroup I2C_transfer_direction
//   * @{
//   */
// #define I2C_Direction_Transmitter       ((uint8_t)0x00)
// #define I2C_Direction_Receiver          ((uint8_t)0x01)
// #define IS_I2C_DIRECTION(DIRECTION) (((DIRECTION) == I2C_Direction_Transmitter) || \
// 										((DIRECTION) == I2C_Direction_Receiver))

// #define  I2C_ACK      					((uint8_t)0x00)
// #define  I2C_NACK         				((uint8_t)0x08)
// #define IS_I2C_ACK(ACK) 	(((ACK) == I2C_ACK) ||  ((DIRECTION) == I2C_NACK))

// #define I2C_STOP      					((uint8_t)0x40)
// #define I2C_NSTOP         				((uint8_t)0x00)
// #define IS_I2C_STOP(STOP) 	(((STOP) == I2C_STOP) ||  ((DIRECTION) == I2C_NSTOP))

/**
 *@ I2C_STATE_Flags
 */
#define I2C_STATE_RxACK                     ((uint8_t)0x80)  /* 收到的应答位： 0表示收到应答，1表示收到NACK */
#define I2C_STATE_BUSY                      ((uint8_t)0x40)  /* 总线忙标志 */
#define I2C_STATE_AL                        ((uint8_t)0x20)  /* 失去仲裁：1表示主设备失去总线控制权  */
#define I2C_STATE_Slave_Addressed           ((uint8_t)0x10)  /* 被寻址：1表示作为从设备时已被寻址成功  */
#define I2C_STATE_Slave_Rw                  ((uint8_t)0x08)  /* 从设备读写：0表示被读 ，1表示被写 */
#define I2C_STATE_Buslock                   ((uint8_t)0x04)  /* 总线死锁：1表示总线出现死锁 */
#define I2C_STATE_TIP                       ((uint8_t)0x02)  /* 传输进行：1表示主设备有效，正在传输 */
#define I2C_STATE_IF                        ((uint8_t)0x01)  /* 中断标志位：1表示传输完一个字节或主设备失去仲裁 */

/** @defgroup I2C_registers
  * @{
  */
// #define IS_I2C_GET_STATUS(I2C_STATUS)  (((I2C_STATUS) == I2C_STATE_RxACK) || \
// 										((I2C_STATUS) == I2C_STATE_BUSY) || \
// 										((I2C_STATUS) == I2C_STATE_AL) || \
// 										((I2C_STATUS) == I2C_STATE_Slave_Addressed) || \
// 										((I2C_STATUS) == I2C_STATE_Slave_Rw) || \
// 										((I2C_STATUS) == I2C_STATE_Buslock) || \
// 										((I2C_STATUS) == I2C_STATE_TIP) || \
// 										((I2C_STATUS) == I2C_STATE_IF))

// #define I2C_Register_PRERL				((uint8_t)0x00)
// #define I2C_Register_PRERH				((uint8_t)0x01)
// #define I2C_Register_CTR				((uint8_t)0x02)
// #define I2C_Register_DR					((uint8_t)0x03)
// #define I2C_Register_SR					((uint8_t)0x04)
// #define I2C_Register_BLTOP				((uint8_t)0x05)
// #define I2C_Register_SADDR				((uint8_t)0x07)

// #define IS_I2C_REGISTER(REGISTER) (((REGISTER) == I2C_Register_PRERL) || \
//                                    ((REGISTER) == I2C_Register_PRERH) || \
//                                    ((REGISTER) == I2C_Register_CTR) || \
//                                    ((REGISTER) == I2C_Register_DR) || \
//                                    ((REGISTER) == I2C_Register_SR) || \
//                                    ((REGISTER) == I2C_Register_BLTOP) || \
//                                    ((REGISTER) == I2C_Register_BLTOP))

// #define IRQ_I2C         	((uint8_t)0x02)
//#define I2C_RXACK			((uint8_t)0x00)
//#define I2C_BUSY			((uint8_t)0x01)
//#define I2C_AL			((uint8_t)0x02)
//#define I2C_SLV_ADDR		((uint8_t)0x03)
//#define I2C_SLV_RW		((uint8_t)0x04)
//#define I2C_BUS_LOCK		((uint8_t)0x05)
//#define I2C_TIP			((uint8_t)0x07)
//#define I2C_IF			((uint8_t)0x07)

void soc_I2C_delay(volatile int j);

// I2C 初始化
void soc_I2C_Init(I2C_InitTypeDef* I2C_InitStruct);
void soc_I2C_StructInit(I2C_InitTypeDef* I2C_InitStruct);
// I2C 初始化

// I2C 时序
void soc_I2C_GenerateSTART(FunctionalState NewState);	 // I2C 主设备产生 start信号
void soc_I2C_GenerateSTOP(FunctionalState NewState);     // I2C 主设备产生 stop信号
void soc_I2C_wait();									 // I2C 等待传输完毕
void soc_I2C_wait_available();							 // I2C 等待总线空闲
void soc_I2C_SendData(uint8_t Data);					 // I2C 发送数据
void soc_I2C_ReceiveData(FunctionalState ACK, FunctionalState STOP); // I2C 接收数据
void soc_I2C_Unlock();									 // I2C 解除总线死锁

// void I2C_AcknowledgeConfig(FunctionalState NewState);// I2C主设备产生ack信号
// void I2C_Send7bitAddress(uint8_t Address, uint8_t I2C_Direction);// I2C发送从设备地址
// void I2C_BUSY();									 // I2C等待总线空闲
// I2C 时序

// I2C 中断
// void I2C_ITConfig(FunctionalState NewState);		 // I2C中断设置
// void I2C_ITCmd(FunctionalState NewState);			 // I2C中断开启
// void I2C_ClearIT();									 // I2C清除中断标志
// void I2C_ClearFlag(uint8_t I2C_FLAG);				 // I2C清除寄存器状态
// ITStatus I2C_GetStatus(uint8_t I2C_STATUS);			 // I2C读取状态
// I2C 中断

// uint8_t I2C_ReadRegister(uint8_t I2C_Register);		 // I2C读取寄存器数值

#ifdef __cplusplus
}
#endif

#endif // _SOC_I2C_H
