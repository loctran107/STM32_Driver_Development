/*
 * 006TestSpi.c
 *
 *  Created on: Aug 28, 2020
 *      Author: Donavan Tran
 *      Description: Test the SPI_SendData PI to send the string "Hello World"
 *      			 and use the below configurations
 *      			 1. SPI-2 Master Mode
 *      			 2. SCLK = max possible
 *      			 3. DFF = 0 and DFF = 1
 *
 *      			 To test the code, connect to the logic analyzer
 */

/*
 * PB14 -> SPI2_MISO
 * PB15 -> SPI2_MOSI
 * PB13 -> SPI2_SCK
 * PB12 -> SPI2_NSS
 * ALT function mode : 5
 */
#include "../drivers/Inc/stm32f407xx.h"

void SPI2_Handler_Setup(SPI_Handle_t* SPI_Handler);
void GPIO_Handler_Setup(GPIO_Handle_t* GPIO_Handler);

int main() {

	//SPI configuration and GPIO pin configuration
	SPI_Handle_t SPI_Handler;
	GPIO_Handle_t GPIO_Handler;
	memset(&SPI_Handler, 0, sizeof(SPI_Handler));
	memset(&GPIO_Handler, 0, sizeof(GPIO_Handler));

	SPI2_Handler_Setup(&SPI_Handler);
	GPIO_Handler_Setup(&GPIO_Handler);

	//Enable the SPI2 Peripherals
	SPI_PeripheralEnable(SPI2, ENABLE);

	char* str = "Hello World";
	SPI_SendData(SPI_Handler.pSPIx, (uint8_t*) str, strlen(str));

	//Wait until the Master is done transferring the bytes of data
	//If busy, stay there. Otherwise, disable the peripheral
	while (SPI_CheckStatusFlag(SPI_Master.pSPIx, SPI_BUSY_FLAG));

	//Disable the peripherals after the str transmission
	SPI_PeripheralEnable(SPI2, DISABLE);

	while (1);
	return 0;
}

void SPI2_Handler_Setup(SPI_Handle_t* SPI_Handler) {

	//Configure the SPI port
	SPI_Handler->pSPIx = SPI2;
	SPI_Handler->SPI_Config.DeviceMode = SPI_DEVICE_MASTER_MODE;
	SPI_Handler->SPI_Config.BusConfig = SPI_BUS_CONFIG_FULL_DUPLX;
	SPI_Handler->SPI_Config.SclkSpeed = SPI_SCLK_SPEED_DIV2; //8MHz since we're using 16MHz HSI
	SPI_Handler->SPI_Config.DFF = SPI_DFF_8_BIT;
	SPI_Handler->SPI_Config.CPOLConfig = SPI_CPOL_LOW;
	SPI_Handler->SPI_Config.CPHAConfig = SPI_CPHA_LOW;
	SPI_Handler->SPI_Config.SSM = SPI_SSM; //software slave management enabled for NSS pin
	SPI_Init(SPI_Handler);
}

void GPIO_Handler_Setup(GPIO_Handle_t* GPIO_Handler) {

	//Configure the GPIO pin for SPI application
	GPIO_Handler->pGPIOx = GPIOB;
	GPIO_Handler->GPIOx_PinConfig.GPIO_PinNumber = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_15;
	GPIO_Handler->GPIOx_PinConfig.GPIO_PinMode = GPIO_ALT_FUNC_MODE;
	GPIO_Handler->GPIOx_PinConfig.GPIO_PinAltFuncMode = AF5;
	GPIO_Handler->GPIOx_PinConfig.GPIO_PinOPType = GPIO_PUSH_PULL;
	GPIO_Handler->GPIOx_PinConfig.GPIO_PinPuPdCtrl = GPIO_NO_PU_PD;
	GPIO_Handler->GPIOx_PinConfig.GPIO_PinSpeed = GPIO_HIGH_SPEED;
	GPIO_Init(GPIO_Handler);


}
