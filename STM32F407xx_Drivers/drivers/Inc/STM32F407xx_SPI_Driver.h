/*
 * STM32F407xx_SPI_Driver.h
 *
 *  Created on: Aug 28, 2020
 *      Author: Donavan Tran
 *      Description: This header file contains specific details and implementations
 *      			 of the SPI peripheral interface
 */

#ifndef INC_STM32F407XX_SPI_DRIVER_H_
#define INC_STM32F407XX_SPI_DRIVER_H_
#include "stm32f407xx.h"

/*****************SPECIFIC MACROS FOR SPI*********************/

/*
 * @SPI_DEVICE_MODE
 */
#define SPI_DEVICE_SLAVE_MODE					0U
#define SPI_DEVICE_MASTER_MODE					1U

/*
 * @SPI_BUS
 */
#define SPI_BUS_CONFIG_FULL_DUPLX				0U
#define SPI_BUS_CONFIG_HALF_DUPLX				1U
#define SPI_BUS_CONFIG_SIMPLEX_RX_ONLLY			2U
//#define SPI_BUS_CONFIG_SIMPLEX_TX_ONLY		3U //Similar to full duplex, so no needed

/*
 * DFF SET or RESET
 */
#define SPI_DFF_8_BIT							0U
#define SPI_DFF_16_BIT							1U

/*
 * CPOL SET or RESET
 */
#define	SPI_CPOL_HIGH							SET
#define SPI_CPOL_LOW							RESET

/*
 * CPHA SET or RESET
 */
#define SPI_CPHA_HIGH							SET
#define SPI_CPHA_LOW							RESET

/*
 * @SPI_SSM
 */
#define SPI_HSM									0U	//Default
#define SPI_SSM									1U

/*
 * SPI_SR status flag
 */
#define SPI_TXE_FLAG							(1 << SPI_SR_TXE)
#define SPI_RXNE_FLAG							(1 << SPI_SR_RXNE)
#define SPI_BUSY_FLAG							(1 << SPI_SR_BSY)
#define SPI_OVR_FLAG							(1 << SPI_SR_OVR)

/*
 *  Baud Rate Control Macros
 */
#define SPI_SCLK_SPEED_DIV2						0U
#define SPI_SCLK_SPEED_DIV4						1U
#define SPI_SCLK_SPEED_DIV8						2U
#define SPI_SCLK_SPEED_DIV16					3U
#define SPI_SCLK_SPEED_DIV32					4U
#define SPI_SCLK_SPEED_DIV64 					5U
#define SPI_SCLK_SPEED_DIV128					6U
#define SPI_SCLK_SPEED_DIV256					7U

/*
 * SPI application states
 */
#define SPI_READY								0U
#define SPI_BUSY_IN_RX							1U
#define SPI_BUSY_IN_TX							2U

/*
 * SPI Application event
 */
#define SPI_EVNT_TX_CMPLT						1U
#define SPI_EVNT_RX_CMPLT						2U
#define SPI_EVNT_OVR_ERR						3U
/*************************************************************/
/***************************FUNCTION MACRO********************/
/*
 * SPI Peripheral Clock Enable
 */
#define SPIx_PCLK_EN(__SPIx__) 	((__SPIx__) == SPI1) ? SPI1_PCLK_EN() :\
								((__SPIx__) == SPI2) ? SPI2_PCLK_EN() : SPI3_PCLK_EN()
/*
 * SPI Peripheral Clock Disable
 */
#define SPIx_PCLK_DI(__SPIx__) 	((__SPIx__) == SPI1) ? SPI1_PCLK_DI() :\
								((__SPIx__) == SPI2) ? SPI2_PCLK_DI() : SPI3_PCLK_DI()

/*
 * SPI Enable
 */
#define SPIx_EN(__INDEX__)		(__INDEX__)->CR1 |= (1 << SPI_CR1_SPE)

/*
 * SPI Disable
 */
#define SPIx_DI(__INDEX__)		(__INDEX__)->CR1 &= ~(1 << SPI_CR1_SPE)

/*************************************************************/

/*
 * SPI configuration structure
 */
typedef struct {
	uint8_t  SclkSpeed;
	uint8_t  DeviceMode;	//@SPI_DEVICE_MODE
	uint8_t  BusConfig;		//@SPI_BUS
	uint8_t  SSM;			//Slave Select Management
	uint8_t  DFF;   		//Data Frame Format
	uint8_t  CPOLConfig;  		//Clock Polarity
	uint8_t  CPHAConfig;  		//Clock Phase
} SPI_Config_t;


/*
 * SPI Handle structure
 */
typedef struct {
	SPI_Reg_t*		pSPIx; 			//Base address of the SPI peripherals
	SPI_Config_t	SPI_Config;
	uint8_t*		pTxBuffer;		//Global Tx buffer pointer to store the buffer address
	uint8_t*		pRxBuffer;		//Global Rx buffer pointer to store the buffer address
	uint32_t		TxLen;			//global length information of the Tx buffer
	uint32_t 		RxLen;			//global length information of the Rx buffer
	uint8_t			TxState;		//global state of Tx
	uint8_t 		RxState;		//global state of Rx
} SPI_Handle_t;

/********************************SPI FUNCTION API DECLARATION*************************/

/*
 * Peripheral clock control
 */
void SPI_PeriClkCtrl(SPI_Reg_t* pSPIx, uint8_t EnOrDi);

/*
 * SPI initialization and de-initialization
 * Parameter: Pointer to the GPIO Handle Structure
 */
void SPI_Init(SPI_Handle_t* pSPIHandler);

/*
 * Enable the SPI peripherals
 */
void SPI_PeripheralEnable(SPI_Reg_t* pSPIx, uint8_t EnOrDi);

/* Consult the RCC Peripheral reset registers for more details*/
void SPI_DeInit(SPI_Reg_t* pSPIx);

/*
 * SPI Send and Receive API
 * Note: blocking is non-interrupt-based and non-blocking is interrupt-based send and receive method
 */
//Blocking-based API
void SPI_SendData(SPI_Reg_t* pSPIx, uint8_t* pTxBuffer, uint32_t len); //Note: it is a standard practice to define len as uint32_t
void SPI_ReceiveData(SPI_Reg_t* pSPIx, uint8_t* pRxBuffer, uint32_t len);

/*
 * SPI Send and Receive API using non-blocking method (interrupt approaches)
 */
uint8_t SPI_SendDataIT(SPI_Handle_t* pSPIHandler, uint8_t* pTxBuffer, uint32_t len); //Note: it is a standard practice to define len as uint32_t
uint8_t SPI_ReceiveDataIT(SPI_Handle_t* pSPIHandler, uint8_t* pRxBuffer, uint32_t len);

/*
 * SPI Interrupt Configuration and Handling
 */
void SPI_IRQITConfig(uint8_t IRQNumber, uint8_t EnOrDi);
void SPI_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriorityValue);
void SPI_IRQHandling(SPI_Handle_t* pSPIHandler);

/*
 * Other controllers API
 */

/*
 * Check if the SPI is still busy transmitting bytes of data
 */
void SPI_ClearOVRFlag(SPI_Reg_t* pSPIx);
void SPI_CloseTransmission(SPI_Handle_t* pSPIHandler);
void SPI_CloseReception(SPI_Handle_t* pSPIHandler);

/*
 * Application callback
 */
void SPI_ApplicationEvent(SPI_Handle_t* pSPIHandler, uint8_t appEvent);
#endif /* INC_STM32F407XX_SPI_DRIVER_H_ */
