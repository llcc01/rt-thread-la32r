#include "soc_i2c.h"
#include "rtthread.h"
#include "rttypes.h"


/* I2C START mask */
#define I2C_START_Set           ((rt_uint8_t)0x80)
#define I2C_START_Reset         ((rt_uint8_t)0x7F)

/* I2C STOP mask */
#define I2C_STOP_Set            ((rt_uint8_t)0x40)
#define I2C_STOP_Reset          ((rt_uint8_t)0xBF)

/* I2C ACK mask */
#define I2C_ACK_Set             ((rt_uint8_t)0x08)
#define I2C_ACK_Reset           ((rt_uint8_t)0xF7)

/* I2C ADD0 mask */
#define I2C_ADD0_Set            ((rt_uint8_t)0x01)
#define I2C_ADD0_Reset          ((rt_uint8_t)0xFE)

#define FLAG_Mask               ((rt_uint8_t)0xFF)
#define ITEN_Mask               ((rt_uint8_t)0x40)

void soc_I2C_delay(volatile int j) {
    while(j--) {
        ;
    }
}

/**
  * @brief  Fills each I2C_InitStruct member with its default value.
  * @param  I2C_InitStruct: pointer to an I2C_InitTypeDef structure which will be initialized.
  * @retval None
  */
void soc_I2C_StructInit(I2C_InitTypeDef* I2C_InitStruct) {
    /*---------------- Reset I2C init structure parameters values ----------------*/
    /* initialize the I2C_ClockSpeed member */
    //I2C_InitStruct->I2C_ClockSpeed = 100000;// IIC 标准速率100k，快速速率400k。
    I2C_InitStruct->I2C_ClockSpeed = 400000;// 
    
    /* Initialize the I2C_Mode member */
    I2C_InitStruct->I2C_Mode = I2C_Mode_Master;
    //I2C_InitStruct->I2C_Mode = I2C_Mode_Slave;

    /* Initialize the I2C_Buslock Check */
    I2C_InitStruct->I2C_BuslockCheckEn = I2C_Buslock_Check_Enable;// 总线死锁状态检查使能
}

/**
  * @brief  Initializes the I2C peripheral according to the specified
  * 		parameters in the I2C_InitStruct .
  * @param  I2C: select the I2C peripheral.
  *         This parameter can be I2C.
  * @param  I2C_InitStruct:  pointer to an I2C_InitTypeDef structure
  *         that contains the configuration information for the specified I2C peripheral.
  * @retval None
  */
void soc_I2C_Init(I2C_InitTypeDef* I2C_InitStruct) {
	rt_uint32_t tmp = 0;
	rt_uint32_t pclk1 = 8000000;// 8MHz 时钟

	/* Check the parameters */
	/*---------------------------- I2C Configuration ------------------------*/
	I2C->CTRL = 0x20;// 选择主模式，访问分频寄存器

	tmp = pclk1 / I2C_InitStruct->I2C_ClockSpeed;// 8M / 400k = 20 = 0x14. 8M / 100k = 80 = 0x50

	tmp = (tmp >> 2) - 1;

	I2C->PRERL = tmp;// 8MHz时钟，如果使用的是100k，那么分频系数 tmp = 0x13，所以低字节寄存器就够了，高字节寄存器为0。
	I2C->PRERH = tmp >> 8;

	I2C->CTRL = 0x80 | (I2C_InitStruct->I2C_Mode) | (I2C_InitStruct->I2C_BuslockCheckEn);// 0b10100010
    // 模块工作使能。选择主模式。

	if (I2C_InitStruct->I2C_BuslockCheckEn) {
		I2C->CR_SR = 0x04;
	}
	soc_I2C_wait();
}

/**
  * @brief  Generates I2C communication START condition.
  * @param  I2C: select the I2C peripheral.
  *   		This parameter can be I2C.
  * @param  NewState: new state of the I2C START condition generation.
  *   	    This parameter can be: ENABLE or DISABLE.
  * @retval None.
  */
void soc_I2C_GenerateSTART(FunctionalState NewState) {
    // soc_printf("2\r\n");
    // my_delay_ms(25);
    if (NewState != DISABLE) {
		/* Generate a START condition */
		I2C->CR_SR = I2C_START_Set;
    } else {
		/* Disable the START condition generation */
		I2C->CR_SR = I2C_START_Reset;
    }
}

/**
  * @brief  Generates I2C communication STOP condition.
  * @param  I2C: select the I2C peripheral.
  *   	    This parameter can be I2C.
  * @param  NewState: new state of the I2C STOP condition generation.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None.
  */
void soc_I2C_GenerateSTOP(FunctionalState NewState) {
    // soc_printf("3\r\n");
    // my_delay_ms(25);
    if (NewState != DISABLE) {
		/* Generate a STOP condition */
		//I2C->CR_SR |= I2C_STOP_Set;// ori
		I2C->CR_SR = I2C_STOP_Set;
    }
    else {
		/* Disable the STOP condition generation */
		//I2C->CR_SR &= I2C_STOP_Reset;// ori
		I2C->CR_SR = I2C_STOP_Reset;
    }
}

/**
  * @brief  Sends a data byte through the I2C peripheral.
  * @param  I2C: select the I2C peripheral.
  *         This parameter can be I2C.
  * @param  Data: Byte to be transmitted.
  * @retval None
  */
void soc_I2C_SendData(rt_uint8_t Data) {
  	/* Write in the DR register the data to be sent */
	// soc_I2C_Unlock();
	I2C->DR = Data;
	I2C->CR_SR = 0x10;
}

/**
  * @brief  Returns the most recent received data by the I2C peripheral.
  * @param  I2C: select the I2C peripheral.
  *         This parameter can be I2C.
  * @retval The value of the received data.
  */
void soc_I2C_ReceiveData(FunctionalState ACK, FunctionalState STOP) {
    // soc_I2C_Unlock();
  	/* Return the data in the DR register */
    I2C->CR_SR = 0x20 | ACK | STOP;// 读取 I2C->DR 中的数据
	return;
}

/**
  * @brief  Wait for the Master's transmission is completed.
  * @param  I2C: select the I2C peripheral.
  *   		This parameter can be I2C.
  * @retval None
  */
void soc_I2C_wait() {
    while ((I2C->CR_SR) & I2C_STATE_TIP) {
    }
}

void soc_I2C_wait_available() {
    while ((I2C->CR_SR) & I2C_STATE_BUSY) {
    // 等待传输完毕
    }
}

/**
  * @brief  Check for buslock and recover it if so.
  * @param  I2C: select the I2C peripheral.
  *         This parameter can be I2C.
  * @retval None
  */
void soc_I2C_Unlock() {
    if ((I2C-> CR_SR) & I2C_STATE_Buslock) {
		// 如果总线死锁则满足 if
		I2C->CR_SR = 0x04;// 解除总线死锁
    }
}

void soc_sccb_wait_available() {
    while ((I2C->SCCB_CR_SR) & SCCB_STATE_BUSY) {
    // 等待传输完毕
    }
}

void soc_sccb_wait_power_ready() {
    while (!((I2C->SCCB_CR_SR) & SCCB_STATE_POWER_READY)) {
    // 等待摄像头电源就绪
    }
}


#define SCCB_WRITE   0x00

/**
 * @brief       SCCB接口3阶段写传输
 * @param       id_addr : ID Address
 *              sub_addr: Sub-address
 *              dat     : Write Data
 * @retval      无
 */
void soc_sccb_3_phase_write(rt_uint8_t id_addr, rt_uint16_t sub_addr, rt_uint8_t dat) {
    I2C->SCCB_REG_DATA = ((id_addr << 1) | SCCB_WRITE) << 24 | sub_addr << 8 | dat;
    soc_sccb_wait_available();
}

void soc_sccb_set_config_done_flag(rt_uint8_t done) {
    if (done) {
        I2C->SCCB_CR_SR |= SCCB_STATE_CONFIG_DONE;
    } else {
        I2C->SCCB_CR_SR &= ~SCCB_STATE_CONFIG_DONE;
    }
}

rt_uint8_t soc_sccb_get_config_done_flag() {
    return (I2C->SCCB_CR_SR & SCCB_STATE_CONFIG_DONE) ? 1 : 0;
}
