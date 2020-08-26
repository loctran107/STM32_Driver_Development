/**
  ******************************************************************************
  * @file    main.c
  * @author  Auto-generated by STM32CubeIDE
  * @version V1.0
  * @brief   Connect an external button to PD5 pin and toggle the LED whenever
  *  		 interrupt is triggered by the button press. Interrupt should be triggered
  *  		 during falling edge of button press.
  ******************************************************************************
*/

#include "../drivers/Inc/gpio_driver.h"

void delay() {
	for (uint32_t i = 0; i < 1000000 / 4; i++);
}

void LEDEnable(GPIO_Handle_t* GPIO_LED) {
	//Enable the LED
	//GPIO_Handle_t GPIO_LED;
	GPIO_LED->pGPIOx = GPIOD;
	GPIO_LED->GPIOx_PinConfig.GPIO_PinNumber = GPIO_PIN_2;
	GPIO_LED->GPIOx_PinConfig.GPIO_PinSpeed = GPIO_HIGH_SPEED;
	GPIO_LED->GPIOx_PinConfig.GPIO_PinPuPdCtrl = GPIO_NO_PU_PD;
	GPIO_LED->GPIOx_PinConfig.GPIO_PinMode = GPIO_OUTPUT_MODE;
	GPIO_LED->GPIOx_PinConfig.GPIO_PinOPType = GPIO_PUSH_PULL;
	GPIO_Init(GPIO_LED);
}

void ButtonEnable(GPIO_Handle_t* GPIO_Button) {
	//Enable the Button
	//GPIO_Handle_t GPIO_Button;
	GPIO_Button->pGPIOx = GPIOD;
	GPIO_Button->GPIOx_PinConfig.GPIO_PinNumber = GPIO_PIN_4;
	GPIO_Button->GPIOx_PinConfig.GPIO_PinPuPdCtrl = GPIO_NO_PU_PD;
	GPIO_Button->GPIOx_PinConfig.GPIO_PinMode = GPIO_INPUT_MODE;
	GPIO_Init(GPIO_Button);
}

/*
 * Write the program that handles the on board LED using on board
 * button
 */
int main(void)
{
	//Setting up the on board LED configuration

	//Enable the GPIO peripheral clock for on board LED and button
	GPIO_PeriClkCtrl(GPIOD, ENABLE);
	//GPIO_PeriClkCtrl(GPIOA, ENABLE);

	//On-board hardware enable
	GPIO_Handle_t GPIO_LED, GPIO_Button;
	LEDEnable(&GPIO_LED);
	ButtonEnable(&GPIO_Button);

	//handle the LED pressing application
	while (1) {
		if (GPIO_ReadFromInputPin(GPIO_Button.pGPIOx, GPIO_Button.GPIOx_PinConfig.GPIO_PinNumber) == BUTTON_PRESSED) {
			delay();
			//GPIO_WriteToOutputPin(GPIOD, GPIO_LED.GPIOx_PinConfig.GPIO_PinNumber, 1);
			GPIO_ToggleOutputPin(GPIO_LED.pGPIOx, GPIO_LED.GPIOx_PinConfig.GPIO_PinNumber);
			//delay();
		}
		/*} else {
			GPIO_WriteToOutputPin(GPIOD, GPIO_LED.GPIOx_PinConfig.GPIO_PinNumber, 0);
		}*/
	}
}
