/*
 * STM32F407xx_I2C_Driver.c
 *
 *  Created on: Sep 3, 2020
 *      Author: Donavan Tran
 */

#include "../Inc/STM32F407xx_I2C_Driver.h"

/*
 * Helper function not accessible by the user application
 */
static uint32_t getAPB1ClkFreq();
static uint32_t getPLLClkFreq();
static void generateStartCondition(I2C_Reg_t* pI2Cx);
static void generateStopCondition(I2C_Reg_t* pI2Cx);
static void clearFlagSB(I2C_Reg_t* pI2Cx);
static void clearFlagADDR(I2C_Reg_t* pI2Cx);

/*****************************************************
 * @fn					- I2C_PeriClkCtrl
 *
 * @brief				- Enable or disable the SPIx clock peripherals
 *
 * @param[in]			- I2C_Reg_t* pI2Cx: Base address of the specific I2C
 * @param[in]			- ENABLE or DISABLE macro
 *
 * @return				- none
 * @note				- none
 */
void I2C_PeriClkCtrl(I2C_Reg_t* pI2Cx, uint8_t EnOrDi) {
	if (EnOrDi) {
		I2Cx_PCLK_EN(pI2Cx); //Go to I2Cx_PCLK_EN macro for more details
	} else {
		I2Cx_PCLK_DI(pI2Cx); //Go to I2Cx_PCLK_DI macro for more details
	}
}

static uint32_t getPLLClkFreq() {

	//Implemented later
	return 0;
}

/*****************************************************
 * @fn					- getAPB1ClkFreq();
 *
 * @brief				- This helper function returns the current I2C clock frequency APB1
 * 						  bus is supplying to this peripheral
 *
 * @param[in]			-
 *
 * @return				- none
 * @note				- See the Clock Tree diagram in STM32F4xx Reference Manual for details
 */
static uint32_t getAPB1ClkFreq() {

	uint16_t temp, AHB1Prescalar,  APB1Prescalar;
	uint16_t AHB1PreSclr[9] = {1, 2, 4, 6, 16, 64, 128, 256, 512};
	uint16_t APB1PreSclr[5] = {1, 2, 4, 8, 16};
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
	temp = (RCC->CFGR >> 10U) & 0x7;
	APB1Prescalar = (temp < 4U) ? APB1PreSclr[0] : APB1PreSclr[temp % 8 + 1];

	//Return the APB1 clock freq
	return (sysClk / AHB1Prescalar) / APB1Prescalar;
}

/*****************************************************
 * @fn					- I2C_Init
 *
 * @brief				- Initialize the I2C port given the handle structure
 *
 * @param[in]			- Handle Structure of I2C that contains all I2C configuration
 * 						  and port
 *
 * @return				- none
 * @note				- none
 */
void I2C_Init(I2C_Handle_t* pI2CHandler) {

	uint16_t ccr_value;
	uint32_t APB1ClkFreq;
	uint64_t temp;
	temp = 0;

	//Enable the peripheral clock
	I2C_PeriClkCtrl(pI2CHandler->pI2Cx, ENABLE);

	//Note: you may have the option for clock stretching, which
	//enables the slave to pull the clock Low to slow down
	//the communication. The clock stretching feature is one of the
	//important features in I2C, used by both master and slave.
	//Clock stretching is enabled by default in slave mode. To disable it,
	//configure the I2C_CR1 register bit 7.

	//I2C1->CR1 |= (1 << I2C_CR1_ACK);

	//Select the peripheral clock frequency
	//The other bits are ignored and set to 0 by default
	APB1ClkFreq = getAPB1ClkFreq();
	pI2CHandler->pI2Cx->CR2 |= (APB1ClkFreq / 1000000U) & 0x3F;
	//I2C1->CR2 |= (APB1ClkFreq / 1000000U) & 0x3F;
	//You may have option to configure the addressing mode in the I2C_OAR1
	//register. However, we don't implement that as part of the configuration
	//option in I2C. If you so wish to do it, implement that yourself!!!
	pI2CHandler->pI2Cx->OAR1 |= pI2CHandler->I2C_Config.DeviceAddress & 0xFF;

	//SPECIAL NOTE: Bit 14 of I2C_OAR1 register should be on kept at 1
	//by the software. Reason: I don't know, figure it out if you can.
	pI2CHandler->pI2Cx->OAR1 |= (1 << 14U);

	//Configure the SCL clock frequencies depending on the mode
	//standard mode/Fast mode and duty cycle bit
	temp |= pI2CHandler->I2C_Config.SCLSpeed << I2C_CCR_F_S;
	temp |= pI2CHandler->I2C_Config.FMDutyCycle << I2C_CCR_DUTY;

	//Given: T(High) + T(Low) = T(I2C_SCL_SPEED)
	//T_I2C_SCL_SPEED = (float) 1 / I2C_SCL_SPEED(pI2CHandler->I2C_Config.SCLSpeed);
	//T_APB1ClkFreq	= (float) 1 / APB1ClkFreq;
	if (pI2CHandler->pI2Cx->CCR & (1 << I2C_CCR_F_S)) { //Fast mode
		if (pI2CHandler->pI2Cx->CCR & (1 << I2C_CCR_DUTY)) { //To reach 400khz

			/*	Calculation: T(High) = 9 * CCR * T(PCLK1)
			 *		   		 T(Low) = 16 * CCR * T(PCLK1)
			 *			     T(High) + T(Low) = 25 * CCR * T(PCLK1)
			 *			     CCR = (T(High) + T(Low)) / (25 * T(PCLK1))
			 */
			ccr_value = APB1ClkFreq / (25 * (I2C_SCL_SPEED(pI2CHandler->I2C_Config.SCLSpeed)));
		} else { //To reach 100kHz

			/*	Calculation: T(High) = 1 * CCR * T(PCLK1)
			 *		   		 T(Low) = 2 * CCR * T(PCLK1)
			 *			     T(High) + T(Low) = 3 * CCR * T(PCLK1)
			 *			     CCR = (T(High) + T(Low)) / (3 * T(PCLK1))
			 */
			ccr_value = APB1ClkFreq / (3 * (I2C_SCL_SPEED(pI2CHandler->I2C_Config.SCLSpeed)));
		}
	} else { //Standard Mode

		/*	Calculation: T(High) = CCR * T(PCLK1)
		 *		   		 T(Low) =   CCR * T(PCLK1)
		 *			     T(High) + T(Low) = 2 * CCR * T(PCLK1)
		 *			     CCR = (T(High) + T(Low)) / (2 * T(PCLK1))
		 */
		ccr_value = APB1ClkFreq / (2 * (I2C_SCL_SPEED(pI2CHandler->I2C_Config.SCLSpeed)));
	}
	temp |= (ccr_value & 0xFFF);
	pI2CHandler->pI2Cx->CCR = temp;

	//Clear the TRISE reg
	pI2CHandler->pI2Cx->TRISE &= ~((int) 1);

	//Configure the Rise Time (TRISE)
	temp = APB1ClkFreq * (I2C_T_RISE(pI2CHandler->I2C_Config.SCLSpeed)) / (10000000U);
	pI2CHandler->pI2Cx->TRISE = (temp + 1) & 0x3F;

}

/*****************************************************
 * @fn					- I2C_DeInit
 *
 * @brief				- Reset the entire register configuration of the specific I2C port
 *
 * @param[in]			- Base address of the specific SPI peripherals (SPI_Reg_t* pI2Cx)
 *
 * @return				- none
 * @note				- The implementation so far only covers only 3 I2C ports
 */
void I2C_DeInit(I2C_Reg_t* pI2Cx) {
	if (pI2Cx == I2C1) {
		I2C1_PCLK_RST();
	} else if (pI2Cx == I2C2) {
		I2C2_PCLK_RST();
	} else {
		I2C3_PCLK_RST();
	}
}

/*****************************************************
 * @fn					- I2C_PeripheralEnable
 *
 * @brief				- Enable that specific I2C peripheral
 *
 * @param[in]			- Base address of the specific SPI peripherals (I2C_Reg_t* pI2Cx)
 * @param[in]			= ENABLE or DISABLE macro
 *
 * @return				- none
 * @note				- none
 */
void I2C_PeripheralEnable(I2C_Reg_t* pI2Cx, uint8_t EnOrDi) {
	if (EnOrDi) {
		pI2Cx->CR1 |= (1 << I2C_CR1_PE);

		//Enable the Acknowledge bit
		pI2Cx->CR1 |=  1 << I2C_CR1_ACK;
	} else {
		pI2Cx->CR1 &= ~(1 << I2C_CR1_PE); //clearing PE also clears ACK
	}
}

/*****************************************************
 * @fn					- I2C_MasterSendData
 *
 * @brief				- Sending the data to slave as master
 *
 * @param[in]			- Base address of the specific I2C peripherals (I2C_Reg_t* pI2Cx)
 * @param[in]			- buffer for transmission (TxBuffer)
 * @param[in]			- length of the buffer (len)
 * @param[in]			- slave address
 *
 * @return				- none
 * @note				- See the Transfer Sequence diagram for master transmitter on page 849
 * 						  in MCU Reference Manual for more details
 */
void I2C_MasterSendData(I2C_Handle_t* pI2CHandler, uint8_t* pTxBuffer,
		                uint32_t len, uint8_t pSlaveAddress) {

    // Activate the Start condition
    // Note: Setting the START bit causes the interface to generate
    //	  	a Start condition and switch to Master Mode (MSL bit set) when
    //	  	the BUSY bit is cleared.
    //	  	This also set the SB bit by hardware (see I2C_SR1 register for details)
    //      You may also need to enable the I2C_CR1 PE register
	generateStartCondition(pI2CHandler->pI2Cx);

	// Poll until the SB bit in SR1 register is set
	// This is important if any of the bit is set by HARDWARE
	while (!I2C_CheckStatusFlag(&pI2CHandler->pI2Cx->SR1, I2C_FLAG_SR1_SB));

	// Clear the SB bit by reading SR1 register followed by
	// writing DR register with Address. If SB bit not clear,
	// SCL will be pulled low and the transmission is delay (which
	// we don't want, obviously)
	clearFlagSB(pI2CHandler->pI2Cx);
	pI2CHandler->pI2Cx->DR = (pSlaveAddress << 1); //Write the slave address to DR register

	//Polling until the ADDR bit is set
	while (!I2C_CheckStatusFlag(&pI2CHandler->pI2Cx->SR1, I2C_FLAG_SR1_ADDR));

	//As soon as the slave address is sent, the ADDR bit is set by HARDWARE
	//and an interrupt is generated if the ITEVFEN bit is set (which we don't cover in
	//this case). Clear this by reading SR1 register followed by reading SR2
	clearFlagADDR(pI2CHandler->pI2Cx);

	//Sending bytes of data to slave
	//Note: writing TxBuffer to DR register clears the BTF bit
	while (len) {

		//Polling until the Transmit register buffer is empty (TXE = 1)
		//Then write first data into DR
		while (!I2C_CheckStatusFlag(&pI2CHandler->pI2Cx->SR1, I2C_FLAG_SR1_TXE));

		//Write TxBuffer into DR
		pI2CHandler->pI2Cx->DR = *pTxBuffer;
		len--;
		pTxBuffer++;
	}

	//Wait for both TXE and BTF is set before closing the communication
	while (!(I2C_CheckStatusFlag(&pI2CHandler->pI2Cx->SR1, I2C_FLAG_SR1_BTF) &&
			 I2C_CheckStatusFlag(&pI2CHandler->pI2Cx->SR1, I2C_FLAG_SR1_TXE)));

	//Generate the Stop condition to terminate the communication_SR1_BTF
	generateStopCondition(pI2CHandler->pI2Cx);

	//Memo: Cover the 10-bit addressing mode scenario later

}

/*****************************************************
 * @fn					- I2C_CheckStatusFlag
 *
 * @brief				- Check the status of the given flag in either SR1 or SR2 register
 *
 * @param[in]			- address of status register of specific I2C peripherals
 * @param[in]			- the status flag
 *
 * @return				- none
 * @note				- none
 */
uint8_t I2C_CheckStatusFlag(__vo uint32_t* statusReg, uint8_t flag) {
	if ((*statusReg) & flag) {
		return FLAG_SET;
	}
	return FLAG_RESET;
}

/*****************************************************
 * @fn					- generateStartCondition()
 *
 * @brief				- helper function that generates the start condition for master
 *
 * @param[in]			- Base address of the specific I2C peripherals (I2C_Reg_t* pI2Cx)
 *
 * @return				- none
 * @note				- none
 */
static void generateStartCondition(I2C_Reg_t* pI2Cx) {
	pI2Cx->CR1 |= 1 << I2C_CR1_START;
}

/*****************************************************
 * @fn					- generateStopCondition()
 *
 * @brief				- helper function that generates the stop condition for master
 *
 * @param[in]			- Base address of the specific I2C peripherals (I2C_Reg_t* pI2Cx)
 *
 * @return				- none
 * @note				- none
 */
static void generateStopCondition(I2C_Reg_t* pI2Cx) {
	pI2Cx->CR1 |= 1 << I2C_CR1_STOP;
}

/*****************************************************
 * @fn					- clearFlagSB()
 *
 * @brief				- helper function that clear the SB status flag
 *
 * @param[in]			- Base address of the specific I2C peripherals (I2C_Reg_t* pI2Cx)
 *
 * @return				- none
 * @note				- none
 */
static void clearFlagSB(I2C_Reg_t* pI2Cx) {
	uint32_t temp;
	temp = pI2Cx->SR1;
	(void) temp; //resolve unused variable warning
}

/*****************************************************
 * @fn					- clearFlagADDR()
 *
 * @brief				- helper function that clear the ADDR status flag
 *
 * @param[in]			- Base address of the specific I2C peripherals (I2C_Reg_t* pI2Cx)
 *
 * @return				- none
 * @note				- none
 */
static void clearFlagADDR(I2C_Reg_t* pI2Cx) {
	uint32_t temp;
	temp = pI2Cx->SR1;
	temp = pI2Cx->SR2;
	(void) temp; //resolve unused variable warning
}

/*****************************************************
 * @fn					- I2C_IRQITConfig
 *
 * @brief				- I2C Interrupt configuration
 *
 * @param[in]			- IRQ Number of the I2C interrupt peripheral
 * @param[in]			= ENABLE or DISABLE macro
 *
 * @return				- none
 * @note				- Refer to the Cortex M4 Generic User Guide the NVIC register table
 */
void I2C_IRQITConfig(uint8_t IRQNumber, uint8_t EnOrDi) {
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
 * @fn					- I2C_IRQPriorityConfig
 *
 * @brief				- I2C Interrupt Priority Configuration
 *
 * @param[in]			- IRQ number of the I2C interrupt peripheral
 * @param[in]			= The priority value of that I2C interrupt peripheral
 *
 * @return				- none
 * @note				- Refer to the Cortex M4 Generic User Guide the NVIC register table
 */
void I2C_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriorityValue) {
	uint32_t indx = IRQNumber >> 2U; //Note: There are IRQ Priority field in each IPR register
	uint32_t remainder = IRQNumber & (~(~(int)0 << 2U));
	uint32_t shift_amount = (remainder * 8U) + IMPLEMENTED_IRQ_PRIORITY_BIT;

	//Configure the IRQ_PR register
	NVIC_IPR(indx) |= IRQPriorityValue << shift_amount;
}
