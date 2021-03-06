/*
 * 009SPIMasterArduinoSlave.c
 *
 *  Created on: Aug 29, 2020
 *      Author: Donavan Tran
 *      Description: SPI Master (STM) and SPI Slave (Arduino) communication
 *      		  	 When the button on the master is pressed, master should send string
 *      		  	 of data to the Arduino slave connected. The data received by
 *      		  	 the Arduino will be displayed on the Arduino serial port
 *      		  	 1. Use full SPI full duplex mode
 *      		  	 2. ST board will be in SPI master mode and Arduino will be
 *      		  	    configured for SPI Slave mode
 *      		  	 3. Use DFF = 0;
 *      			 4. Use hardware slave management (SSM = 0)
 *      			 5. SCLK speed = 2MHz, fclk = 16MHz
 *
 *      			 Note: In this exercise, master is not going to receive anything from the slave.
 *      			       Thus, the master's MISO can be disabled
 *
 *      			  	   Slave does not know how many bytes of data master is going to send.
 *      			  	   So master first sends the number bytes info which slaves is going to
 *      			  	   receive next.
 */

#include "../drivers/Inc/stm32f407xx.h"

void GPIO_ButtonInit(GPIO_Handle_t* GPIO_Button);
void SPI_GPIOInit(GPIO_Handle_t* SPI_GPIO);
void SPI_MasterInit(SPI_Handle_t* SPI_Master);
void delay();

int main(void) {

	//Define dummy string to send to SPI Slave (Arduino)
	char* str = "Hello World";

	//Define handle structure for GPIO button, GPIO for SPI, and SPI1
	GPIO_Handle_t GPIO_Button, SPI_GPIO;
	SPI_Handle_t SPI_Master;

	//Set everything to 0 by default
	memset(&GPIO_Button, 0, sizeof(GPIO_Button));
	memset(&SPI_GPIO, 0, sizeof(SPI_GPIO));
	memset(&SPI_Master, 0, sizeof(SPI_Master));

	//Initialize GPIO button
	GPIO_ButtonInit(&GPIO_Button);
	SPI_GPIOInit(&SPI_GPIO);
	SPI_MasterInit(&SPI_Master);


	while (1) {

		//When the button is pressed, SPI Master (ST) sends str data to SPI Slave (Arduino)
		if (!GPIO_ReadFromInputPin(GPIO_Button.pGPIOx, GPIO_PIN_4)) {
			delay(); //wait until the debouncing is over ~200ms

			//Enable the SPI peripheral
			SPI_PeripheralEnable(SPI2, ENABLE);

			//First, send the length information
			uint8_t dataLen = strlen(str);
			SPI_SendData(SPI_Master.pSPIx, &dataLen , 1);

			//Send data to SPI Slave
			SPI_SendData(SPI_Master.pSPIx, (uint8_t*) str, strlen(str));

			//Wait until the Master is done transferring the bytes of data
			//If busy, stay there. Otherwise, disable the peripheral
			while (SPI_CheckStatusFlag(SPI_Master.pSPIx, SPI_BUSY_FLAG));

			//Disable the SPI peripheral
			SPI_PeripheralEnable(SPI2, DISABLE);
		}

	}

	return EXIT_SUCCESS;
}

void GPIO_ButtonInit(GPIO_Handle_t* GPIO_Button) {

	//Enable button to be of PD4
	GPIO_Button->pGPIOx = GPIOD;
	GPIO_Button->GPIOx_PinConfig.GPIO_PinMode = GPIO_INPUT_MODE;
	GPIO_Button->GPIOx_PinConfig.GPIO_PinNumber = GPIO_PIN_4;
	GPIO_Button->GPIOx_PinConfig.GPIO_PinOPType = GPIO_PUSH_PULL;
	GPIO_Button->GPIOx_PinConfig.GPIO_PinPuPdCtrl = GPIO_NO_PU_PD;
	GPIO_Button->GPIOx_PinConfig.GPIO_PinSpeed = GPIO_HIGH_SPEED;
	GPIO_Init(GPIO_Button);
}

void SPI_GPIOInit(GPIO_Handle_t* SPI_GPIO) {

	//According to the alternate function map table
	//PA4 --> SPI1_NSS
	//PA5 --> SPI1_SCK
	//PA6 --> SPI1_MISO
	//PA7 --> SPI1_MOSI
	SPI_GPIO->pGPIOx = GPIOB;
	SPI_GPIO->GPIOx_PinConfig.GPIO_PinMode = GPIO_ALT_FUNC_MODE;
	SPI_GPIO->GPIOx_PinConfig.GPIO_PinAltFuncMode = AF5;
	SPI_GPIO->GPIOx_PinConfig.GPIO_PinNumber = GPIO_PIN_13 | GPIO_PIN_15 | GPIO_PIN_12;
	SPI_GPIO->GPIOx_PinConfig.GPIO_PinOPType = GPIO_PUSH_PULL;
	SPI_GPIO->GPIOx_PinConfig.GPIO_PinPuPdCtrl = GPIO_PU;
	SPI_GPIO->GPIOx_PinConfig.GPIO_PinSpeed = GPIO_HIGH_SPEED;
	GPIO_Init(SPI_GPIO);
}

void SPI_MasterInit(SPI_Handle_t* SPI_Master) {
	SPI_Master->pSPIx = SPI2;
	SPI_Master->SPI_Config.BusConfig = SPI_BUS_CONFIG_FULL_DUPLX;
	SPI_Master->SPI_Config.DeviceMode = SPI_DEVICE_MASTER_MODE;
	SPI_Master->SPI_Config.DFF = SPI_DFF_8_BIT;
	SPI_Master->SPI_Config.SSM = SPI_HSM;
	SPI_Master->SPI_Config.SclkSpeed = SPI_SCLK_SPEED_DIV8;
	SPI_Master->SPI_Config.CPHAConfig = SPI_CPHA_LOW;
	SPI_Master->SPI_Config.CPOLConfig = SPI_CPOL_LOW;
	SPI_Init(SPI_Master);
}

void delay() {
	for (uint32_t i = 0; i < 500000 / 2; i++);
}

