/*
 * gpio_driver.c
 *
 *  Created on: Aug 18, 2020
 *      Author: Donavan Tran
 *      Description: This is the source code specific to the GPIO driver
 *      			 of the STM32F407xx architecture
 */
#include "../Inc/gpio_driver.h"

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
void GPIO_PeriClkCtrl(GPIO_Reg_t* pGPIOx, uint8_t EnOrDi) {

	//Enable or disable the GPIO clock peripherals
	if (EnOrDi) {

		//Special note: switch cannot be used for pointer, only integer
		if (pGPIOx == GPIOA) {
			GPIOA_PCLK_EN();
		} else if (pGPIOx == GPIOB) {
			GPIOB_PCLK_EN();
		} else if (pGPIOx == GPIOC) {
			GPIOC_PCLK_EN();
		} else if (pGPIOx == GPIOD) {
			GPIOD_PCLK_EN();
		} else if (pGPIOx == GPIOE) {
			GPIOE_PCLK_EN();
		} else if (pGPIOx == GPIOF) {
			GPIOF_PCLK_EN();
		} else if (pGPIOx == GPIOG) {
			GPIOG_PCLK_EN();
		} else if (pGPIOx == GPIOH) {
			GPIOH_PCLK_EN();
		} else if (pGPIOx == GPIOI) {
			GPIOI_PCLK_EN();
		} else if (pGPIOx == GPIOJ) {
			GPIOJ_PCLK_EN();
		} else {
			GPIOK_PCLK_EN();
		}
	} else {
		if (pGPIOx == GPIOA) {
			GPIOA_PCLK_DI();
		} else if (pGPIOx == GPIOB) {
			GPIOB_PCLK_DI();
		} else if (pGPIOx == GPIOC) {
			GPIOC_PCLK_DI();
		} else if (pGPIOx == GPIOD) {
			GPIOD_PCLK_DI();
		} else if (pGPIOx == GPIOE) {
			GPIOE_PCLK_DI();
		} else if (pGPIOx == GPIOF) {
			GPIOF_PCLK_DI();
		} else if (pGPIOx == GPIOG) {
			GPIOG_PCLK_DI();
		} else if (pGPIOx == GPIOH) {
			GPIOH_PCLK_DI();
		} else if (pGPIOx == GPIOI) {
			GPIOI_PCLK_DI();
		} else if (pGPIOx == GPIOJ) {
			GPIOJ_PCLK_DI();
		} else {
			GPIOK_PCLK_DI();
		}
	}
}


/*****************************************************
 * @fn					- GPIO_Init
 *
 * @brief				- Initialize the given GPIO port
 *
 * @param[in]			- Handle structure of the GPIO pin
 * @param[in]			-
 *
 * @return				- none
 * @note				- none
 */
void GPIO_Init(GPIO_Handle_t* pGPIOHandler) {
	GPIO_Reg_t* GPIOx = pGPIOHandler->pGPIOx;
	GPIO_PinConfig_t GPIOx_PinConf = pGPIOHandler->GPIOx_PinConfig;

	/*Note: before setting the bits: make sure those registers are clear */
	//Clearing the registers
	GPIOx->MODER &= ~0xFFFFFFFF;
	GPIOx->OTYPER &= ~0xFFFFFFFF;
	GPIOx->OSPEEDR &= ~0xFFFFFFFF;
	GPIOx->PUPDR &= ~0xFFFFFFFF;
	GPIOx->AFR[0] &= ~0xFFFFFFFF;
	GPIOx->AFR[1] &= ~0xFFFFFFFF;

	//Setting the bits register
	for (uint16_t i = 0U; i < GPIO_PIN_NUMBER; i++) {

		//Check if the ith bit is set
		if (GPIOx_PinConf.GPIO_PinNumber & (1 << i)) {

			//Handle the non-interrupt case if the ith bit is set
			if (GPIOx_PinConf.GPIO_PinMode <= GPIO_ANALOG_MODE) {

				//Handle the GPIO pin OUTPUT Mode
				if (GPIOx_PinConf.GPIO_PinMode == GPIO_OUTPUT_MODE) {

					//Configure the OTYPER register
					GPIOx->OTYPER |= (GPIOx_PinConf.GPIO_PinOPType << i);

					//Configure the OSPEED register
					GPIOx->OSPEEDR |= (GPIOx_PinConf.GPIO_PinSpeed << i * 2U);
				}

				if (GPIOx_PinConf.GPIO_PinMode == GPIO_ALT_FUNC_MODE) {

					//Handle the alternate function mode register
					if (i < (GPIO_PIN_NUMBER / 2)) {
						GPIOx->AFR[0] |= (GPIOx_PinConf.GPIO_PinAltFuncMode << i * 4U);
					} else {
						GPIOx->AFR[1] |= (GPIOx_PinConf.GPIO_PinAltFuncMode << (i % (GPIO_PIN_NUMBER / 2)) * 4U);
					}
				}

				//Handle the MODER register
				GPIOx->MODER |= (GPIOx_PinConf.GPIO_PinMode << i * 2U);

				//Handle the PuPdCtrl Register
				GPIOx->PUPDR |= (GPIOx_PinConf.GPIO_PinPuPdCtrl << i * 2U);

			} else {
				//handle the interrupt code later
			}
		}
	}
}

/*****************************************************
 * @fn					- GPIO_DeInit
 *
 * @brief				- De-initialize the all of the GPIO pin details
 *
 * @param[in]			- Base address of the GPIO port
 * @param[in]			-
 *
 * @return				- none
 * @note				-  Consult the RCC Peripheral reset registers to reset
 * 					       all the registers of the peripherals
 */
void GPIO_DeInit(GPIO_Reg_t *pGPIOx) {

	//Reset all the registers of the respective GPIO peripherals
	if (pGPIOx == GPIOA) {
		GPIOA_PCLK_RST();
	} else if (pGPIOx == GPIOB) {
		GPIOB_PCLK_RST();
	} else if (pGPIOx == GPIOC) {
		GPIOC_PCLK_RST();
	} else if (pGPIOx == GPIOD) {
		GPIOD_PCLK_RST();
	} else if (pGPIOx == GPIOE) {
		GPIOE_PCLK_RST();
	} else if (pGPIOx == GPIOF) {
		GPIOF_PCLK_RST();
	} else if (pGPIOx == GPIOG) {
		GPIOG_PCLK_RST();
	} else if (pGPIOx == GPIOH) {
		GPIOH_PCLK_RST();
	} else if (pGPIOx == GPIOI) {
		GPIOI_PCLK_RST();
	} else if (pGPIOx == GPIOJ) {
		GPIOJ_PCLK_RST();
	} else if (pGPIOx == GPIOK) {
		GPIOK_PCLK_RST();
	}
}

/*****************************************************
 * @fn					- GPIO_ReadFromInputPin
 *
 * @brief				- Read from the input pin
 *
 * @param[in]			- Base address of the GPIO port
 * @param[in]			- The pin number
 *
 * @return				- unsigned 8 bit integer
 * @note				- none
 */
uint8_t GPIO_ReadFromInputPin(GPIO_Reg_t* pGPIOx, uint8_t pinNumber) {
	//value = (uint8_t) ((pGPIOx->IDR & (1 << pinNumber)) >> pinNumber);
	/*or value = (uint8_t) ((pGPIOx->IDR >> pinNumber) & 0x1);*/
	return (uint8_t) ((pGPIOx->IDR >> pinNumber) & 0x1);
}

/*****************************************************
 * @fn					- GPIO_ReadFromInputPort
 *
 * @brief				- Read from the GPIO input port
 *
 * @param[in]			- GPIO base address
 * @param[in]			-
 *
 * @return				- unsigned 16 bit integer
 * @note				- none
 */
uint16_t GPIO_ReadFromInputPort(GPIO_Reg_t* pGPIOx) {
	return (uint16_t) pGPIOx->IDR;
}

/*****************************************************
 * @fn					- GPIO_WriteToOutputPin
 *
 * @brief				- Write either 1 or 0 to a particular GPIO pin
 *
 * @param[in]			- GPIO base address
 * @param[in]			- unsigned 8 bit integer pin number
 * @param[in] 			- unsigned 8 bit integer value
 * @return				- none
 * @note				- none
 */
void GPIO_WriteToOutputPin(GPIO_Reg_t* pGPIOx, uint8_t pinNumber, uint8_t value) {
	if (value) {
		pGPIOx->ODR |= (1 << pinNumber);
	} else {
		pGPIOx->ODR &= ~(1 << pinNumber);
	}
}

/*****************************************************
 * @fn					- GPIO_WriteToOutputPort
 *
 * @brief				- Write to the GPIO output port
 *
 * @param[in]			- GPIO base address
 * @param[in]			- unsigned 16 bit integer value
 *
 *
 * @return				- none
 * @note				- none
 */
void GPIO_WriteToOutputPort(GPIO_Reg_t* pGPIOx, uint16_t value) {
	pGPIOx->ODR = value;
}

/*****************************************************
 * @fn					- GPIO_ToggleOutputPin
 *
 * @brief				- Toggling the Pin on and off
 *
 * @param[in]			- Base address of the GPIO port
 * @param[in]			- unsigned 8 bit integer pin number
 *
 * @return				- none
 * @note				- none
 */
void GPIO_ToggleOutputPin(GPIO_Reg_t* pGPIOx, uint32_t pinNumber) {

	/*
	 * There are two approaches to solve this toggling pin problems.
	 * First: Determine the number of bits shifted of the pinNumber,
	 * 		  then configure the ODR register appropriately by XOR the value
	 * 		  This method is much less efficient when toggling multiple pins
	 * Second: Use BSRR register and check if the ODR is currently on or off at
	 * 		   the corresponding pin
	 */

	/*Second method*/
	if ((pGPIOx->ODR & pinNumber) == pinNumber) { //all the bits at pinNumber are set
		pGPIOx->BSRR = (pinNumber << GPIO_PIN_NUMBER);
	} else {
		pGPIOx->BSRR = pinNumber;
	}

}

/*****************************************************
 * @fn					-
 *
 * @brief				-
 *
 * @param[in]			-
 * @param[in]			-
 *
 * @return				- none
 * @note				- none
 */
void GPIO_IRQConfig(uint8_t IRQNumber, uint8_t IRQPriority, uint8_t EnOrDi) {

}


/*****************************************************
 * @fn					-
 *
 * @brief				-
 *
 * @param[in]			-
 * @param[in]			-
 *
 * @return				- none
 * @note				- none
 */
void GPIO_IRQHandling(uint8_t pinNumber) {

}
