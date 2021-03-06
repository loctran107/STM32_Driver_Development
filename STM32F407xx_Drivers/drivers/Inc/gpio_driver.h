/*
 * gpio_driver.h
 *
 *  Created on: Aug 18, 2020
 *      Author: Donavan Tran
 *      Description: This header file contains specific details
 *      			 related to the GPIO peripherals of the STM32F407xx
 */

#ifndef INC_GPIO_DRIVER_H_
#define INC_GPIO_DRIVER_H_
#include "stm32f407xx.h"

/*
 * @GPIO Pin Config
 */
#define GPIO_PIN_SET	SET
#define GPIO_PIN_RESET	RESET
#define BUTTON_PRESSED 	RESET //LOW

/*
 * @GPIO_PCLK_EN
 */
#define GPIO_PCLK_EN(__GPIOx__)    ((__GPIOx__) == (GPIOA)) ? GPIOA_PCLK_EN() :\
								   ((__GPIOx__) == (GPIOB)) ? GPIOB_PCLK_EN() :\
								   ((__GPIOx__) == (GPIOC)) ? GPIOC_PCLK_EN() :\
								   ((__GPIOx__) == (GPIOD)) ? GPIOD_PCLK_EN() :\
								   ((__GPIOx__) == (GPIOE)) ? GPIOE_PCLK_EN() :\
								   ((__GPIOx__) == (GPIOF)) ? GPIOF_PCLK_EN() :\
								   ((__GPIOx__) == (GPIOG)) ? GPIOG_PCLK_EN() :\
								   ((__GPIOx__) == (GPIOH)) ? GPIOH_PCLK_EN() :\
								   ((__GPIOx__) == (GPIOI)) ? GPIOI_PCLK_EN() :\
								   ((__GPIOx__) == (GPIOJ)) ? GPIOJ_PCLK_EN() : GPIOK_PCLK_EN()

/*
 * @GPIO_PCLK_DI
 */
#define GPIO_PCLK_DI(__GPIOx__)    ((__GPIOx__) == (GPIOA)) ? GPIOA_PCLK_DI() :\
								   ((__GPIOx__) == (GPIOB)) ? GPIOB_PCLK_DI() :\
								   ((__GPIOx__) == (GPIOC)) ? GPIOC_PCLK_DI() :\
								   ((__GPIOx__) == (GPIOD)) ? GPIOD_PCLK_DI() :\
								   ((__GPIOx__) == (GPIOE)) ? GPIOE_PCLK_DI() :\
								   ((__GPIOx__) == (GPIOF)) ? GPIOF_PCLK_DI() :\
								   ((__GPIOx__) == (GPIOG)) ? GPIOG_PCLK_DI() :\
								   ((__GPIOx__) == (GPIOH)) ? GPIOH_PCLK_DI() :\
								   ((__GPIOx__) == (GPIOI)) ? GPIOI_PCLK_DI() :\
								   ((__GPIOx__) == (GPIOJ)) ? GPIOJ_PCLK_DI() : GPIOK_PCLK_DI()

/*
 * @GPIO_PORT_INDEX
 */
#define GPIO_PORT_INDEX(__GPIOx__) 	((uint8_t) ((__GPIOx__) == (GPIOA)) ? 0U :\
											   ((__GPIOx__) == (GPIOB)) ? 1U :\
											   ((__GPIOx__) == (GPIOC)) ? 2U :\
											   ((__GPIOx__) == (GPIOD)) ? 3U :\
											   ((__GPIOx__) == (GPIOE)) ? 4U :\
											   ((__GPIOx__) == (GPIOF)) ? 5U :\
											   ((__GPIOx__) == (GPIOG)) ? 6U :\
											   ((__GPIOx__) == (GPIOH)) ? 7U : 8U)
/*
 * @GPIO_PIN_NUMBER
 */
#define GPIO_PIN_NUMBER 			((uint16_t) 16)
/*
 * @GPIO_PIN_NO
 */
#define GPIO_PIN_0				((uint16_t) 0x0001)  // Pin 0 selected
#define GPIO_PIN_1				((uint16_t) 0x0002)  // Pin 1 selected
#define GPIO_PIN_2				((uint16_t) 0x0004)  // Pin 2 selected
#define GPIO_PIN_3				((uint16_t) 0x0008)  // Pin 3 selected
#define GPIO_PIN_4				((uint16_t) 0x0010)  // Pin 4 selected
#define GPIO_PIN_5				((uint16_t) 0x0020)  // Pin 5 selected
#define GPIO_PIN_6				((uint16_t) 0x0040)  // Pin 6 selected
#define GPIO_PIN_7				((uint16_t) 0x0080)  // Pin 7 selected
#define GPIO_PIN_8				((uint16_t) 0x0100)  // Pin 8 selected
#define GPIO_PIN_9				((uint16_t) 0x0200)  // Pin 9 selected
#define GPIO_PIN_10				((uint16_t) 0x0400)  // Pin 10 selected
#define GPIO_PIN_11				((uint16_t) 0x0800)  // Pin 11 selected
#define GPIO_PIN_12				((uint16_t) 0x1000)  // Pin 12 selected
#define GPIO_PIN_13				((uint16_t) 0x2000)  // Pin 13 selected
#define GPIO_PIN_14				((uint16_t) 0x4000)  // Pin 14 selected
#define GPIO_PIN_15				((uint16_t) 0x8000)  // Pin 15 selected

#define GPIO_PIN_ALL				((uint16_t) 0xFFFF)  // All pin selected
/*
 * @GPIO_PIN_MODES
 */
#define GPIO_INPUT_MODE				0x0U
#define GPIO_OUTPUT_MODE			0x1U
#define GPIO_ALT_FUNC_MODE			0x2U
#define GPIO_ANALOG_MODE			0x3U
#define GPIO_IT_FT_MODE				0x4U /* GPIO Interrupt Falling Edge Trigger*/
#define GPIO_IT_RT_MODE				0x5U /* GPIO Interrupt Rising Edge Trigger */
#define GPIO_IT_RFT_MODE			0x6U /* GPIO Interrupt Both Falling and Rising Edge Trigger*/

/*
 * @GPIO_OUTPUT_TYPE
 */
#define GPIO_PUSH_PULL				0x0U
#define GPIO_OPEN_DRAIN				0x1U

/*
 * @GPIO_SPEED
 */
#define GPIO_LOW_SPEED				0x0U
#define GPIO_MEDIUM_SPEED			0x1U
#define GPIO_HIGH_SPEED				0x2U
#define GPIO_VERY_HIGH_SPEED		0x3U

/*
 * @GPIO_PULL_UP_PULL_DOWN
 */
#define GPIO_NO_PU_PD				0x0U
#define GPIO_PU						0x1U
#define GPIO_PD						0x2U

/*
 * @GPIO_ALT_FUNC_OPTION
 */
#define AF0							((uint8_t) 0x0)
#define AF1							((uint8_t) 0x1)
#define AF2							((uint8_t) 0x2)
#define AF3							((uint8_t) 0x3)
#define AF4							((uint8_t) 0x4)
#define AF5							((uint8_t) 0x5)
#define AF6							((uint8_t) 0x6)
#define AF7							((uint8_t) 0x7)
#define AF8							((uint8_t) 0x8)
#define AF9							((uint8_t) 0x9)
#define AF10						((uint8_t) 0xA)
#define AF11						((uint8_t) 0xB)
#define AF12						((uint8_t) 0xC)
#define AF13						((uint8_t) 0xD)
#define AF14						((uint8_t) 0xE)
#define AF15						((uint8_t) 0xF)
/*
 * Configuration structure of the GPIO pins
 */
typedef struct {
	uint32_t GPIO_PinNumber;
	uint32_t GPIO_PinMode;  /*!< Possible values from @GPIO_PIN_MODES >*/
	uint32_t GPIO_PinSpeed; /*!< Possible values from @GPIO_SPEED >*/
	uint32_t GPIO_PinPuPdCtrl; /*!< Possible values from @GPIO_PULL_UP_PULL_DOWN >*/
	uint32_t GPIO_PinOPType; /*!< Possible values from @PGPIO_OUTPUT_TYPE >*/
	uint32_t GPIO_PinAltFuncMode;
} GPIO_PinConfig_t;

/*
 * Handle structure for GPIO pins
 */
typedef struct {

	/* Pointer to hold the base address of the GPIO peripherals*/
	GPIO_Reg_t* pGPIOx; //This holds the base address of the GPIO port which the pins belong to
	GPIO_PinConfig_t GPIOx_PinConfig; //configure structure of the GPIO pin

} GPIO_Handle_t;

/*****************************************************************************************************
 *									API SUPPORTED FOR THIS GPIO DRIVER 								 *
 *				For more information about this API, check the function description.				 *
 *****************************************************************************************************/

/* GPIO Function Prototype Declaration */

/*
 * Peripheral clock control
 * Parameter 1: Pointer to the base address of the GPIO port
 * Parameter 2: 8 bit integer that tells whether to enable or disable
 *              the clock peripheral
 */
void GPIO_PeriClkCtrl(GPIO_Reg_t* pGPIOx, uint8_t EnOrDi);

/*
 * GPIO initialization and de-initialization
 * Parameter: Pointer to the GPIO Handle Structure
 */
void GPIO_Init(GPIO_Handle_t* pGPIOHandler);

/* Consult the RCC Peripheral reset registers for more details*/
void GPIO_DeInit(GPIO_Reg_t *pGPIOx);

/*
 * GPIO Data Read/Write
 */
uint8_t GPIO_ReadFromInputPin(GPIO_Reg_t* pGPIOx, uint16_t pinNumber);
uint16_t GPIO_ReadFromInputPort(GPIO_Reg_t* pGPIOx);
void GPIO_WriteToOutputPin(GPIO_Reg_t* pGPIOx, uint16_t pinNumber, uint8_t value);
void GPIO_WriteToOutputPort(GPIO_Reg_t* pGPIOx, uint16_t value);

/*
 * GPIO Toggle Output pin
 */
void GPIO_ToggleOutputPin(GPIO_Reg_t* pGPIOx, uint32_t pinNumber);

/*
 * GPIO Interrupt Configuration and Handling
 */
void GPIO_IRQITConfig(uint8_t IRQNumber, uint8_t EnOrDi);
void GPIO_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriorityValue);
void GPIO_IRQHandling(uint8_t pinNumber);

#endif /* INC_GPIO_DRIVER_H_ */
