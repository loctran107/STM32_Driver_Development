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
I2C_Handle_t  I2C_Handler; //global data for I2C ISR
uint8_t TXE_CMPLT = RESET;
uint8_t RXNE_CMPLT = RESET;
uint8_t rxComplt = RESET;
uint8_t AF_FLAG = RESET;
int main() {

	uint8_t dummy[30]; //dummy buffer
	uint8_t len, commandcode;
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

	//Enable the I2C Peripheral enable
	I2C_PeripheralEnable(I2C_Handler.pI2Cx, ENABLE);


	while (1) {
		//wait till button is pressed
		while( ! GPIO_ReadFromInputPin(GPIOA,GPIO_PIN_0) );

		//to avoid button de-bouncing related issues 200ms of delay
		delay();

		commandcode = COMMAND_REQUEST;


		while(I2C_MasterSendDataIT(&I2C_Handler,&commandcode,1,SLAVE_ADDR,I2C_SR_SET) != I2C_READY);

		while(I2C_MasterReceiveDataIT(&I2C_Handler,&len,1,SLAVE_ADDR,I2C_SR_SET)!= I2C_READY);



		commandcode = 0x52;
		while(I2C_MasterSendDataIT(&I2C_Handler,&commandcode,1,SLAVE_ADDR,I2C_SR_SET) != I2C_READY);


		while(I2C_MasterReceiveDataIT(&I2C_Handler,dummy,len,SLAVE_ADDR,I2C_SR_RESET)!= I2C_READY);

		rxComplt = RESET;

		//wait till rx completes
		while(rxComplt != SET)
		{

		}

		dummy[len+1] = '\0';

		printf("Data : %s",dummy);

		rxComplt = RESET;

	}

	return EXIT_SUCCESS;
}

//Implement I2C Call back function to signal event completion
void I2C_ApplicationEventCallBack(I2C_Handle_t* pI2CHandler, uint8_t appEvt) {

	//If possible, try implementing different application
	//events such as Acknowledgement Error and such
	switch (appEvt) {
	case I2C_EVT_TX_CMPLT:	TXE_CMPLT = SET;  break;
	case I2C_EVT_RX_CMPLT: RXNE_CMPLT = SET; rxComplt = SET; break;
	case I2C_ERR_AF: AF_FLAG = SET; break;
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
	I2C_Handler->I2C_Config.DeviceAddress = MY_ADDR;

	I2C_DeInit(I2C_Handler->pI2Cx);
	I2C_Init(I2C_Handler);
}

void delay() {
	for (uint32_t i; i < 5000000; i++);
}
