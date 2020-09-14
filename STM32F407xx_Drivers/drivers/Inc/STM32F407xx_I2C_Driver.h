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
 */
void I2C_Master_Tx(I2C_Handle_t* I2CHandler);
void I2C_Master_Rx(I2C_Handle_t* I2CHandler);

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
uint8_t I2C_CheckStatusFlag(I2C_Reg_t* pI2Cx, uint8_t flag);
/***********************************I2C REGISTER BIT MACROS*****************************/

/*
 * @I2C_CR1 bit macros
 */
#define I2C_CR1_SWRST				15U		//Software Reset
#define I2C_CR1_ALERT				13U		//SMBus Alert
#define I2C_CR1_PEC					12U		//Packet Error Checking
#define I2C_CR1_POS					11U		//Acknowledge/PEC position (for data reception)
#define I2C_CR1_ACK					10U		//Acknowledge enable
#define I2C_CR1_STOP				9U		//Stop generation
#define I2C_CR1_START				8U		//Start generation
#define I2C_CR1_NOSTRETCH			7U		//Clock stretching disable (Slave mode)
#define I2C_CR1_ENGC				6U		//General call enable
#define I2C_CR1_ENPEC				5U		//PEC enable
#define I2C_CR1_ENARP				4U		//ARP enable
#define I2C_CR1_SMBTYPE				3U		//SMBus Type
#define I2C_CR1_SMBUS				1U		//SMBus mode
#define I2C_CR1_PE					0U 		//Peripheral enable

/*
 * @I2C_CR2 bit macros
 */
#define I2C_CR2_LAST				12U		//DMA Last Transfer
#define I2C_CR2_DMAEN				11U		//DMA Request Enable
#define I2C_CR2_ITBUFEN				10U		//Buffer Interrupt Enable
#define I2C_CR2_ITEVTEN				9U		//Event Interrupt Enable
#define I2C_CR2_ITERREN				8U		//Error Interrupt Enable
#define I2C_CR2_FREQ				0U		//Peripheral Clock Frequency

/*
 * @I2C_SR1 bit macros
 */
#define I2C_SR1_SMBALERT			15U		//SMBus Alert
#define I2C_SR1_TIMEOUT				14U		//Timeout or Tlow Error
#define I2C_SR1_PECERR				12U		//PEC Error in Reception
#define I2C_SR1_OVR					11U		//Overrun/underrun
#define I2C_SR1_AF					10U		//Acknowledge Failure
#define I2C_SR1_ARLO				9U		//Arbitration Lost (Master Mode)
#define I2C_SR1_BERR				8U		//Bus Error
#define I2C_SR1_TXE					7U		//Data RegisterEmpty (transmitters)
#define I2C_SR1_RXNE				6U		//Data Register not empty (receivers)
#define I2C_SR1_STOPF				4U		//Stop Detection (Slave Mode)
#define I2C_SR1_ADD10				3U		//10-bit Header Sent (Master Mode)
#define I2C_SR1_BTF					2U		//Byte Transfer Finished
#define I2C_SR1_ADDR				1U		//Address Sent (Master Mode)/matched (slave mode)
#define I2C_SR1_SB					0U		//Start Bit(Master Mode)

/*
 * @I2C_SR2 bit macros
 */
#define I2C_SR2_PEC					8U		//Packet error checking register
#define I2C_SR2_DUALF				7U		//Dual flag(Slave mode)
#define I2C_SR2_SMBHOST				6U		//SMBus host header (Slave mode)
#define I2C_SR2_SMBDEFAULT			5U		//SMBus device default address(Slave mode)
#define I2C_SR2_GENCALL				4U		//General call address (Slave mode)
#define I2C_SR2_TRA					2U		//Transmitter/Receiver
#define I2C_SR2_BUSY				1U		//Bus busy
#define I2C_SR2_MSL					0U		//Master/Slave

/*
 * @I2C_CCR bit macros
 */
#define I2C_CCR_F_S					15U		//I2C master mode selection
#define I2C_CCR_DUTY				14U		//Fm mode duty cycle
#define I2C_CCR_CCR					0U		//Clock control register in Fm/Sm mode (Master Mode)

#endif /* INC_STM32F407XX_I2C_DRIVER_H_ */