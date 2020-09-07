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
 * @I2C_ADDRESSING_MODE macros
 */
#define I2C_ADDR_MODE_7_BIT		0U		//7-bit slave addressing mode
#define I2C_ADDR_MODE_10_BIT	1U  	//10-bit slave addressing mode

/*
 * @I2C_SCL_SPEED
 * Note: STM32F407xx only supports either standard or fast mode
 */
#define I2C_SCL_SPEED_SM		0U		//Standard Mode (100Kbps)
#define I2C_SCL_SPEED_FM		1U		//Fast Mode 	(400Kbps)

#define I2C_SCL_SPEED			(I2C_SCL_SPEED_FM) ? 400000U : 100000U
/*
 * @I2C_ACK_CTRL macros
 */
#define I2C_ACK_EN				ENABLE
#define I2C_ACK_DI				DISABLE

/*
 * @I2C_DUTY_CYCLE
 */
#define I2C_FM_DUTY_2			RESET
#define I2C_FM_DUTY_16_9		SET

/*
 * @I2C Status Flag
 */
#define I2C_FLAG_SR1_SB			(1 << I2C_SR1_SB)
#define I2C_FLAG_SR1_ADDR		(1 << I2C_SR1_ADDR)
#define I2C_FLAG_SR1_TXE		(1 << I2C_SR1_TXE)
#define I2C_FLAG_SR1_RXNE		(1 << I2C_SR1_RXNE)
#define I2C_FLAG_SR1_BTF		(1 << I2C_SR1_BTF)
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
	uint8_t SCLSpeed;		//See @I2C_SCLSPEED macros for more details
	uint8_t DeviceAddress;	//For slave device
	uint8_t ACKControl;		//By default, ACK is not enabled. See @I2C_ACK_CTRL macros for more details
	uint8_t FMDutyCycle; 	//Fast mode duty cycles. See @I2C_DUTY_CYCLE macros for more details
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
 * Enable the I2C peripherals
 */
void I2C_PeripheralEnable(I2C_Reg_t* pI2Cx, uint8_t EnOrDi);

/*
 * I2C Master Tx and Rx
 * Note: len should always be in uint32_t
 */
void I2C_MasterSendData(I2C_Handle_t* pI2CHandler, uint8_t* pTxBuffer, uint32_t len, uint8_t* pSlaveAddress);
void I2C_MasterReceiveData(I2C_Handle_t* pI2CHandler, uint8_t* pRxBuffer, uint32_t len, uint8_t* pSlaveAddress);

/*
 * I2C Slave Tx and Rx
 */
void I2C_Slave_Tx(I2C_Handle_t* I2CHandler);
void I2C_Slave_Rx(I2C_Handle_t* I2CHandler);


/*
 * I2C Interrupt Configuration and Handling
 */
void I2C_IRQITConfig(uint8_t IRQNumber, uint8_t EnOrDi);
void I2C_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriorityValue);

/*
 * Check if the I2C is still busy transmitting bytes of data
 */
uint8_t I2C_CheckStatusFlag(__vo uint32_t* statusReg, uint8_t flag);
//uint8_t I2C_CheckStatusSR2Flag(I2C_Reg_t* pI2Cx, uint8_t flag);


#endif /* INC_STM32F407XX_I2C_DRIVER_H_ */
