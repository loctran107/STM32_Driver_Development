/*
 * STM32F407xx_USART_UART_Driver.c
 *
 *  Created on: Sep 16, 2020
 *      Author: Donavan Tran
 */

#include "../drivers/Inc/stm32f407xx.h"

/*
 * Helper functions that are private to user applications
 */
static uint32_t getAPBxClkFreq(USART_Reg_t* pUSARTx);
static uint32_t getPLLClkFreq();
static uint32_t getUSARTDiv(USART_Handle_t* pUSARTHandler);
static void TXEInterruptHandler(USART_Handle_t* pUSARTHandler);
static void RXNEInterruptHandler(USART_Handle_t* pUSARTHandler);

/*****************************************************
 * @fn					- USART_PeriClkCtrl
 *
 * @brief				- Enable or disable the USARTx clock peripherals
 *
 * @param[in]			- Base address of the specific USART
 * @param[in]			- ENABLE or DISABLE macro
 *
 * @return				- none
 * @note				- none
 */
void USART_PeriClkCtrl(USART_Reg_t* pUSARTx, uint8_t EnOrDi) {
	if (EnOrDi) {
		USARTx_PCLK_EN(pUSARTx); //Go to I2Cx_PCLK_EN macro for more details
	} else {
		USARTx_PCLK_DI(pUSARTx); //Go to I2Cx_PCLK_DI macro for more details
	}
}

/*****************************************************
 * @fn					- USART_Init
 *
 * @brief				- Initialize the USART port given the handle structure
 *
 * @param[in]			- Handle Structure of USART that contains all USART configuration
 * 						  and port
 *
 * @return				- none
 * @note				- none
 */
void USART_Init(USART_Handle_t* pUSARTHandler) {
	uint32_t USARTDiv;
	uint16_t mantissa, temp;
	uint8_t fraction;
	//Enable the peripheral clock
	USART_PeriClkCtrl(pUSARTHandler->pUSARTx, ENABLE);


	//Configure USART mode
	switch (pUSARTHandler->USART_Config.Mode) {
	case USART_MODE_ONLY_TX:	//Set the TE bit in USART_CR to send an idle frame as the first transmission
								pUSARTHandler->pUSARTx->CR1 |= 1 << USART_CR1_TE;
								break;

	case USART_MODE_ONLY_RX:	//Set the TE bit in USART_CR to send an idle frame as the first transmission
								pUSARTHandler->pUSARTx->CR1 |= 1 << USART_CR1_RE;
								break;

	case USART_MODE_TX_RX:		pUSARTHandler->pUSARTx->CR1 |= 1 << USART_CR1_TE;
								pUSARTHandler->pUSARTx->CR1 |= 1 << USART_CR1_RE;
								break;

	}
	//Configure the parity control
	if (pUSARTHandler->USART_Config.ParityControl != USART_PARITY_DI) {

		//Enable the Parity control
		pUSARTHandler->pUSARTx->CR1 |= 1 << USART_CR1_PCE;

		//Set the parity bit control
		pUSARTHandler->pUSARTx->CR1 |= pUSARTHandler->USART_Config.ParityControl << USART_CR1_PS;

	}

	//Configure the over-sampling
	pUSARTHandler->pUSARTx->CR1 |= pUSARTHandler->USART_Config.Oversampling << USART_CR1_OVER8;

	//Configure the word length
	pUSARTHandler->pUSARTx->CR1 |= pUSARTHandler->USART_Config.WordLength << USART_CR1_M;

	//Configure the number of stop bits
	pUSARTHandler->pUSARTx->CR2 |= pUSARTHandler->USART_Config.NoOfStopBits << USART_CR2_STOP;

	//Configure the HW flow control
	if (pUSARTHandler->USART_Config.HWFlowControl != USART_HW_FLOW_CTRL_NONE) {

		//Enable CTS bit
		if (pUSARTHandler->USART_Config.HWFlowControl == USART_HW_FLOW_CTRL_CTS) {
			pUSARTHandler->pUSARTx->CR3 |= pUSARTHandler->USART_Config.HWFlowControl << USART_CR3_CTSE;
		}
		//Enable RTS bit
		else if (pUSARTHandler->USART_Config.HWFlowControl == USART_HW_FLOW_CTRL_RTS) {
			pUSARTHandler->pUSARTx->CR3 |= pUSARTHandler->USART_Config.HWFlowControl << USART_CR3_RTSE;
		}
		//Enable both RTS and CTS
		else {
			pUSARTHandler->pUSARTx->CR3 |= pUSARTHandler->USART_Config.HWFlowControl << USART_CR3_CTSE;
			pUSARTHandler->pUSARTx->CR3 |= pUSARTHandler->USART_Config.HWFlowControl << USART_CR3_RTSE;
		}
	}

	//Baud rate configuration
	USARTDiv = getUSARTDiv(pUSARTHandler);

	//Note: Floating point calculation can be fatal sometimes.
	// 		Thus, avoid using it only when necessary.
	//		Both solution using floating point and normal calculation would yield
	//		the same result, however, one tends to be safer and less complex than
	//		the others
	//FIRST APPROACH uses fclk / 8 * baudrate

	/*
	float temp1;
	uint16_t temp2;
	mantissa = (uint16_t) USARTDiv;
	temp1 = USARTDiv - mantissa;
	temp1 *= (pUSARTHandler->USART_Config.Oversampling == USART_OVERSAMPLING_BY_8) ? 8U : 16U;

	//Check for round up or down
	temp2 = temp1 * 10U;
	temp2 = (temp2 % ((temp2 / 10U) * 10U)) >= 5 ? ceil(temp1) : floor(temp1);
	if (temp2 > 15U) {
		//Add the carry to mantissa
		mantissa += temp2 - 15U;
	}
	fraction = (uint8_t) temp2;*/

	//SECOND APPROACH
	mantissa = USARTDiv / 100U;
	temp = USARTDiv - (mantissa * 100U);
	temp *= (pUSARTHandler->USART_Config.Oversampling == USART_OVERSAMPLING_BY_8) ? 8U : 16U;
	temp = (temp + 50U) / 100U; //always round up
	if (temp > 15U) {
		mantissa += temp - 15U;
		fraction = 15U;
	} else {
		fraction = temp;
	}
	pUSARTHandler->pUSARTx->BRR |= mantissa << USART_BRR_DIV_MANTISSA;
	pUSARTHandler->pUSARTx->BRR |= fraction << USART_BRR_DIV_FRACTION;
}

/*****************************************************
 * @fn					- USART_DeInit
 *
 * @brief				- Reset the entire register configuration of the specific USART port
 *
 * @param[in]			- Base address of the specific SPI peripherals (USART_Reg_t* pUSARTx)
 *
 * @return				- none
 * @note				- The implementation so far only covers only 6 USART/USART ports
 */
void USART_DeInit(USART_Reg_t* pUSARTx) {
	if (pUSARTx == USART1) {
		USART1_PCLK_RST();
	} else if (pUSARTx == USART2) {
		USART2_PCLK_RST();
	} else if (pUSARTx == USART3) {
		USART3_PCLK_RST();
	} else if (pUSARTx == UART4) {
		UART4_PCLK_RST();
	} else if (pUSARTx == UART5) {
		UART5_PCLK_RST();
	} else {
		USART6_PCLK_RST();
	}
}

/*****************************************************
 * @fn					- USART_PeripheralEnable
 *
 * @brief				- Enable that specific USART peripheral
 *
 * @param[in]			- Base address of the specific USART peripherals (USART_Reg_t* pUSARTx)
 * @param[in]			= ENABLE or DISABLE macro
 *
 * @return				- none
 * @note				- none
 */
void USART_PeripheralEnable(USART_Reg_t* pUSARTx, uint8_t EnOrDi) {
	if (EnOrDi) {
		pUSARTx->CR1 |= 1 << USART_CR1_UE;
	} else {
		pUSARTx->CR1 &= ~(1 << USART_CR1_UE);
	}
}

/*****************************************************
 * @fn					- USART_SendData
 *
 * @brief				- Send data to other connected USART/UART device
 *
 * @param[in]			- Base address of the specific USART peripherals (USART_Reg_t* pUSARTx)
 * @param[in]			- buffer for transmission (TxBuffer)
 * @param[in]			- length of the buffer (len)
 *
 * @return				- none
 * @note				- See the MCU Reference Manual for more details
 * 						  This is a polling (non-interrupt) approach
 */
void USART_SendData(USART_Handle_t* pUSARTHandler, uint8_t* pTxBuffer, uint32_t len) {

	while (len) {
		//Poll until TXE is set
		while (!USART_CheckStatusFlag(&pUSARTHandler->pUSARTx->SR, USART_FLAG_SR_TXE));

		//Check if the word length is 8 or 9-bit data frame
		if (pUSARTHandler->USART_Config.WordLength == USART_WORDLEN_9BITS) {

			//2 bytes of data will be sent, 9-bit masking is necessary
			pUSARTHandler->pUSARTx->DR = *((uint16_t*) pTxBuffer) & ((uint16_t) 0x1FF);

			//When parity bit is disabled, the data sent is 2 bytes
			//For more, see page 991 in MCU Reference Manual
			if (pUSARTHandler->USART_Config.ParityControl == USART_PARITY_DI) {
				(uint16_t*) pTxBuffer++;
			} else {
				//Parity bit is used in this transfer. Thus, 8 bits of user data
				//will be sent. The 9th bit will be replaced with parity bit
				//by hardware
				pTxBuffer++;
			}
		} else {
			//Write buffer into the data register at 8-bit data frame
			pUSARTHandler->pUSARTx->DR = *pTxBuffer & ((uint8_t) 0xFF);
			pTxBuffer++;
		}
		len--;
	}

	//Wait until the transmission is complete
	while (!USART_CheckStatusFlag(&pUSARTHandler->pUSARTx->SR, USART_FLAG_SR_TC));

}

/*****************************************************
 * @fn					- USART_ReceiveData
 *
 * @brief				- Receive data from other connected USART/UART device
 *
 * @param[in]			- Base address of the specific USART peripherals (USART_Reg_t* pUSARTx)
 * @param[in]			- buffer for reception (RxBuffer)
 * @param[in]			- length of the buffer (len)
 *
 * @return				- none
 * @note				- See the MCU Reference Manual for more details
 * 						  This is the polling (non-interrupt) approach
 */
void USART_ReceiveData(USART_Handle_t* pUSARTHandler, uint8_t* pRxBuffer, uint32_t len) {

	while (len) {
		//Poll until TXE is set
		while (!USART_CheckStatusFlag(&pUSARTHandler->pUSARTx->SR, USART_FLAG_SR_RXNE));

		//Check if the word length is 8 or 9-bit data frame
		if (pUSARTHandler->USART_Config.WordLength == USART_WORDLEN_9BITS) {

			//When parity bit is disabled, 9-bit of data will be received and no parity bit
			if (pUSARTHandler->USART_Config.ParityControl == USART_PARITY_DI) {
				//2 bytes of data will be sent, 9-bit masking is necessary
				*((uint16_t*) pRxBuffer) = pUSARTHandler->pUSARTx->DR & ((uint16_t) 0x1FF);
				(uint16_t*) pRxBuffer++;
			} else {
				//Parity bit is used in this transfer. Thus, 8 bits of user data
				//will be received. The 9th bit will be replaced with parity bit
				//by hardware
				*pRxBuffer = pUSARTHandler->pUSARTx->DR & ((uint16_t) 0xFF);
				pRxBuffer++;
			}
		} else {
			//In 8-bit data frame, if parity bit is disabled, 8 bits will
			//be received
			if (pUSARTHandler->USART_Config.ParityControl == USART_PARITY_DI) {
				//Write buffer into the data register at 8-bit data frame
				*pRxBuffer = pUSARTHandler->pUSARTx->DR & ((uint8_t) 0xFF);
			} else {
				//if parity bit is enabled, only 7 bit will be received
				*pRxBuffer = pUSARTHandler->pUSARTx->DR & ((uint8_t) 0x7F);
			}
			pRxBuffer++;
		}
		len--;
	}
}

/*****************************************************
 * @fn					- USART_SendDataIT
 *
 * @brief				- Send data to other connected USART/UART device
 *
 * @param[in]			- Base address of the specific USART peripherals (USART_Reg_t* pUSARTx)
 * @param[in]			- buffer for transmission (TxBuffer)
 * @param[in]			- length of the buffer (len)
 *
 * @return				- none
 * @note				- See the MCU Reference Manual for more details
 * 						  This is a non-polling (interrupt) approach
 */
uint8_t USART_SendDataIT(USART_Handle_t* pUSARTHandler, uint8_t* pTxBuffer, uint32_t len) {
	uint8_t currState;
	currState = pUSARTHandler->TxState;
	if (currState != USART_BUSY_IN_TX) {

		//Load Txbuffer and len as global
		pUSARTHandler->pTxBuffer = pTxBuffer;
		pUSARTHandler->TxLen = len;
		pUSARTHandler->TxState = USART_BUSY_IN_TX;

		//Enable the TXEIE and TCIE interrupt
		pUSARTHandler->pUSARTx->CR1 |= 1 << USART_CR1_TXEIE;
		pUSARTHandler->pUSARTx->CR1 |= 1 << USART_CR1_TCIE;

		if (pUSARTHandler->USART_Config.HWFlowControl == USART_HW_FLOW_CTRL_CTS) {
			pUSARTHandler->pUSARTx->CR3 |= 1 << USART_CR3_CTSIE;
		}
	}
	return currState;
}

/*****************************************************
 * @fn					- USART_ReceiveDataIT
 *
 * @brief				- Receive data from other connected USART/UART device
 *
 * @param[in]			- Base address of the specific USART peripherals (USART_Reg_t* pUSARTx)
 * @param[in]			- buffer for reception (RxBuffer)
 * @param[in]			- length of the buffer (len)
 *
 * @return				- none
 * @note				- See the MCU Reference Manual for more details
 * 						  This is the non-polling (interrupt) approach
 */
uint8_t USART_ReceiveDataIT(USART_Handle_t* pUSARTHandler, uint8_t* pRxBuffer, uint32_t len) {
	uint8_t currState;
	currState = pUSARTHandler->RxState;
	if (currState != USART_BUSY_IN_RX) {

		//Load Rxbuffer and len as global
		pUSARTHandler->pTxBuffer = pRxBuffer;
		pUSARTHandler->TxLen = len;
		pUSARTHandler->RxState = USART_BUSY_IN_RX;

		//Enable the RXNEIE and PE interrupt (PE as Parity Error)
		pUSARTHandler->pUSARTx->CR1 |= 1 << USART_CR1_RXNEIE;
		pUSARTHandler->pUSARTx->CR1 |= 1 << USART_CR1_PEIE;

	}
	return currState;
}

/*****************************************************
 * @fn					- USART_IRQITConfig
 *
 * @brief				- USART Interrupt configuration
 *
 * @param[in]			- IRQ Number of the USART interrupt peripheral
 * @param[in]			= ENABLE or DISABLE macro
 *
 * @return				- none
 * @note				- Refer to the Cortex M4 Generic User Guide the NVIC register table
 */
void USART_IRQITConfig(uint8_t IRQNumber, uint8_t EnOrDi) {
	//In order to trigger the interrupt on the processor side,
	//configuration enable on the ISER of the NVIC is needed
	//Note: There are 7 different NVIC_ISER and NVIC_ICER register
	//at certain range
	uint32_t indx, remainder;
	indx = IRQNumber >> 5U; //Index to configure the correct NVIC_ISER
	if (indx >= 0U  && indx <= 7U) {
		remainder = IRQNumber & (~(~(int)0 << 5U)); //find the remainder
		if (EnOrDi) {
			NVIC_ISER(indx) |= 1 << remainder; //See NVIC_ISER(__INDEX__) declaration for more implementation details
		} else {
			NVIC_ICER(indx) |= 1 << remainder; //See NVIC_ICER(__INDEX__) declaration for more implementation details
		}
	}
}

/*****************************************************
 * @fn					- USART_IRQPriorityConfig
 *
 * @brief				- USART Interrupt Priority Configuration
 *
 * @param[in]			- IRQ number of the I2C interrupt peripheral
 * @param[in]			= The priority value of that USART interrupt peripheral
 *
 * @return				- none
 * @note				- Refer to the Cortex M4 Generic User Guide the NVIC register table
 */
void USART_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriorityValue) {
	uint32_t indx = IRQNumber >> 2U; //Note: There are IRQ Priority field in each IPR register
	uint32_t remainder = IRQNumber & (~(~(int)0 << 2U));
	uint32_t shift_amount = (remainder * 8U) + IMPLEMENTED_IRQ_PRIORITY_BIT;

	//Configure the IRQ_PR register
	NVIC_IPR(indx) |= IRQPriorityValue << shift_amount;
}

/*****************************************************
 * @fn					- USART_IRQHandling
 *
 * @brief				- USART API that handles all interrupt event
 *
 * @param[in]			- IRQ number of the I2C interrupt peripheral
 * @param[in]			= The priority value of that USART interrupt peripheral
 *
 * @return				- none
 * @note				- Refer to the Cortex M4 Generic User Guide the NVIC register table
 */
void USART_IRQHandling(USART_Handle_t* pUSARTHandler) {

	uint8_t temp1, temp2;
/**********************************TXE_INTERRPUT_HANDLER**********************************/
	temp1 = USART_CheckStatusFlag(&pUSARTHandler->pUSARTx->SR, USART_FLAG_SR_TXE);
	temp2 = pUSARTHandler->pUSARTx->CR1 & (1 << USART_CR1_TXEIE);
	if (temp1 && temp2) {
		TXEInterruptHandler(pUSARTHandler);
	}

/**********************************TC_INTERRUPT_HANDLER********************************/
	temp1 = USART_CheckStatusFlag(&pUSARTHandler->pUSARTx->SR, USART_FLAG_SR_TC);
	temp2 = pUSARTHandler->pUSARTx->CR1 & (1 << USART_CR1_TCIE);
	if (temp1 && temp2) {

		//When TCE is set, this signals the end of USART/UART transmission
		//Note: TCE is set by hardware and cleared by software by software
		//		sequence,


	}

/**********************************CTS_INTERRUPT_HANDLER*******************************/
	temp1 = USART_CheckStatusFlag(&pUSARTHandler->pUSARTx->SR, USART_FLAG_SR_CTS);
	temp2 = pUSARTHandler->pUSARTx->CR3 & (1 << USART_CR3_CTSIE);
	if (temp1 && temp2) {


	}

/*********************************RXNE_INTERRUPT_HANDLER*********************************/
	temp1 = USART_CheckStatusFlag(&pUSARTHandler->pUSARTx->SR, USART_FLAG_SR_RXNE);
	temp2 = pUSARTHandler->pUSARTx->CR1 & (1 << USART_CR1_RXNEIE);
	if (temp1 && temp2) {
		RXNEInterruptHandler(pUSARTHandler);

	}

/**********************************PE_INTERRUPT_HANDLER***********************************/
	temp1 = USART_CheckStatusFlag(&pUSARTHandler->pUSARTx->SR, USART_FLAG_SR_PE);
	temp2 = pUSARTHandler->pUSARTx->CR1 & (1 << USART_CR1_PEIE);
	if (temp1 && temp2) {

	}
}

/*****************************************************
 * @fn					- USART_CheckStatusFlag
 *
 * @brief				- Check the status of the given flag in register
 *
 * @param[in]			- address of status register of specific USART peripherals
 * @param[in]			- the status flag
 *
 * @return				- none
 * @note				- none
 */
uint8_t USART_CheckStatusFlag(__vo uint32_t* statusReg, uint16_t flag) {
	if ((*statusReg) & flag) {
		return FLAG_SET;
	}
	return FLAG_RESET;
}

/*****************************************************
 * @fn					- getPLLClkFreq();
 *
 * @brief				- This helper function returns the current I2C clock frequency PLL
 * 						  bus is supplying to this peripheral
 *
 * @param[in]			-
 *
 * @return				- none
 * @note				- See the Clock Tree diagram in STM32F4xx Reference Manual for details
 */
static uint32_t getPLLClkFreq() {

	//Implemented later
	return 1;
}

/*****************************************************
 * @fn					- getAPBxClkFreq();
 *
 * @brief				- This helper function returns the current USART clock frequency APB1
 * 						  bus is supplying to this peripheral
 *
 * @param[in]			-
 *
 * @return				- none
 * @note				- See the Clock Tree diagram in STM32F4xx Reference Manual for details
 */
static uint32_t getAPBxClkFreq(USART_Reg_t* pUSARTx) {

	uint16_t temp, AHB1Prescalar,  APBxPrescalar;
	uint16_t AHB1PreSclr[9] = {1, 2, 4, 6, 16, 64, 128, 256, 512};
	uint16_t APBxPreSclr[5] = {1, 2, 4, 8, 16};
	uint32_t sysClk;

	//Determine what system clock the MCU is running
	switch ((RCC->CFGR >> 2U) & 0x3) {
	case RCC_HSE:	sysClk = HSE_CLK_FREQ; break; //8MHz
	case RCC_HSI:	sysClk = HSI_CLK_FREQ; break; //16MHz
	case RCC_PLL:	sysClk = getPLLClkFreq(); break; //See this function implementation for details
	}

	//Determine the prescalar factor AHB1 is using
	temp = (RCC->CFGR >> 4U) & 0xF;
	AHB1Prescalar = (temp < 8U) ? AHB1PreSclr[0] : AHB1PreSclr[temp % 8 + 1];

	//Determine the prescalar factor APB1 is using
	if (pUSARTx == USART2 || pUSARTx == USART3 || pUSARTx == UART4 || pUSARTx == UART5) {
		//USART2, USART3, UART4, UART5 hang on APB1
		temp = (RCC->CFGR >> 10U) & 0x7;
	} else {
		//USART1 and USART6 hang on APB2
		temp = (RCC->CFGR >> 13U) & 0x7;
	}
	APBxPrescalar = (temp < 4U) ? APBxPreSclr[0] : APBxPreSclr[temp % 8 + 1];

	//Return the APB1 clock freq
	return (sysClk / AHB1Prescalar) / APBxPrescalar;
}

/*****************************************************
 * @fn					- getUSARTDiv
 *
 * @brief				- helper function that calculates the USART baud rate
 *
 * @param[in]			- handle structure of specific UART peripheral
 *
 * @return				- return the USART baud rate
 * @note				- none
 */
static uint32_t getUSARTDiv(USART_Handle_t* pUSARTHandler) {

	uint8_t factor = (pUSARTHandler->USART_Config.Oversampling == USART_OVERSAMPLING_BY_8) ? 1U : 0U;
	uint32_t APBxClkFreq = getAPBxClkFreq(pUSARTHandler->pUSARTx);

	return (APBxClkFreq * 25U) / (2 * (2 - factor) * pUSARTHandler->USART_Config.BaudRate);
}

/*****************************************************
 * @fn					- TXEInterruptHandler
 *
 * @brief				- helper function that handles the TXE interrupt
 *
 * @param[in]			- handle structure of specific UART peripheral
 *
 * @return				- none
 * @note				- none
 */
static void TXEInterruptHandler(USART_Handle_t* pUSARTHandler) {
	//Check if the word length is 8 or 9-bit data frame
	if (pUSARTHandler->USART_Config.WordLength == USART_WORDLEN_9BITS) {

		//2 bytes of data will be sent, 9-bit masking is necessary
		pUSARTHandler->pUSARTx->DR = *((uint16_t*) pUSARTHandler->pTxBuffer) & ((uint16_t) 0x1FF);

		//When parity bit is disabled, the data sent is 2 bytes
		//For more, see page 991 in MCU Reference Manual
		if (pUSARTHandler->USART_Config.ParityControl == USART_PARITY_DI) {
			(uint16_t*) pUSARTHandler->pTxBuffer++;
		} else {
			//Parity bit is used in this transfer. Thus, 8 bits of user data
			//will be sent. The 9th bit will be replaced with parity bit
			//by hardware
			pUSARTHandler->pTxBuffer++;
		}
	} else {
		//Write buffer into the data register at 8-bit data frame
		pUSARTHandler->pUSARTx->DR = *(pUSARTHandler->pTxBuffer) & ((uint8_t) 0xFF);
		pUSARTHandler->pTxBuffer++;
	}
	pUSARTHandler->TxLen--;
}

/*****************************************************
 * @fn					- RXNEInterruptHandler
 *
 * @brief				- helper function that handles the RXNE interrupt
 *
 * @param[in]			- handle structure of specific UART peripheral
 *
 * @return				- none
 * @note				- none
 */
static void RXNEInterruptHandler(USART_Handle_t* pUSARTHandler) {
	//Check if the word length is 8 or 9-bit data frame
	if (pUSARTHandler->USART_Config.WordLength == USART_WORDLEN_9BITS) {

		//When parity bit is disabled, 9-bit of data will be received and no parity bit
		if (pUSARTHandler->USART_Config.ParityControl == USART_PARITY_DI) {
			//2 bytes of data will be sent, 9-bit masking is necessary
			*((uint16_t*) pUSARTHandler->pRxBuffer) = pUSARTHandler->pUSARTx->DR & ((uint16_t) 0x1FF);
			(uint16_t*) pUSARTHandler->pRxBuffer++;
		} else {
			//Parity bit is used in this transfer. Thus, 8 bits of user data
			//will be received. The 9th bit will be replaced with parity bit
			//by hardware
			*(pUSARTHandler->pRxBuffer) = pUSARTHandler->pUSARTx->DR & ((uint16_t) 0xFF);
			pUSARTHandler->pRxBuffer++;
		}
	} else {
		//In 8-bit data frame, if parity bit is disabled, 8 bits will
		//be received
		if (pUSARTHandler->USART_Config.ParityControl == USART_PARITY_DI) {
			//Write buffer into the data register at 8-bit data frame
			*(pUSARTHandler->pRxBuffer) = pUSARTHandler->pUSARTx->DR & ((uint8_t) 0xFF);
		} else {
			//if parity bit is enabled, only 7 bit will be received
			*(pUSARTHandler->pRxBuffer) = pUSARTHandler->pUSARTx->DR & ((uint8_t) 0x7F);
		}
		pUSARTHandler->pRxBuffer++;
	}
	pUSARTHandler->RxLen--;
}

//This API is up to the user application to implement
__weak void USART_ApplicationEventCallback(USART_Handle_t* pUSARTHandler, uint8_t appEvnt);
