/*
 * 016StmUSARTArduinoTx.c
 *
 *  Created on: Sep 19, 2020
 *      Author: Donavan Tran
 *      Description: Write a program to send some message over UART from STM32
 *      			 board to Arduino board. The Arduino board will display
 *      			 the message (on Arduino Serial monitor) sent from the ST
 *      			 board
 *
 *      			 Baudrate: 115200 bps
 *      			 Frame format: 1 stop bit, 8 bits data frame, no parity
 */

#include "../drivers/Inc/stm32f407xx.h"

GPIO_Handle_t GPIO_Button;
GPIO_Handle_t USART_GPIO;
USART_Handle_t USART_Handler;

/*
 * Helper function prototypes
 */
void delay();
void GPIO_Button_Init();
void USART_GPIO_Init();
void USART_Handler_Init();
char TxBuffer[1024] = "Testing USART Tx...\n\r";
int main(void) {

	//Set all element to 0
	memset(&GPIO_Button, 0, sizeof(GPIO_Button));
	memset(&USART_GPIO, 0, sizeof(USART_GPIO));
	memset(&USART_Handler, 0, sizeof(USART_Handler));

	GPIO_Button_Init();
	USART_GPIO_Init();
	USART_Handler_Init();

	USART_PeripheralEnable(USART2, ENABLE);

	while(1) {
		while (!GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_0));

		//Rebouncing delay
		delay();

		USART_SendData(&USART_Handler, ((uint8_t*) TxBuffer), strlen(TxBuffer));
	}

	return EXIT_SUCCESS;

}

void delay() {
	for (uint32_t i = 0; i < 500000; i++);
}

void GPIO_Button_Init() {
	GPIO_Button.pGPIOx = GPIOA;
	GPIO_Button.GPIOx_PinConfig.GPIO_PinMode = GPIO_INPUT_MODE;
	GPIO_Button.GPIOx_PinConfig.GPIO_PinNumber = GPIO_PIN_0;
	GPIO_Button.GPIOx_PinConfig.GPIO_PinPuPdCtrl = GPIO_NO_PU_PD;
	GPIO_Button.GPIOx_PinConfig.GPIO_PinSpeed = GPIO_HIGH_SPEED;
	GPIO_Init(&GPIO_Button);
}

void USART_GPIO_Init() {

	//Use USART2
	//USART2_TX		: PA2
	//USART3_Rx		: PA3
	USART_GPIO.pGPIOx = GPIOA;
	USART_GPIO.GPIOx_PinConfig.GPIO_PinMode = GPIO_ALT_FUNC_MODE;
	USART_GPIO.GPIOx_PinConfig.GPIO_PinAltFuncMode = AF7;
	USART_GPIO.GPIOx_PinConfig.GPIO_PinOPType = GPIO_PUSH_PULL;
	USART_GPIO.GPIOx_PinConfig.GPIO_PinPuPdCtrl = GPIO_PU;
	USART_GPIO.GPIOx_PinConfig.GPIO_PinSpeed = GPIO_HIGH_SPEED;
	USART_GPIO.GPIOx_PinConfig.GPIO_PinNumber = GPIO_PIN_2 | GPIO_PIN_3;
	GPIO_Init(&USART_GPIO);

}

void USART_Handler_Init() {

	//Using USART2
	USART_Handler.pUSARTx = USART2;
	USART_Handler.USART_Config.HWFlowControl = USART_HW_FLOW_CTRL_NONE;
	USART_Handler.USART_Config.Mode = USART_MODE_ONLY_TX;
	USART_Handler.USART_Config.Oversampling = USART_OVERSAMPLING_BY_16;
	USART_Handler.USART_Config.BaudRate = USART_STD_BAUD_115200;
	USART_Handler.USART_Config.NoOfStopBits = USART_STOPBITS_1;
	USART_Handler.USART_Config.ParityControl = USART_PARITY_DI;
	USART_Handler.USART_Config.WordLength = USART_WORDLEN_8BITS;
	USART_Init(&USART_Handler);
}
