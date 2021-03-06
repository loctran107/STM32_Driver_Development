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
 * @I2C_APPLICATION_STATES
 */
#define I2C_READY				0U
#define I2C_BUSY_IN_RX			1U
#define I2C_BUSY_IN_TX			2U

/*
 * @I2C_SR (Repeated Start)
 */
#define I2C_SR_SET				SET
#define I2C_SR_RESET			RESET
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
#define I2C_SCL_SPEED(__SCL_SPEED__)    ((__SCL_SPEED__) ==  I2C_SCL_SPEED_FM) ? 400000U : 100000U

/*
 * Maximum Rise time in Sm/Fm mode (microsecond)
 * Note: Maximum rise time of Sm is 1000ns = 1 microseconds = 1/1000000 s
 * 	     Maximum rise time of Fm is 300ns = 0.3 microseconds = 1/300000 s
 */
#define I2C_T_RISE(__SCL_SPEED__) 		((__SCL_SPEED__) == I2C_SCL_SPEED_FM) ? 3U : 10U

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
 * @I2C SR1 Status Flag
 */
#define I2C_FLAG_SR1_SB			(1 << I2C_SR1_SB)
#define I2C_FLAG_SR1_ADDR		(1 << I2C_SR1_ADDR)
#define I2C_FLAG_SR1_TXE		(1 << I2C_SR1_TXE)
#define I2C_FLAG_SR1_RXNE		(1 << I2C_SR1_RXNE)
#define I2C_FLAG_SR1_BTF		(1 << I2C_SR1_BTF)
#define I2C_FLAG_SR1_TIMEOUT	(1 << I2C_SR1_TIMEOUT)
#define I2C_FLAG_SR1_PECERR		(1 << I2C_SR1_PECERR)
#define I2C_FLAG_SR1_OVR		(1 << I2C_SR1_OVR)
#define I2C_FLAG_SR1_AF			(1 << I2C_SR1_AF)
#define I2C_FLAG_SR1_ARLO		(1 << I2C_SR1_ARLO)
#define I2C_FLAG_SR1_BERR		(1 << I2C_SR1_BERR)
#define I2C_FLAG_SR1_STOPF		(1 << I2C_SR1_STOPF)
#define I2C_FLAG_SR1_ADD10		(1 << I2C_SR1_ADD10)
#define I2C_FLAG_SR1_SMBALERT	(1 << I2C_SR1_SMBALERT)

/*
 * @I2C SR2 Status flag
 */
#define I2C_FLAG_SR2_SML		(1 << I2C_SR2_MSL)
#define I2C_FLAG_SR2_TRA		(1 << I2C_SR2_TRA)
/*
 * @I2C Application Event Status
 */
#define I2C_EVT_TX_CMPLT		0U
#define I2C_EVT_RX_CMPLT		1U
#define I2C_EVT_STOPF_CMPLT		2U
#define I2C_ERR_BERR			3U
#define I2C_ERR_AF				4U
#define I2C_ERR_ARLO			5U
#define I2C_ERR_OVR				6U
#define I2C_ERR_PECERR			7U
#define I2C_ERR_TIMEOUT			8U
#define I2C_ERR_SMBALERT		9U
#define I2C_EVT_DATA_REQ		10U
#define I2C_EVT_DATA_RCV		11U

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
	I2C_Reg_t*		pI2Cx; 			//I2Cx peripheral base address
	uint8_t*		pTxBuffer;		//pointer to Tx buffer
	uint8_t*		pRxBuffer;		//pointer to Rx buffer
	I2C_Config_t	I2C_Config;		//I2C configuration structure
	uint32_t 		TxLen;			//length of Tx buffer
	uint32_t 		RxLen;			//length of Rx buffer
	uint32_t 		RxSize;			//Size of the Rx buffer
	uint8_t 		TxRxState;		//since I2C is half-duplex in STM32, we only need 1 state
	uint8_t 		RepeatedStart;	//repeated Start condition
	uint8_t 		DeviceAddr;		//storing the device address
} I2C_Handle_t;


/***********************************I2C API PROTOTYPES**************************************/
/*
 * Peripheral clock control
 */
void I2C_PeriClkCtrl(I2C_Reg_t* pI2Cx, uint8_t EnOrDi);

/*
 * I2C initialization and de-initialization
 * Parameter: Pointer to the I2C Handle Structure
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
 * 		 The interrupt API returns the application states
 */
void I2C_MasterSendData(I2C_Handle_t* pI2CHandler, uint8_t* pTxBuffer, uint32_t len,
						uint8_t pSlaveAddress, uint8_t repeatedStart);
void I2C_MasterReceiveData(I2C_Handle_t* pI2CHandler, uint8_t* pRxBuffer, uint32_t len,
		 	 	 	 	   uint8_t pSlaveAddress, uint8_t repeatedStart);

void I2C_SlaveSendData(I2C_Reg_t* pI2Cx, uint8_t data);
uint8_t I2C_SlaveReceiveData(I2C_Reg_t* pI2Cx);

/*
 * I2C Master Tx and RX interrupt API
 */
uint8_t I2C_MasterSendDataIT(I2C_Handle_t* pI2CHandler, uint8_t* pTxBuffer, uint32_t len,
						uint8_t pSlaveAddress, uint8_t repeatedStart);
uint8_t I2C_MasterReceiveDataIT(I2C_Handle_t* pI2CHandler, uint8_t* pRxBuffer, uint32_t len,
		 	 	 	 	     uint8_t pSlaveAddress, uint8_t repeatedStart);
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
 * I2C Event and Error IRQ Handling
 */
void I2C_EV_IRQHandling(I2C_Handle_t* pI2CHandler);
void I2C_ER_IRQHandling(I2C_Handle_t* pI2CHandler);
void I2C_InterruptCtrl(I2C_Reg_t* pI2Cx, uint8_t EnOrDi);

/*
 * Check if the I2C is still busy transmitting bytes of data
 */
uint8_t I2C_CheckStatusFlag(__vo uint32_t* statusReg, uint16_t flag);

/*
 * Other application APIs
 */
void I2C_ApplicationEventCallBack(I2C_Handle_t* pI2CHandler, uint8_t appEvt);


#endif /* INC_STM32F407XX_I2C_DRIVER_H_ */
