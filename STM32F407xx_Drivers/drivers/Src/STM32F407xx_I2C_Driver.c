/*
 * STM32F407xx_I2C_Driver.c
 *
 *  Created on: Sep 3, 2020
 *      Author: Donavan Tran
 */

#include "../Inc/STM32F407xx_I2C_Driver.h"

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

/*****************************************************
 * @fn					- I2C_Init
 *
 * @brief				- Initialize the I2C port given the handle structure
 *
 * @param[in]			- Handle Structure of I2C that contains all SPI configuration and port
 *
 * @return				- none
 * @note				- none
 */
void I2C_Init(I2C_Handle_t* pI2CHandler) {


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
