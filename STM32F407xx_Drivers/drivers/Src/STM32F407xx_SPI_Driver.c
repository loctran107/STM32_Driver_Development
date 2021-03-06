/*
 * STM32F407xx_SPI_Driver.c
 *
 *  Created on: Aug 28, 2020
 *      Author: Donavan Tran
 *      Description: This source file contains specific definitions of SPI
 *      			 function API
 */

#include "../Inc/STM32F407xx_SPI_Driver.h"

/*
 * Helper functions declarations
 */
static void SPI_TXE_IT_Handle(SPI_Handle_t* pSPIHandler);
static void SPI_RXNE_IT_Handle(SPI_Handle_t* pSPIHandler);
static void SPI_OVR_IT_Handle(SPI_Handle_t* pSPIHandler);
static uint8_t SPI_CheckStatusFlag(SPI_Reg_t* pSPIx, uint16_t flag);
/*****************************************************
 * @fn					- SPI_PeriClkCtrl
 *
 * @brief				- Enable or disable the SPIx clock peripherals
 *
 * @param[in]			- SPI_Reg_t* pSPIx: Base address of the specific SPI
 * @param[in]			- ENABLE or DISABLE macro
 *
 * @return				- none
 * @note				- none
 */
void SPI_PeriClkCtrl(SPI_Reg_t* pSPIx, uint8_t EnOrDi) {
	if (EnOrDi) {
		SPIx_PCLK_EN(pSPIx); //Go to SPIx_PCLK_EN macro for more details
	} else {
		SPIx_PCLK_DI(pSPIx); //Go to SPIx_PCLK_DI macro for more details
	}
}

/*****************************************************
 * @fn					- SPI_Init
 *
 * @brief				- Initialize the SPI port given the handle structure
 *
 * @param[in]			- Handle Structure of SPI that contains all SPI configuration and port
 *
 * @return				- none
 * @note				- none
 */
void SPI_Init(SPI_Handle_t* pSPIHandler) {
	uint32_t temp;
	SPI_Config_t SPIConf;

	temp     = 0;
	SPIConf  = pSPIHandler->SPI_Config;

	//Enable the SPI Clock Peripheral
	SPI_PeriClkCtrl(pSPIHandler->pSPIx, ENABLE);

	//Set the SPI interface as Master or Slave
	temp |= SPIConf.DeviceMode << SPI_CR1_MSTR;

	//Configure Clock baud rate (only required in Master Mode)
	if (SPIConf.DeviceMode == SPI_DEVICE_MASTER_MODE) {
		temp |= SPIConf.SclkSpeed << SPI_CR1_BR;
	}

	//Configure the Bus Config.
	//Note: if BIDIMODE = 0, the SPI is in full-duplex
	//		if BIDIMODE = 1, the SPI is in half-duplex
	//To select simplex receive only configuration, RDONLY = 1 & BIDIMODE = 0
	switch (SPIConf.BusConfig) {
	case SPI_BUS_CONFIG_FULL_DUPLX: 		temp &= ~(1 << SPI_CR1_BIDIMODE); //Clear the BIDIMODE bit
											break;
	case SPI_BUS_CONFIG_HALF_DUPLX: 		temp |= 1 << SPI_CR1_BIDIMODE; //Set the BIDIMODE bit
											break;
	case SPI_BUS_CONFIG_SIMPLEX_RX_ONLLY:	temp &= ~(1 << SPI_CR1_BIDIMODE); //Clear the BIDIMODE bit
											temp |= 1 << SPI_CR1_RXONLY; //Set the RXONLY bit
											break;
	}

	//Set the DFF bit to define 8- or 16-bit data frame format
	temp |= SPIConf.DFF << SPI_CR1_DFF;

	//Select the CPOL and CPHA bits to define one of the 4 relationships
	//between the data transfer and the serial clock
	temp |= SPIConf.CPOLConfig << SPI_CR1_CPOL;
	temp |= SPIConf.CPHAConfig << SPI_CR1_CPHA;

	//Set the SSM pin to select either Hardware Slave Management
	//Software Slave Management (potentially configure the SSI pin)
	temp |= SPIConf.SSM << SPI_CR1_SSM;

	//Unless the SSI bit is set when the master is in master mode
	//and master mode is enables software slave management, a master
	//mode fault (MODF) bit will be set in SPI_SR register.
	//By setting SSI = 1 when SSM = 1, this sets the NSS set internally high,
	//avoiding MODF error. Remember, SSI influences the NSS state when SSM = 1
	//By default, SSI = 0, which is not an acceptable configuration when working
	//in an non-multi\-master application
	if (SPIConf.SSM) {
		temp |= 1 << SPI_CR1_SSI; //Set the SSI bit
	} else {
		//Making SSOE 1 does NSS output enable.
		//The NSS pin is automatically managed by the hardware.
		//i.e. when SPE = 1, NSS will be automatically pulled to low
		//and NSS pin will be high when SPE = 0
		pSPIHandler->pSPIx->CR2 |= 1 << SPI_CR2_SSOE;
	}

	//Configure the SPI_CR1 Register based on the above configuration
	pSPIHandler->pSPIx->CR1 = temp;

}

/*****************************************************
 * @fn					- SPI_Enable
 *
 * @brief				- Enable the specific SPI peripherals
 *
 * @param[in]			- Base address of the specific SPI peripherals (SPI_Reg_t* pSPIx)
 *
 * @return				- none
 * @note				- The implementation so far only covers only 3 SPI ports
 */
void SPI_PeripheralEnable(SPI_Reg_t* pSPIx, uint8_t EnOrDi) {
	if (EnOrDi) {
		SPIx_EN(pSPIx);
	} else {
		SPIx_DI(pSPIx);
	}
}

/*****************************************************
 * @fn					- SPI_DeInit
 *
 * @brief				- Reset the entire register configuration of the specific SPI port
 *
 * @param[in]			- Base address of the specific SPI peripherals (SPI_Reg_t* pSPIx)
 *
 * @return				- none
 * @note				- The implementation so far only covers only 3 SPI ports
 */
void SPI_DeInit(SPI_Reg_t* pSPIx) {
	if (pSPIx == SPI1) {
		SPI1_PCLK_RST();
	} else if (pSPIx == SPI2) {
		SPI2_PCLK_RST();
	} else {
		SPI3_PCLK_RST();
	}
}

/*
 * Private helper functions
 */
static uint8_t SPI_CheckStatusFlag(SPI_Reg_t* pSPIx, uint16_t flag) {

	if (pSPIx->SR & flag) {
		return FLAG_SET;
	}
	return FLAG_RESET;
}

/*****************************************************
 * @fn					- SPI_SendData (blocking approach)
 *
 * @brief				- Send the message through SPI channel
 *
 * @param[in]			- Base address of the SPI peripherals
 * @param[in]			- Buffer pointer to the data
 * @param[in]			- The number of bytes transmitted are indicated by len
 *
 * @return				- none
 * @note				- it is a standard practice to define len as uint32_t
 * 						- This is a blocking API (polling-based API) because the function call will wait
 * 						  until all the bytes are transmitted
 *
 */
void SPI_SendData(SPI_Reg_t* pSPIx, uint8_t* pTxBuffer, uint32_t len) {
	while (len) {

		//Wait until the Tx Buffer is empty to ready to load data
		while (!SPI_CheckStatusFlag(pSPIx, SPI_TXE_FLAG));
		if ((pSPIx->CR1 & (1 << SPI_CR1_DFF)) == (1 << SPI_CR1_DFF)) { //16-bit data frame
			pSPIx->DR |= *((uint16_t*) pTxBuffer);
			len--;
			len--;
			(uint16_t*) pTxBuffer++;
		} else {
			pSPIx->DR |= *(pTxBuffer);
			len--;
			pTxBuffer++;
		}
	}
}

/*****************************************************
 *
 * @fn					- SPI_ReceiveData
 *
 * @brief				- Receive the data read from the Rx buffer
 *
 * @param[in]			- Base address of the SPIx peripherals
 * @param[in]			- pointer to the Rx Buffer
 * @param[in]			- the number of bytes of the buffer
 *
 * @return				- none
 * @note				- This is a blocking API (polling-based implementation)
 */
void SPI_ReceiveData(SPI_Reg_t* pSPIx, uint8_t* pRxBuffer, uint32_t len) {
	while (len) {
		while (!SPI_CheckStatusFlag(pSPIx, SPI_RXNE_FLAG)); //Wait until the RxBuffer is not empty (full)

		//Read the data
		if ((pSPIx->CR1 & (1 << SPI_CR1_DFF)) == (1 << SPI_CR1_DFF)) { //16-bit data frame format
			*((uint16_t *) pRxBuffer) = pSPIx->DR; //load the data from DR to RxBuffer
			len--;
			len--;
			(uint16_t*) pRxBuffer++;
		} else { //8-bit data frame
			*(pRxBuffer) = pSPIx->DR;
			len--;
			pRxBuffer++;
		}
	}
}

/*****************************************************
 * @fn					- SPI_SendDataIT (non-blocking approach)
 *
 * @brief				- Send the message through the SPI channels using
 * 						  interrupt service routine
 *
 * @param[in]			- Base address of the SPI peripherals
 * @param[in]			- Buffer pointer to the data
 * @param[in]			- The number of bytes transmitted are indicated by len
 *
 * @return				- none
 * @note				- it is a standard practice to define len as uint32_t
 * 						- This is a non-blocking API (polling-based API) because the function call will wait
 * 						  until all the bytes are transmitted
 *
 */
uint8_t SPI_SendDataIT(SPI_Handle_t* pSPIHandler, uint8_t* pTxBuffer, uint32_t len) {

	uint8_t SPI_State = pSPIHandler->TxState;
	if (SPI_State != SPI_BUSY_IN_TX) {
		//1. Save the Tx buffer address and len information in lsome global variables
		pSPIHandler->TxLen = len;
		pSPIHandler->pTxBuffer = pTxBuffer;

		//2. Mark the SPI state as busy in the transmission so that
		//   no other code can take over same SPI information until transmission is over
		pSPIHandler->TxState = SPI_BUSY_IN_TX;

		//3. Enable the TXEIE control bit to trigger interrupt whenever the TXE flag is set
		//When the interrupt is trigger, the data transmision
		//will be then handled by the Interrupt Service Routine code
		pSPIHandler->pSPIx->CR2 |= (1 << SPI_CR2_TXEIE);

	}

	return SPI_State;

}

/*****************************************************
 *
 * @fn					- SPI_ReceiveDataIT (non-blocking approach)
 *
 * @brief				- Receive the data read from the Rx buffer
 *
 * @param[in]			- Base address of the SPIx peripherals
 * @param[in]			- pointer to the Rx Buffer
 * @param[in]			- the number of bytes of the buffer
 *
 * @return				- none
 * @note				- This is a non-blocking API (polling-based implementation)
 */
uint8_t SPI_ReceiveDataIT(SPI_Handle_t* pSPIHandler, uint8_t* pRxBuffer, uint32_t len) {

	uint8_t SPI_State = pSPIHandler->RxState;
	if (SPI_State != SPI_BUSY_IN_RX) {
		//1. Save the Tx buffer address and len information in some global variables
		pSPIHandler->RxLen = len;
		pSPIHandler->pRxBuffer = pRxBuffer;

		//2. Mark the SPI state as busy in the transmission so that
		//   no other code can take over same SPI information until transmission is over
		pSPIHandler->RxState = SPI_BUSY_IN_RX;

		//3. Enable the TXEIE control bit to trigger interrupt whenever the TXE flag is set
		//When the interrupt is trigger, the data transmision
		//will be then handled by the Interrupt Service Routine code
		pSPIHandler->pSPIx->CR2 |= (1 << SPI_CR2_RXNEIE);

	}

	return SPI_State;
}

/*****************************************************
 * @fn					- SPI_IRQITConfig
 *
 * @brief				- Enable the interrupt by selecting the IRQ number
 * 						- on the vector table
 *
 * @param[in]			- unsigned integer IRQ number
 * @param[in]			- ENABLE or DISABLE macro
 *
 * @return				- none
 * @note				- Look at Cortex M4 Processor Generic Guide NVIC section for details
 */
void SPI_IRQITConfig(uint8_t IRQNumber, uint8_t EnOrDi) {

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
 * @fn					- SPI_IRQPriorityConfig
 *
 * @brief				- Configuring the priority of the specific IRQ Number
 *
 * @param[in]			- unsigned integer 8 bit interrupt request number
 * @param[in]			- unsigned integer 8 bit interrupt request priority
 *
 * @return				- none
 * @note				- Refer to the Cortex M4 Generic User Guide the NVIC register table
 */
void SPI_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriorityValue) {
	uint32_t indx = IRQNumber >> 2U; //Note: There are IRQ Priority field in each IPR register
	uint32_t remainder = IRQNumber & (~(~(int)0 << 2U));
	uint32_t shift_amount = (remainder * 8U) + IMPLEMENTED_IRQ_PRIORITY_BIT;

	//Configure the IRQ_PR register
	NVIC_IPR(indx) |= IRQPriorityValue << shift_amount;
}

/*****************************************************
 * @fn					- SPI_IRQHandling
 *
 * @brief				- Handle the interrupt request of the SPI peripherals
 *
 * @param[in]			- pointer to the SPI Handle structure
 *
 * @return				- none
 * @note				- For the time being, the only error interupt that got triggered
 * 						  is when overrun occurred. To add more details, take a look at
 * 						  the datasheet.
 * 						  Also, all the handle functions included in this function
 * 						  are helper functions; thus they are not public to the
 * 						  user applications (create private keyword Dennis Ritchie)Denni
 */
void SPI_IRQHandling(SPI_Handle_t* pSPIHandler) {
	SPI_Reg_t* pSPIx = pSPIHandler->pSPIx;

	//Check what event trigger the interrupt through SPI_SR register
	if ((pSPIx->SR & SPI_TXE_FLAG) && (pSPIx->SR & (1 << SPI_CR2_TXEIE))) { //TXE is triggered
		SPI_TXE_IT_Handle(pSPIHandler);
	} else if ((pSPIx->SR & SPI_RXNE_FLAG) && (pSPIx->SR & (1 << SPI_CR2_RXNEIE))) { //RXNE is triggered
		SPI_RXNE_IT_Handle(pSPIHandler);
	} else if ((pSPIx->SR & SPI_OVR_FLAG) && (pSPIx->SR & (1 << SPI_CR2_ERRIE))) { //overrun fault is triggered
		SPI_OVR_IT_Handle(pSPIHandler);
	}
}

/*****************************************************
 * @fn					- SPI_TXE_IT_Handle()
 *
 * @brief				- Handle the TXT Interrupt Event
 *
 * @param[in]			- pointer to the SPI handle structure
 *
 * @return				- none
 * @note				-
 */
static void SPI_TXE_IT_Handle(SPI_Handle_t* pSPIHandler) {
	SPI_Reg_t* pSPIx = pSPIHandler->pSPIx;
	if ((pSPIx->CR1 & (1 << SPI_CR1_DFF)) == (1 << SPI_CR1_DFF)) { //16-bit data frame
		pSPIx->DR |= *((uint16_t*) pSPIHandler->pTxBuffer);
		pSPIHandler->TxLen--;
		pSPIHandler->TxLen--;
		(uint16_t*) pSPIHandler->pTxBuffer++;
	} else {
		pSPIx->DR |= *(pSPIHandler->pTxBuffer);
		pSPIHandler->TxLen--;
		pSPIHandler->pTxBuffer++;
	}

	if (pSPIHandler->TxLen <= 0) {

		//close the application and inform the application that Tx is over

		//This prevents interrupts from setting up TXT flags
		SPI_CloseTransmission(pSPIHandler);
		SPI_ApplicationEvent(pSPIHandler, SPI_EVNT_TX_CMPLT);

	}
}

/*****************************************************
 * @fn					- SPI_RXE_IT_Handle()
 *
 * @brief				- Handle the TXT Interrupt Event
 *
 * @param[in]			- pointer to the SPI handle structure
 *
 * @return				- none
 * @note				-
 */
static void SPI_RXNE_IT_Handle(SPI_Handle_t* pSPIHandler) {
	SPI_Reg_t* pSPIx = pSPIHandler->pSPIx;
	if ((pSPIx->CR1 & (1 << SPI_CR1_DFF)) == (1 << SPI_CR1_DFF)) { //16-bit data frame
		*((uint16_t*) pSPIHandler->pRxBuffer) = pSPIx->DR;
		pSPIHandler->RxLen--;
		pSPIHandler->RxLen--;
		(uint16_t*) pSPIHandler->pRxBuffer++;
	} else {
		 *(pSPIHandler->pRxBuffer) = pSPIx->DR;
		pSPIHandler->RxLen--;
		pSPIHandler->pRxBuffer++;
	}

	if (pSPIHandler->RxLen <= 0) {

		//close the application and inform the application that Tx is over
		SPI_CloseReception(pSPIHandler);
		SPI_ApplicationEvent(pSPIHandler, SPI_EVNT_RX_CMPLT);
		//This prevents interrupts from setting up TXT flags


	}
}

/*****************************************************
 * @fn					- SPI_OVR_IT_Handle()
 *
 * @brief				- Handle the TXT Interrupt Event
 *
 * @param[in]			- pointer to the SPI handle structure
 *
 * @return				- none
 * @note				-
 */
static void SPI_OVR_IT_Handle(SPI_Handle_t* pSPIHandler) {

	uint8_t temp;
	//1. Clear the overrun flag
	if (pSPIHandler->TxState != SPI_BUSY_IN_TX) {
		SPI_ClearOVRFlag(pSPIHandler->pSPIx);
	}

	//2. Inform the application
	SPI_ApplicationEvent(pSPIHandler, SPI_EVNT_OVR_ERR);
	(void) temp;
}

/*****************************************************
 * @fn					- SPI_CloseTransmission()
 *
 * @brief				- Closing the interrupt transmission
 *
 * @param[in]			- pointer to the SPI handle structure
 *
 * @return				- none
 * @note				-
 */
void SPI_CloseTransmission(SPI_Handle_t* pSPIHandler) {
	pSPIHandler->pSPIx->CR2 &= ~(1 << SPI_CR2_TXEIE);
	pSPIHandler->pTxBuffer = NULL;
	pSPIHandler->TxLen = 0;
	pSPIHandler->TxState = SPI_READY;
}

/*****************************************************
 * @fn					- SPI_CloseReception()
 *
 * @brief				- Closing the interrupt reception
 *
 * @param[in]			- pointer to the SPI handle structure
 *
 * @return				- none
 * @note				-
 */
void SPI_CloseReception(SPI_Handle_t* pSPIHandler) {
	pSPIHandler->pSPIx->CR2 &= ~(1 << SPI_CR2_RXNEIE);
	pSPIHandler->pRxBuffer = NULL;
	pSPIHandler->RxLen = 0;
	pSPIHandler->RxState = SPI_READY;
}

/*****************************************************
 * @fn					- SPI_ClearOVRFlag()
 *
 * @brief				- Closing the interrupt overrun error exception
 *
 * @param[in]			- Base address of the SPI peripherals
 *
 * @return				- none
 * @note				- Consult the datasheet for more details about clearing the
 * 						  OVR bit
 */
void SPI_ClearOVRFlag(SPI_Reg_t* pSPIx) {
	uint8_t temp;
	temp = pSPIx->DR;
	temp = pSPIx->SR;
	(void) temp;
}

/*****************************************************
 * @fn					- SPI_ApplicationEvent
 *
 * @brief				- Inform the user the completed event
 *
 * @param[in]			- pointer to the SPI handle structure
 * @param[in]			- event status
 *
 * @return				- none
 * @note				- Informing the user
 */
__weak void SPI_ApplicationEvent(SPI_Handle_t* pSPIHandler, uint8_t appEvent) {
	//This is weak implementation. The application may override this function
}
