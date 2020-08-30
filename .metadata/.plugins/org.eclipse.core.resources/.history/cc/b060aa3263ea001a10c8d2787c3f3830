/*
 * STM32F407xx_SPI_Driver.c
 *
 *  Created on: Aug 28, 2020
 *      Author: Donavan Tran
 *      Description: This source file contains specific definitions of SPI
 *      			 function API
 */

#include "../Inc/STM32F407xx_SPI_Driver.h"

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
	switch (SPIConf.DeviceMode) {
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
	temp |= SPIConf.CPOL << SPI_CR1_CPOL;
	temp |= SPIConf.CPHA << SPI_CR1_CPHA;

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
void SPI_PeripheralControl(SPI_Reg_t* pSPIx, uint8_t EnOrDi) {
	if (EnOrDi) {
		pSPIx->CR1 |= (1 << SPI_CR1_SPE);
		//SPIx_EN(pSPIx);
	} else {
		pSPIx->CR1 &= ~(1 << SPI_CR1_SPE);
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
uint8_t SPI_CheckStatusFlag(SPI_Reg_t* pSPIx, uint8_t flag) {

	if (pSPIx->SR & flag) {
		return FLAG_SET;
	}
	return FLAG_RESET;
}

/*****************************************************
 * @fn					- GPIO_PeriClkCtrl
 *
 * @brief				- Enable or disable the GPIOx clock peripherals
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
 * @fn					- GPIO_PeriClkCtrl
 *
 * @brief				- Enable or disable the GPIOx clock peripherals
 *
 * @param[in]			- GPIO_Reg_t* pGPIOx: Base address of the GPIO port
 * @param[in]			- ENABLE or DISABLE macro
 *
 * @return				- none
 * @note				- none
 */
void SPI_ReceiveData(SPI_Reg_t* pSPIx, uint8_t* pRxBuffer, uint32_t len);


/*****************************************************
 * @fn					- GPIO_PeriClkCtrl
 *
 * @brief				- Enable or disable the GPIOx clock peripherals
 *
 * @param[in]			- GPIO_Reg_t* pGPIOx: Base address of the GPIO port
 * @param[in]			- ENABLE or DISABLE macro
 *
 * @return				- none
 * @note				- none
 */
void SPI_IRQITConfig(uint8_t IRQNumber, uint8_t EnOrDi);

/*****************************************************
 * @fn					- GPIO_PeriClkCtrl
 *
 * @brief				- Enable or disable the GPIOx clock peripherals
 *
 * @param[in]			- GPIO_Reg_t* pGPIOx: Base address of the GPIO port
 * @param[in]			- ENABLE or DISABLE macro
 *
 * @return				- none
 * @note				- none
 */
void SPI_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriorityValue);

/*****************************************************
 * @fn					- GPIO_PeriClkCtrl
 *
 * @brief				- Enable or disable the GPIOx clock peripherals
 *
 * @param[in]			- GPIO_Reg_t* pGPIOx: Base address of the GPIO port
 * @param[in]			- ENABLE or DISABLE macro
 *
 * @return				- none
 * @note				- none
 */
void SPI_IRQHandling(SPI_Handle_t* pSPIHandler);

