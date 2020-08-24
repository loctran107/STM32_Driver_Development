/**
  ******************************************************************************
  * @file    main.c
  * @author  Auto-generated by STM32CubeIDE
  * @version V1.0
  * @brief   Default main function.
  ******************************************************************************
*/
#include "header.h"
int main(void)
{
	//Configure the clock of RCC_CFGR to enable MCO1 peripheral
	uint32_t *pMCO1_Clock = (uint32_t*) RCC_CFGR;
	*pMCO1_Clock &= ~(0x3 << 21); //clear bit 21 and 22

	//Enable the clock for GPIOA
	uint32_t *pAHB1_Clock = (uint32_t*) RCC_AHB1ENR;
	*pAHB1_Clock |= (1 << 0);

	//Set the prescalar configuration
	*pMCO1_Clock |= (0x4 << 24); //divide by 2

	/*--------------------------*/
	//Choose the moder for GPIOA port 8 (PA8)
	uint32_t *pGPIOA_MODER = (uint32_t*) GPIOA_MODER;
	*pGPIOA_MODER &= ~(0x3 << 16); //clear bit 16 and 17
	*pGPIOA_MODER |= (0x2 << 16); //set bit 16 and 17 to alternate function mode

	//Choose the alterate function high for PA8
	uint32_t *pGPIOA_AFRH = (uint32_t*) GPIOA_AFRH;
	*pGPIOA_AFRH &= ~(0xf << 0); //Choose AF0 alternate function mode

	for(;;);



}
