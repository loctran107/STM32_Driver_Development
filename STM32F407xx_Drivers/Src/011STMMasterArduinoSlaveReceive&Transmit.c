/*
 * 010SPIMasterArduinoSlaveOnBoardButton.c
 *
 *  Created on: Aug 30, 2020
 *      Author: Donavan Tran
 *      Description: SPI Master (STM) and SPI Slave (Arduino) communication
 *      		  	 When the button on the master is pressed, master sends a command to the slave
 *      		  	 and slave responds as per the command implementation
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
#include <stdio.h>
//Command codes
#define COMMAND_LED_CTRL			0x50
#define COMMAND_SENSOR_READ			0x51
#define COMMAND_LED_READ			0x52
#define COMMAND_PRINT				0x53
#define COMMAND_ID_READ				0x54

#define LED_ON						1
#define LED_OFF						0

//Arduino analog pins
#define ANALOG_PIN0					0
#define ANALOG_PIN1					1
#define ANALOG_PIN2					2
#define ANALOG_PIN3					3
#define ANALOG_PIN4					4
#define ANALOG_PIN5					5

//Define LED pin
#define LED_PIN						1

void ButtonEnable(GPIO_Handle_t* GPIO_Button);
void SPI_GPIOInit(GPIO_Handle_t* SPI_GPIO);
void SPI_MasterInit(SPI_Handle_t* SPI_Master);
uint8_t SPI_VerifyResponse(uint8_t ackbyte);
void delay();

int main(void) {

	//Define dummy string to send to SPI Slave (Arduino)
	//char str[] = "Sacrilegious: committing or characterized by sacrilege: having or showing a lack of respect";

	//Dummy bytes
	uint8_t	dummy_write = 0xEE;
	uint8_t dummy_read;
	//uint8_t args[2];
	//Define handle structure for GPIO button, GPIO for SPI, and SPI1
	GPIO_Handle_t GPIO_Button, SPI_GPIO;
	SPI_Handle_t SPI_Master;

	//Set everything to 0 by default
	memset(&GPIO_Button, 0, sizeof(GPIO_Button));
	memset(&SPI_GPIO, 0, sizeof(SPI_GPIO));
	memset(&SPI_Master, 0, sizeof(SPI_Master));

	//Initialize GPIO button
	ButtonEnable(&GPIO_Button);
	SPI_GPIOInit(&SPI_GPIO);
	SPI_MasterInit(&SPI_Master);


	while (1) {

		/*//When the button is pressed, SPI Master (ST) sends str data to SPI Slave (Arduino)
		if (GPIO_ReadFromInputPin(GPIO_Button.pGPIOx, GPIO_PIN_0)) {
			delay(); //wait until the debouncing is over ~200ms

			//Enable the SPI peripheral
			SPI_PeripheralEnable(SPI2, ENABLE);

			uint8_t commandcode, ackbyte;
			//1. CMD_LED_CTRL		<pin no (1)>	<value(1)>
			uint8_t commandcde = COMMAND_SENSOR_READ;
			uint8_t ackbyte;

			SPI_SendData(SPI_Master.pSPIx, &commandcde, 1);

			//Do dummy read here since in SPI communication, if Master or Slave sends 1 byte,
			//it also receive 1 byte in the RxBuffer in return. Thus, RXNE will be set, which
			//may cause reading error later if not clear this read correctly.
			//Thus, do the dummy read right after the command code is sent over and clear the flag
			SPI_ReceiveData(SPI_Master.pSPIx, &dummy_read, 1);

			//send some dummy bits (1 byte) to fetch the response from the slave
			SPI_SendData(SPI_Master.pSPIx, &dummy_write, 1);
			SPI_ReceiveData(SPI_Master.pSPIx, &ackbyte, 1);

			if (SPI_VerifyResponse(ackbyte)) {
				uint8_t analog = ANALOG_PIN2;

				SPI_SendData(SPI_Master.pSPIx, &analog, 1);

				SPI_ReceiveData(SPI_Master.pSPIx, &dummy_read, 1);

				delay();
				SPI_SendData(SPI_Master.pSPIx, &dummy_write, 1);

				uint8_t analog_read;
				SPI_ReceiveData(SPI_Master.pSPIx, &analog_read, 1);
			}
			commandcode = COMMAND_ID_READ;

			//send command
			SPI_SendData(SPI_Master.pSPIx,&commandcode,1);

			//do dummy read to clear off the RXNE
			SPI_ReceiveData(SPI_Master.pSPIx,&dummy_read,1);

			//Send some dummy byte to fetch the response from the slave
			SPI_SendData(SPI_Master.pSPIx,&dummy_write,1);

			//read the ack byte received
			SPI_ReceiveData(SPI_Master.pSPIx,&ackbyte,1);

			uint8_t id[11];
			uint32_t i=0;
			if( SPI_VerifyResponse(ackbyte))
			{
				//read 10 bytes id from the slave
				for(  i = 0 ; i < 10 ; i++)
				{
					//send dummy byte to fetch data from slave
					SPI_SendData(SPI_Master.pSPIx,&dummy_write,1);
					SPI_ReceiveData(SPI_Master.pSPIx,&id[i],1);
				}

				id[10] = '\0';

				printf("COMMAND_ID : %s \n",id);

			}*/

		//wait till button is pressed
				while( ! GPIO_ReadFromInputPin(GPIOA,GPIO_PIN_0) );

				//to avoid button de-bouncing related issues 200ms of delay
				delay();

				//enable the SPI2 peripheral
				SPI_PeripheralEnable(SPI2,ENABLE);

			    //1. CMD_LED_CTRL  	<pin no(1)>     <value(1)>

				uint8_t commandcode = COMMAND_LED_CTRL;
				uint8_t ackbyte;
				uint8_t args[2];

				//send command
				SPI_SendData(SPI2,&commandcode,1);

				//do dummy read to clear off the RXNE
				SPI_ReceiveData(SPI2,&dummy_read,1);

				//Send some dummy bits (1 byte) fetch the response from the slave
				SPI_SendData(SPI2,&dummy_write,1);

				//read the ack byte received
				SPI_ReceiveData(SPI2,&ackbyte,1);

				if( SPI_VerifyResponse(ackbyte))
				{
					args[0] = LED_PIN;
					args[1] = LED_ON;

					//send arguments
					SPI_SendData(SPI2,args,2);
					printf("COMMAND_LED_CTRL Executed\n");
				}
				//end of COMMAND_LED_CTRL

				//2. CMD_SENOSR_READ   <analog pin number(1) >

				//wait till button is pressed
				while( ! GPIO_ReadFromInputPin(GPIOA,GPIO_PIN_0) );

				//to avoid button de-bouncing related issues 200ms of delay
				delay();

				commandcode = COMMAND_SENSOR_READ;

				//send command
				SPI_SendData(SPI2,&commandcode,1);

				//do dummy read to clear off the RXNE
				SPI_ReceiveData(SPI2,&dummy_read,1);

				//Send some dummy byte to fetch the response from the slave
				SPI_SendData(SPI2,&dummy_write,1);

				//read the ack byte received
				SPI_ReceiveData(SPI2,&ackbyte,1);

				if( SPI_VerifyResponse(ackbyte))
				{
					args[0] = ANALOG_PIN0;

					//send arguments
					SPI_SendData(SPI2,args,1); //sending one byte of

					//do dummy read to clear off the RXNE
					SPI_ReceiveData(SPI2,&dummy_read,1);

					//insert some delay so that slave can ready with the data
					delay();

					//Send some dummy bits (1 byte) fetch the response from the slave
					SPI_SendData(SPI2,&dummy_write,1);

					uint8_t analog_read;
					SPI_ReceiveData(SPI2,&analog_read,1);
					printf("COMMAND_SENSOR_READ %d\n",analog_read);
				}

				//3.  CMD_LED_READ 	 <pin no(1) >

				//wait till button is pressed
				while( ! GPIO_ReadFromInputPin(GPIOA,GPIO_PIN_0) );

				//to avoid button de-bouncing related issues 200ms of delay
				delay();

				commandcode = COMMAND_LED_READ;

				//send command
				SPI_SendData(SPI2,&commandcode,1);

				//do dummy read to clear off the RXNE
				SPI_ReceiveData(SPI2,&dummy_read,1);

				//Send some dummy byte to fetch the response from the slave
				SPI_SendData(SPI2,&dummy_write,1);

				//read the ack byte received
				SPI_ReceiveData(SPI2,&ackbyte,1);

				if( SPI_VerifyResponse(ackbyte))
				{
					args[0] = LED_PIN;

					//send arguments
					SPI_SendData(SPI2,args,1); //sending one byte of

					//do dummy read to clear off the RXNE
					SPI_ReceiveData(SPI2,&dummy_read,1);

					//insert some delay so that slave can ready with the data
					delay();

					//Send some dummy bits (1 byte) fetch the response from the slave
					SPI_SendData(SPI2,&dummy_write,1);

					uint8_t led_status;
					SPI_ReceiveData(SPI2,&led_status,1);
					printf("COMMAND_READ_LED %d\n",led_status);

				}

				//4. CMD_PRINT 		<len(2)>  <message(len) >

				//wait till button is pressed
				while( ! GPIO_ReadFromInputPin(GPIOA,GPIO_PIN_0) );

				//to avoid button de-bouncing related issues 200ms of delay
				delay();

				commandcode = COMMAND_PRINT;

				//send command
				SPI_SendData(SPI2,&commandcode,1);

				//do dummy read to clear off the RXNE
				SPI_ReceiveData(SPI2,&dummy_read,1);

				//Send some dummy byte to fetch the response from the slave
				SPI_SendData(SPI2,&dummy_write,1);

				//read the ack byte received
				SPI_ReceiveData(SPI2,&ackbyte,1);

				uint8_t message[] = "Hello ! How are you ??";
				if( SPI_VerifyResponse(ackbyte))
				{
					args[0] = strlen((char*)message);

					//send arguments
					SPI_SendData(SPI2,args,1); //sending length

					//send message
					SPI_SendData(SPI2,message,args[0]);

					printf("COMMAND_PRINT Executed \n");

				}

				//5. CMD_ID_READ
				//wait till button is pressed
				while( ! GPIO_ReadFromInputPin(GPIOA,GPIO_PIN_0) );

				//to avoid button de-bouncing related issues 200ms of delay
				delay();

				commandcode = COMMAND_ID_READ;

				//send command
				SPI_SendData(SPI2,&commandcode,1);

				//do dummy read to clear off the RXNE
				SPI_ReceiveData(SPI2,&dummy_read,1);

				//Send some dummy byte to fetch the response from the slave
				SPI_SendData(SPI2,&dummy_write,1);

				//read the ack byte received
				SPI_ReceiveData(SPI2,&ackbyte,1);

				uint8_t id[11];
				uint32_t i=0;
				if( SPI_VerifyResponse(ackbyte))
				{
					//read 10 bytes id from the slave
					for(  i = 0 ; i < 10 ; i++)
					{
						//send dummy byte to fetch data from slave
						SPI_SendData(SPI2,&dummy_write,1);
						SPI_ReceiveData(SPI2,&id[i],1);
					}

					id[11] = '\0';

					printf("COMMAND_ID : %s \n",id);





			//Wait until the Master is done transferring the bytes of data
			//If busy, stay there. Otherwise, disable the peripheral
			while (SPI_CheckStatusFlag(SPI_Master.pSPIx, SPI_BUSY_FLAG));

			//Disable the SPI peripheral
			SPI_PeripheralEnable(SPI2, DISABLE);
		}

	}

	return EXIT_SUCCESS;
}

void ButtonEnable(GPIO_Handle_t* GPIO_Button) {
	//Enable the Button
	//GPIO_Handle_t GPIO_Button;
	GPIO_Button->pGPIOx = GPIOA;
	GPIO_Button->GPIOx_PinConfig.GPIO_PinNumber = GPIO_PIN_0;
	GPIO_Button->GPIOx_PinConfig.GPIO_PinPuPdCtrl = GPIO_NO_PU_PD;
	GPIO_Button->GPIOx_PinConfig.GPIO_PinMode = GPIO_INPUT_MODE;
	GPIO_Button->GPIOx_PinConfig.GPIO_PinSpeed = GPIO_VERY_HIGH_SPEED;
	GPIO_Init(GPIO_Button);
}

void SPI_GPIOInit(GPIO_Handle_t* SPI_GPIO) {

	//According to the alternate function map table
	//PB12 --> SPI1_NSS
	//PB13 --> SPI1_SCK
	//PB14 --> SPI1_MISO
	//PB15 --> SPI1_MOSI
	SPI_GPIO->pGPIOx = GPIOB;
	SPI_GPIO->GPIOx_PinConfig.GPIO_PinMode = GPIO_ALT_FUNC_MODE;
	SPI_GPIO->GPIOx_PinConfig.GPIO_PinAltFuncMode = AF5;
	SPI_GPIO->GPIOx_PinConfig.GPIO_PinNumber = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
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

uint8_t SPI_VerifyResponse(uint8_t ackbyte) {
	if (ackbyte == 0xF5) { //ACK
		return 1;
	}
	return 0;
}
void delay() {
	for (uint32_t i = 0; i < 500000 / 2; i++); //~200 ms
}





