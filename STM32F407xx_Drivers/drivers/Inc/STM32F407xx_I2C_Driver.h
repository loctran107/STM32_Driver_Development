/*
 * STM32F407xx_I2C_Driver.h
 *
 *  Created on: Sep 3, 2020
 *      Author: Donavan Tran
 */

#ifndef INC_STM32F407XX_I2C_DRIVER_H_
#define INC_STM32F407XX_I2C_DRIVER_H_

#include "stm32f407xx.h"
/*
 * @I2C_MODE_SELECTION macros
 */
#define I2C_MODE_SLAVE_TX		0U		//Slave Transmitter
#define I2C_MODE_SLAVE_RX  		1U		//Slave Receiver
#define I2C_MODE_MASTER_TX 		2U		//Master Transmitter
#define I2C_MODE_MASTER_RX 		3U		//Master Receiver

/*
 * @I2C_BUS_FREQ_MODE macros
 * Note: STM32F407xx only supports either standard or fast mode
 */
#define I2C_BUS_FREQ_SM			0U		//Standard Mode
#define I2C_BUS_FREQ_FM			1U		//Fast Mode

/*
 * @I2C_ADDRESSING_MODE macros
 */
#define I2C_ADDR_MODE_7_BIT		0U		//7-bit slave addressing mode
#define I2C_ADDR_MODE_10_BIT	1U  	//10-bit slave addressing mode

/*******************************I2C FUNCTION MACROS**************************************/
/*
 * I2C Peripheral Clock Enable
 */
#define I2Cx_PCLK_EN(__I2Cx__) 	((__I2Cx__) == I2C1) ? I2C1_PCLK_EN() :\
								((__I2Cx__) == I2C2) ? I2C2_PCLK_EN() : I2C3_PCLK_EN()
/*
 * I2C Peripheral Clock Disable
 */
#define I2Cx_PCLK_DI(__I2Cx__) 	((__I2Cx__) == I2C1) ? I2C1_PCLK_DI() :\
								((__I2Cx__) == I2C2) ? I2C2_PCLK_DI() : I2C3_PCLK_DI()

/****************************************************************************************/
/*
 * I2C configuration structure
 */
typedef struct {
	uint8_t Mode;				//Note: by default, I2C operates in Slave mode. See @I2C_MODE_SELECTION macro
	uint8_t BusFrequencyMode;	//See @I2C_BUS_FREQ_MODE macros for more details
	uint8_t PseriClkFreq;		//2MHz-50MHz
	uint8_t AddressingMode;		//See @I2C_ADDRESSING_MODE macros for more details
} I2C_Config_t;

/*s
 * Handle structure of I2C peripherals
 */
typedef struct {
	I2C_Reg_t*		pI2Cx; 		//I2Cx peripheral base address
	I2C_Config_t	I2C_Config;	//I2C configuration structure
} I2C_Handle_t;


/***********************************I2C API PROTOTYPES**************************************/
/*
 * Peripheral clock control
 */
void I2C_PeriClkCtrl(I2C_Reg_t* pI2Cx, uint8_t EnOrDi);

/*
 * SPI initialization and de-initialization
 * Parameter: Pointer to the GPIO Handle Structure
 */
void I2C_Init(I2C_Handle_t* pI2CHandler);

/* Consult the RCC Peripheral reset registers for more details*/
void I2C_DeInit(I2C_Reg_t* pI2Cx);

/*
 * I2C Master Tx and Rx
 */
void I2C_Master_Tx(I2C_Handle_t* I2CHandler);
void I2C_Master_Rx(I2C_Handle_t* I2CHandler);


/*
 * I2C Slave Tx and Rx
 */
void I2C_Slave_Tx(I2C_Handle_t* I2CHandler);
void I2C_Slave_Rx(I2C_Handle_t* I2CHandler);

#endif /* INC_STM32F407XX_I2C_DRIVER_H_ */
