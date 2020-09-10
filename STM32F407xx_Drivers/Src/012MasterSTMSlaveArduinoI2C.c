/*
 * 012MasterSTMSlaveArduinoI2C.c
 *
 *  Created on: Sep 7, 2020
 *      Author: Donavan Tran
 *      Description: I2C Master(STM) and I2C Slave(Arduino) communication.
 *      			 When button on the master is pressed, master should send data to the
 *      			 Arduino slave connected. The data received by the Arduino will
 *      			 be displayed on the Arduino serial port
 *      			 1. Use I2C SCL = 100kHz (Standard mode)
 *      			 2. Use internal pull-up resistor for SDA and SCL lines
 */

#include "../drivers/Inc/stm32f407xx.h"

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

		I2C_MasterSendData(&I2C_Handler, data, strlen((char*) data), SLAVE_ADDR);

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
	I2C_GPIO->GPIOx_PinConfig.GPIO_PinNumber = GPIO_PIN_6 | GPIO_PIN_9;
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
