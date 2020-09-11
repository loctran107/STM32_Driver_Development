/*
 * 013MasterSTMSlaveArduinoRecepSend.c
 *
 *  Created on: Sep 10, 2020
 *      Author: Donavan Tran
 *      Description: When button on the master is pressed, master should read and display data
 *      		     from Arduino Slave connected. First master has to get the length of the data
 *      		     from the slave to read subsequent data from the slave
 *
 *      		     1. Use I2C SCL = 100 kHz (Standard mode)
 *      		     2. Use internall pull-up resistors for SDA and SCl lines
 *
 */

#include "../drivers/Inc/stm32f407xx.h"

#include <stdio.h>
#define COMMAND_REQUEST			0x51
#define COMMAND_READ_DATA		0x52
#define MY_ADDR			0x61
#define SLAVE_ADDR		0x68
void GPIO_ButtonInit(GPIO_Handle_t* GPIO_Button);
void GPIO_I2CInit(GPIO_Handle_t* I2C_GPIO);
void I2C_HandlerInit(I2C_Handle_t* I2C_Handler);
void delay();

//some data
//Note: data should be less than 32 bytes in one transaction (a.k.a less than 32 characters)
uint8_t data[30] = " Testing I2C Master Tx\n";

int main() {

	uint8_t dummy[30]; //dummy buffer
	//Define the handle structures
	GPIO_Handle_t GPIO_Button, I2C_GPIO;
	I2C_Handle_t  I2C_Handler;
	memset(&GPIO_Button, 0, sizeof(GPIO_Button));
	memset(&I2C_GPIO, 0, sizeof(GPIO_Button));
	memset(&I2C_Handler, 0, sizeof(GPIO_Button));

	GPIO_ButtonInit(&GPIO_Button);
	GPIO_I2CInit(&I2C_GPIO);
	I2C_HandlerInit(&I2C_Handler);


	//Enable the I2C Peripheral enable
	I2C_PeripheralEnable(I2C_Handler.pI2Cx, ENABLE);


	while (1) {
		while(!GPIO_ReadFromInputPin(GPIOA,GPIO_PIN_0));

		//to avoid button de-bouncing related issues 200ms of delay
		delay();

		//Send the command code to slave
		uint8_t command = COMMAND_REQUEST;
		I2C_MasterSendData(&I2C_Handler, &command, 1, SLAVE_ADDR);

		delay();

		//Read the length (1 byte) of the data from slave
		uint8_t len_data;
		I2C_MasterReceiveData(&I2C_Handler, &len_data, 1, SLAVE_ADDR);

		//Master sends command code 0x52 to read complete data from slave
		command = COMMAND_READ_DATA;
		I2C_MasterSendData(&I2C_Handler, &command, 1, SLAVE_ADDR);

		delay();

		//Read the complete data from the slave
		I2C_MasterReceiveData(&I2C_Handler, dummy, sizeof(dummy), SLAVE_ADDR);
		dummy[len_data] = '\0';
		printf("%s", dummy);

	}

	return EXIT_SUCCESS;
}

void GPIO_ButtonInit(GPIO_Handle_t* GPIO_Button) {
	GPIO_Button->pGPIOx = GPIOA;
	GPIO_Button->GPIOx_PinConfig.GPIO_PinMode = GPIO_INPUT_MODE;
	GPIO_Button->GPIOx_PinConfig.GPIO_PinNumber = GPIO_PIN_0;
	GPIO_Button->GPIOx_PinConfig.GPIO_PinPuPdCtrl = GPIO_NO_PU_PD;
	GPIO_Button->GPIOx_PinConfig.GPIO_PinSpeed = GPIO_HIGH_SPEED;

	GPIO_DeInit(GPIO_Button->pGPIOx);
	GPIO_Init(GPIO_Button);
}

void GPIO_I2CInit(GPIO_Handle_t* I2C_GPIO) {
	I2C_GPIO->pGPIOx = GPIOB;
	I2C_GPIO->GPIOx_PinConfig.GPIO_PinMode = GPIO_ALT_FUNC_MODE;
	I2C_GPIO->GPIOx_PinConfig.GPIO_PinAltFuncMode = AF4;
	I2C_GPIO->GPIOx_PinConfig.GPIO_PinNumber = GPIO_PIN_6 | GPIO_PIN_7;
	I2C_GPIO->GPIOx_PinConfig.GPIO_PinOPType = GPIO_OPEN_DRAIN;
	I2C_GPIO->GPIOx_PinConfig.GPIO_PinPuPdCtrl = GPIO_PU;
	I2C_GPIO->GPIOx_PinConfig.GPIO_PinSpeed = GPIO_MEDIUM_SPEED;

	GPIO_DeInit(I2C_GPIO->pGPIOx);
	GPIO_Init(I2C_GPIO);
}

void I2C_HandlerInit(I2C_Handle_t* I2C_Handler) {
	I2C_Handler->pI2Cx = I2C1;
	I2C_Handler->I2C_Config.ACKControl = I2C_ACK_EN;
	I2C_Handler->I2C_Config.FMDutyCycle = I2C_FM_DUTY_2;
	I2C_Handler->I2C_Config.SCLSpeed = I2C_SCL_SPEED_SM;
	I2C_Handler->I2C_Config.DeviceAddress = MY_ADDR;

	I2C_DeInit(I2C_Handler->pI2Cx);
	I2C_Init(I2C_Handler);
}

void delay() {
	for (uint32_t i; i < 10000000; i++);
}
