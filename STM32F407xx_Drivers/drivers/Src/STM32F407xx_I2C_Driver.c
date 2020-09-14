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
	case RCC_HSE:	sysClk = HSE_CLK_FREQ;	//8MHz
	case RCC_HSI:	sysClk = HSI_CLK_FREQ;	//16MHz
	case RCC_PLL:	sysClk = getPLLClkFreq();	//See this function implementation for details
	}

	//Determine the prescalar factor AHB1 is using
	temp = (RCC->CFGR >> 4U) & 0xF;
	AHB1Prescalar = (temp < 8U) ? AHB1PreSclr[0] : AHB1PreSclr[temp % 8 + 1];

	//Determine the prescalar factor APB1 is using
	temp = (RCC->CFGR >> 10U) & 0x7;
	APB1Prescalar = (temp < 4U) ? APB1PreSclr[0] : APB1PreSclr[temp % 8 + 1];

	//Return the APB1 clock freq
	return (sysClk / AHB1Prescalar / APB1Prescalar);
}

/*****************************************************
 * @fn					- I2C_Init
 *
 * @brief				- Initialize the I2C port given the handle structure
 *
 * @param[in]			- Handle Structure of I2C that contains all I2C configuration and port
 *
 * @return				- none
 * @note				- none
 */
void I2C_Init(I2C_Handle_t* pI2CHandler) {

	uint16_t APB1ClkFreq;
	float T_I2C_SCL_SPEED, T_APB1ClkFreq;

	//Enable the peripheral clock
	I2C_PeriClkCtrl(pI2CHandler->pI2Cx, ENABLE);

	//Note: you may have the option for clock stretching, which
	//enables the slave to pull the clock Low to slow down
	//the communication. The clock stretching feature is one of the
	//important features in I2C, used by both master and slave.
	//Clock stretching is enabled by default in slave mode. To disable it,
	//configure the I2C_CR1 register bit 7.

	//Enable the Acknowledge bit
	pI2CHandler->pI2Cx->CR1 |= (1 << I2C_CR1_ACK);

	//Select the peripheral clock frequency
	//The other bits are ignored and set to 0 by default
	APB1ClkFreq = getAPB1ClkFreq();
	pI2CHandler->pI2Cx->CR2 |= (APB1ClkFreq / 1000000U) & 0x3F;

	//You may have option to configure the addressing mode in the I2C_OAR1
	//register. However, we don't implement that as part of the configuration
	//option in I2C. If you so wish to do it, implement that yourself!!!
	pI2CHandler->pI2Cx->OAR1 |= pI2CHandler->I2C_Config.DeviceAddress & 0xFF;

	//SPECIAL NOTE: Bit 14 of I2C_OAR1 register should be on kept at 1
	//by the software. Reason: I don't know, figure it out if you can.
	pI2CHandler->pI2Cx->OAR1 |= (1 << 14U);

	//Configure the SCL clock frequencies depending on the mode
	//standard mode/Fast mode and duty cycle bit
	pI2CHandler->pI2Cx->CCR |= pI2CHandler->I2C_Config.SCLSpeed << I2C_CCR_F_S;
	pI2CHandler->pI2Cx->CCR |= pI2CHandler->I2C_Config.FMDutyCycle << I2C_CCR_DUTY;

	//Given: T(High) + T(Low) = T(I2C_SCL_SPEED)
	T_I2C_SCL_SPEED = (float) 1 / I2C_SCL_SPEED;
	T_APB1ClkFreq	= (float) 1 / APB1ClkFreq;
	if (pI2CHandler->pI2Cx->CCR & (1 << I2C_CCR_F_S)) { //Fast mode
		if (pI2CHandler->pI2Cx->CCR & (1 << I2C_CCR_DUTY)) { //To reach 400khz

			//Calculation: T(High) = 9 * CCR * T(PCLK1)
			//			   T(Low) = 16 * CCR * T(PCLK1)
			//			   T(High) + T(Low) = 25 * CCR * T(PCLK1)
			//			   CCR = (T(High) + T(Low)) / (25 * T(PCLK1))
			pI2CHandler->pI2Cx->CCR |= (uint32_t) ((T_I2C_SCL_SPEED) / (25 * T_APB1ClkFreq));
		} else { //To reach 100kHz

			//Calculation: T(High) = CCR * T(PCLK1)
			//			   T(Low) = 2 * CCR * T(PCLK1)
			//			   T(High) + T(Low) = 3 * CCR * T(PCLK1)
			//			   CCR = (T(High) + T(Low)) / (3 * T(PCLK1))
			pI2CHandler->pI2Cx->CCR |= (uint32_t) ((T_I2C_SCL_SPEED) / (3 * T_APB1ClkFreq));
		}
	} else { //Standard Mode

		//Calculation: T(High) = CCR * T(PCLK1)
		//			   T(Low)  =  CCR * T(PCLK1)
		//			   T(High) + T(Low) = 2 * CCR * T(PCLK1)
		//			   CCR = (T(High) + T(Low)) / (2 * T(PCLK1))
		pI2CHandler->pI2Cx->CCR |= (uint32_t) ((T_I2C_SCL_SPEED) / (2 * T_APB1ClkFreq));
	}

	//Configure the Rise Time



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
 * @param[in]			- Base address of the specific SPI peripherals (SPI_Reg_t* pI2Cx)
 * @param[in]			= ENABLE or DISABLE macro
 *
 * @return				- none
 * @note				- none
 */
void I2C_PeripheralEnable(I2C_Reg_t* pI2Cx, uint8_t EnOrDi) {
	if (EnOrDi) {
		pI2Cx->CR1 |= (1 << I2C_CR1_PE);
	} else {
		pI2Cx->CR1 &= ~(1 << I2C_CR1_PE);
	}
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