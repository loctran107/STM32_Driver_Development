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
		GPIO_PCLK_EN(pGPIOx);
	} else {
		GPIO_PCLK_DI(pGPIOx);
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

	//uint32_t temp;
	GPIO_Reg_t* GPIOx = pGPIOHandler->pGPIOx;
	GPIO_PinConfig_t GPIOx_PinConf = pGPIOHandler->GPIOx_PinConfig;

	//Enable the GPIO Clock
	GPIO_PeriClkCtrl(GPIOx, ENABLE);

	/*Note: before setting the bits: make sure those registers are clear */
	//Clearing the registers
	//Setting the bits register
	for (uint16_t i = 0U; i < GPIO_PIN_NUMBER; i++) {

		//Check if the ith bit is set
		if (GPIOx_PinConf.GPIO_PinNumber & (1 << i)) {

			//Handle the GPIO pin alternate function mode and GPIO output mode
			if (GPIOx_PinConf.GPIO_PinMode == GPIO_ALT_FUNC_MODE || GPIOx_PinConf.GPIO_PinMode == GPIO_OUTPUT_MODE) {
				if (i < (GPIO_PIN_NUMBER / 2)) {
					GPIOx->AFR[0] |= (GPIOx_PinConf.GPIO_PinAltFuncMode << i * 4U);
				} else {
					GPIOx->AFR[1] |= (GPIOx_PinConf.GPIO_PinAltFuncMode << (i % (GPIO_PIN_NUMBER / 2)) * 4U);
				}

				//Configure the OTYPER register
				GPIOx->OTYPER |= (GPIOx_PinConf.GPIO_PinOPType << i);

				//Configure the OSPEED register
				GPIOx->OSPEEDR |= (GPIOx_PinConf.GPIO_PinSpeed << i * 2U);
			}

			//Handle the GPIO pin interrupt mode
			if ((GPIOx_PinConf.GPIO_PinMode == GPIO_IT_FT_MODE) || (GPIOx_PinConf.GPIO_PinMode == GPIO_IT_RT_MODE)
				|| (GPIOx_PinConf.GPIO_PinMode == GPIO_IT_RFT_MODE)) {

				//Enable the clock for SYSCFG registers
				SYSCFG_PCLK_EN();

				//Handling the falling edge cases
				if (GPIOx_PinConf.GPIO_PinMode == GPIO_IT_FT_MODE) {
					//Configure the FTSR
					EXTI->FTSR |= (1 << i);
					EXTI->RTSR &= ~(1 << i); //Clear the corresponding RTSR bit
				} else if (GPIOx_PinConf.GPIO_PinMode == GPIO_IT_RT_MODE) {
					//Configure the RTSR
					EXTI->RTSR |= (1 << i);
					EXTI->FTSR &= ~(1 << i); //Clear the corresponding FTSR bit
				}  else {
					//Configure both the FTSR and RTSR
					EXTI->FTSR |= (1 << i);
					EXTI->RTSR |= (1 << i);
				}

				//Configure the GPIO port selection in SYSCFG_EXTICR
				//Rule: int quotient = n >> z
				//		int remainder = n & (~(~(int) 0) << z)
				//In this case, since we always want z = 2, as we divide by 4,
				//the term ~(~(int) 0) << z) = 0x03U
				SYSCFG->EXTICR[i >> 2U] |= GPIO_PORT_INDEX(GPIOx) << ((i & 0x03U) * 4U);

				//Enable the EXTI interrupt delivery using IMR
				EXTI->IMR |= (1 << i);
			}

			//Handle the MODER register
			GPIOx->MODER |= (GPIOx_PinConf.GPIO_PinMode << i * 2U);

			//Handle the PuPdCtrl Register
			GPIOx->PUPDR |= (GPIOx_PinConf.GPIO_PinPuPdCtrl << i * 2U);
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
uint8_t GPIO_ReadFromInputPin(GPIO_Reg_t* pGPIOx, uint16_t pinNumber) {
	//value = (uint8_t) ((pGPIOx->IDR & (1 << pinNumber)) >> pinNumber);
	/*or value = (uint8_t) ((pGPIOx->IDR >> pinNumber) & 0x1);*/
	return ((pGPIOx->IDR & pinNumber) == pinNumber) ? 1 : 0;
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
void GPIO_WriteToOutputPin(GPIO_Reg_t* pGPIOx, uint16_t pinNumber, uint8_t value) {
	if (value) {
		pGPIOx->BSRR = pinNumber;
	} else {
		pGPIOx->BSRR = pinNumber << GPIO_PIN_NUMBER;
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
 * @fn					- GPIO_IRQITConfig
 *
 * @brief				- Interrupt Request configuration of the GPIO
 *
 * @param[in]			- unsigned integer 8 bit interrupt request number
 * @param[in]			- unsigned integer 8 bit interrupt request priority
 * @param[in]			- unsigned integer 8 bit enable or disable macros
 *
 * @return				- none
 * @note				- Refer to the Cortex M4 Generic User Guide the NVIC register table
 */
void GPIO_IRQITConfig(uint8_t IRQNumber, uint8_t EnOrDi) {

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
 * @fn					- GPIO_IRQPriorityConfig
 *
 * @brief				- Configuring the priority of the specific IRQ Number
 *
 * @param[in]			- unsigned integer 8 bit interrupt request number
 * @param[in]			- unsigned integer 8 bit interrupt request priority
 *
 * @return				- none
 * @note				- Refer to the Cortex M4 Generic User Guide the NVIC register table
 */
void GPIO_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriorityValue) {
	uint32_t indx = IRQNumber >> 2U; //Note: There are IRQ Priority field in each IPR register
	uint32_t remainder = IRQNumber & (~(~(int)0 << 2U));
	uint32_t shift_amount = (remainder * 8U) + IMPLEMENTED_IRQ_PRIORITY_BIT;

	//Configure the IRQ_PR register
	NVIC_IPR(indx) |= IRQPriorityValue << shift_amount;
}


/*****************************************************
 * @fn					- GPIO_IRQHandling
 *
 * @brief				- Handling the GPIO interrupt by clearing the EXTI pending register bit
 *
 * @param[in]			- GPIOx pin number
 * @param[in]			-
 *
 * @return				- none
 * @note				- Interrupt handling may varies depending on the peripheral you're using
 */
void GPIO_IRQHandling(uint8_t pinNumber) {

	if ((EXTI->PR & pinNumber) == pinNumber) {
		EXTI->PR |= 1 << pinNumber; //clear the EXTI pending register bit at the corresponding pin number
	}
}

