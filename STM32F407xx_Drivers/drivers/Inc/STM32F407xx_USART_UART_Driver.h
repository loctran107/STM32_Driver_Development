/*
 * STM32F407xx_USART_UART_Driver.h
 *
 *  Created on: Sep 16, 2020
 *      Author: Donavan Tran
 */

#ifndef INC_STM32F407XX_USART_UART_DRIVER_H_
#define INC_STM32F407XX_USART_UART_DRIVER_H_

/***************************USART_CONFIG_MACROS******************/

/*
 * @USART_BAUD_RATE macors
 */
#define USART_STD_BAUD_1200			1200U
#define USART_STD_BAUD_2400			2400U
#define USART_STD_BAUD_9600			9600U
#define USART_STD_BAUD_19200		19200U
#define USART_STD_BAUD_38400		38400U
#define USART_STD_BAUD_57600		57600U
#define USART_STD_BAUD_115200		115200U
#define USART_STD_BAUD_230400		203400U
#define USART_STD_BAUD_460800		460800U
#define USART_STD_BAUD_921600		921600U
#define USART_STD_BAUD_2M			2000000U
#define USART_STD_BAUD_3M			3000000U

/*
 * @USART_MODE macros
 */
#define USART_MODE_ONLY_TX			0U
#define USART_MODE_ONLY_RX			1U
#define USART_MODE_TX_RX			2U

/*
 * @USART_WORDLEN macros
 */
#define USART_WORDLEN_8BITS			0U
#define USART_WORDLEN_9BITS			1U

/*
 * @USART_PARITY_CONTROL
 */
#define USART_PARITY_EN_EVEN		0U
#define USART_PARITY_EN_ODD			1U
#define USART_PARITY_DI				2U

/*
 * @USART_NO_STOP_BITS
 */
#define USART_STOPBITS_1			0U
#define USART_STOPBITS_0_5			1U
#define USART_STOPBITS_2			2U
#define USART_STOPBITS_1_5			3U

/*
 * @USART_HW_FLOW_CTRL
 */
#define USART_HW_FLOW_CTRL_NONE		0U
#define USART_HW_FLOW_CTRL_CTS		1U
#define USART_HW_FLOW_CTRL_RTS		2U
#define USART_HW_FLOW_CTRL_CTS_RTS	3U

/*
 * @USART_OVERSAMPLING macros
 */
#define USART_OVERSAMPLING_BY_8		1U  //standard USART mode (SPI included)
#define USART_OVERSAMPLING_BY_16	0U	//Smartcard, LIN, and IrDA mode

/*
 * @USART Status flags
 */
#define USART_FLAG_SR_TXE			(1 << USART_SR_TXE)
#define USART_FLAG_SR_RXNE			(1 << USART_SR_RXNE)
#define USART_FLAG_SR_TC			(1 << USART_SR_TC)
#define USART_FLAG_SR_CTS			(1 << USART_SR_CTS)
#define USART_FLAG_SR_PE			(1 << USART_SR_PE)
#define USART_FLAG_SR_LBD			(1 << USART_SR_LBD)
#define USART_FLAG_SR_IDLE			(1 << USART_SR_IDLE)
#define USART_FLAG_SR_ORE			(1 << USART_SR_ORE)
#define USART_FLAG_SR_NF			(1 << USART_SR_NF)
#define USART_FLAG_SR_FE			(1 << USART_SR_FE)
/*
 * @USART state
 */
#define USART_READY					0U
#define USART_BUSY_IN_TX			1U
#define USART_BUSY_IN_RX			2U
/****************************USART_FUNCTION_MACROS******************/
/*
 * I2C Peripheral Clock Enable
 */
#define USARTx_PCLK_EN(__USARTx__) 	((__USARTx__) == USART1) ? USART1_PCLK_EN() :\
								    ((__USARTx__) == USART2) ? USART2_PCLK_EN() :\
								    ((__USARTx__) == USART3) ? USART3_PCLK_EN() :\
								    ((__USARTx__) == UART4)  ? UART4_PCLK_EN()	:\
								    ((__USARTx__) == UART5)  ? UART5_PCLK_EN()	: USART6_PCLK_EN()
/*
 * I2C Peripheral Clock Disable
 */
#define USARTx_PCLK_DI(__USARTx__) 	((__USARTx__) == USART1) ? USART1_PCLK_DI() :\
								    ((__USARTx__) == USART2) ? USART2_PCLK_DI() :\
								    ((__USARTx__) == USART3) ? USART3_PCLK_DI() :\
								    ((__USARTx__) == UART4)  ? UART4_PCLK_DI()	:\
								    ((__USARTx__) == UART5)  ? UART5_PCLK_DI()	: USART6_PCLK_DI()



/*******************************************************************/
/*
 * Configuration structure of USART
 */
typedef struct {
	uint32_t BaudRate;		//See @USART_BAUD_RATE macros for details
	uint8_t  Mode;			//See @USART_MODE macros for details
	uint8_t  NoOfStopBits; 	//See @UART_NO_STOP_BITS macros for details
	uint8_t  WordLength; 	//See @USART_WORD_LENGTH for details
	uint8_t  ParityControl; //See @USART_PARITY_CONTROl for details
	uint8_t  HWFlowControl; //See @USART_HW_FLOW_CTRL macros for details
	uint8_t  Oversampling;	//See @USART_OVERSAMPLING macros for details
} USART_Config_t;

/*
 * Handle structure of USART
 */
typedef struct {
	USART_Reg_t* pUSARTx;
	USART_Config_t USART_Config;
	uint8_t* pTxBuffer;
	uint8_t* pRxBuffer;
	uint32_t TxLen;
	uint32_t RxLen;
	uint8_t  TxState;
	uint8_t  RxState;
} USART_Handle_t;

/*******************************USART_API************************/

/*
 * Peripheral clock control
 */
void USART_PeriClkCtrl(USART_Reg_t* pUSARTx, uint8_t EnOrDi);

/*
 * USART initialization and de-initialization
 * Parameter: Pointer to the USART Handle Structure
 */
void USART_Init(USART_Handle_t* pUSARTHandler);

/* Consult the RCC Peripheral reset registers for more details*/
void USART_DeInit(USART_Reg_t* pUSARTx);

/*
 * Enable the USART/UART peripherals
 */
void USART_PeripheralEnable(USART_Reg_t* pUSARTx, uint8_t EnOrDi);

/*
 * USART Send and Receive APIs
 */
void USART_SendData(USART_Handle_t* pUSARTHandler, uint8_t* pTxBuffer, uint32_t len);
void USART_ReceiveData(USART_Handle_t* pUSARTHandler, uint8_t* pRxBuffer, uint32_t len);

/*
 * USART Send and Receive IT APIs
 */
uint8_t USART_SendDataIT(USART_Handle_t* pUSARTHandler, uint8_t* pTxBuffer, uint32_t len);
uint8_t USART_ReceiveDataIT(USART_Handle_t* pUSARTHandler, uint8_t* pRxBuffer, uint32_t len);


/*
 * USART Interrupt Configuration and Handling
 */
void USART_IRQITConfig(uint8_t IRQNumber, uint8_t EnOrDi);
void USART_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriorityValue);
void USART_IRQHandling(USART_Handle_t* pUSARTHandler);
/*
 * Check if the USART/UART is still busy transmitting bytes of data
 */
uint8_t USART_CheckStatusFlag(__vo uint32_t* statusReg, uint16_t flag);

/*
 * Other supporting APIs
 */
void USART_ApplicationEventCallback(USART_Handle_t* pUSARTHandler, uint8_t appEvnt);
#endif /* INC_STM32F407XX_USART_UART_DRIVER_H_ */
