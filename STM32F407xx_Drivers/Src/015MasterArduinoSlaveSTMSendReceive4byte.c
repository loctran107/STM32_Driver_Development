/*
 * 013MasterSTMSlaveArduinoRecepSend.c
 *
 *  Created on: Sep 15, 2020
 *      Author: Donavan Tran
 *      Description: Master Arduino and Slave STM32. Master should read and display data
 *      			 from STM32 connected. First master has to read the length from the
 *      			 slave to read subsequent data from the slave
 *
 *      		     1. Use I2C SCL = 100 kHz (Standard mode)
 *      		     2. Use internall pull-up resistors for SDA and SCl lines
 *
 */

#include "../drivers/Inc/stm32f407xx.h"

#include <stdio.h>
#define COMMAND_REQUEST			0x51
#define COMMAND_READ_DATA		0x52
#define SLAVE_ADDR		0x68

void GPIO_ButtonInit(GPIO_Handle_t* GPIO_Button);
void GPIO_I2CInit(GPIO_Handle_t* I2C_GPIO);
void I2C_HandlerInit(I2C_Handle_t* I2C_Handler);
void delay();

uint8_t commandCode = 0;
//some data
//Note: data should be less than 32 bytes in one transaction (a.k.a less than 32 characters)
uint8_t Txdata[] = "Testing I2C Slave Tx successfulHIHIHIHIHIHIHIHIHIHIHIHI\n";
I2C_Handle_t  I2C_Handler; //global data for I2C ISR


int main() {



	//Define the handle structures
	GPIO_Handle_t GPIO_Button, I2C_GPIO;
	memset(&GPIO_Button, 0, sizeof(GPIO_Button));
	memset(&I2C_GPIO, 0, sizeof(GPIO_Button));
	memset(&I2C_Handler, 0, sizeof(GPIO_Button));

	GPIO_ButtonInit(&GPIO_Button);
	GPIO_I2CInit(&I2C_GPIO);
	I2C_HandlerInit(&I2C_Handler);


	//Enable the NVIC table for I2C Event an Error Interrupt
	I2C_IRQITConfig(I2C1_EV_IRQ_NO, ENABLE);
	I2C_IRQITConfig(I2C1_ER_IRQ_NO, ENABLE);

	//Enable the interrupt control
	I2C_InterruptCtrl(I2C1, ENABLE);

	//Enable the I2C Peripheral enable
	I2C_PeripheralEnable(I2C_Handler.pI2Cx, ENABLE);

	while(1);
	return EXIT_SUCCESS;
}

//Implement I2C Call back function to signal event completion
void I2C_ApplicationEventCallBack(I2C_Handle_t* pI2CHandler, uint8_t appEvt) {

	//These variables act as global to only within this function
	//due to static keyword.
	//Note: When this function pops off the stack, these variables
	//		still exist since they are stored in the global data region of memory,
	//		not in the stack.
	static uint8_t counter = 0;
	static uint8_t write_byte = 0;

	//If possible, try implementing different application
	//events such as Acknowledgement Error and such
	switch (appEvt) {

	//Slave receives command code from master
	case I2C_EVT_DATA_RCV:	commandCode = I2C_SlaveReceiveData(pI2CHandler->pI2Cx);
						  	break;

	//Master requests data from slave
	case I2C_EVT_DATA_REQ:	if (commandCode ==  COMMAND_REQUEST) {
								I2C_SlaveSendData(I2C1, ((strlen((char*) Txdata)) >> (counter * 8)) & 0xFF);
								counter++;
							} else if (commandCode == COMMAND_READ_DATA) {
								I2C_SlaveSendData(I2C1, Txdata[write_byte++]);
							} else {
								//Possibly terminate the communication
								//or wait until master sends correct command
							}
							break;
	//End I2C Slave Transmission mode
	//Invalidate the commandCode and the counter so that slave no
	//longer sends data bytes to master.
	case I2C_ERR_AF:		if (commandCode == COMMAND_REQUEST) {
								commandCode = 0xff;
							}

							counter = 0;
							if (write_byte >= strlen((char*) Txdata)) {
								write_byte = 0;
								commandCode = 0xff;
							}

	//End I2C Slave Reception mode
	//When the slave receives all the data
	case I2C_EVT_STOPF_CMPLT: counter = 0; break;

	}
}

//Interrupt Service Routine to handle I2C1 Event Interrupt
void I2C1_EV_IRQHandler(void) {
	I2C_EV_IRQHandling(&I2C_Handler);
}

//Interrupt Service Routine to handle I2C1 Error Interrupt
void I2C1_ER_IRQHandler(void) {
	I2C_ER_IRQHandling(&I2C_Handler);
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
	I2C_Handler->I2C_Config.DeviceAddress = SLAVE_ADDR;

	I2C_DeInit(I2C_Handler->pI2Cx);
	I2C_Init(I2C_Handler);
}

void delay() {
	for (uint32_t i; i < 5000000; i++);
}
