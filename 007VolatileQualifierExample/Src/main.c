/**
  ******************************************************************************
  * @file    main.c
  * @author  Auto-generated by STM32CubeIDE
  * @version V1.0
  * @brief   Default main function.
  ******************************************************************************
*/

#include <stdint.h>
#include <stdlib.h>
#define SRAM1_ADDR 		(*(volatile unsigned long*)0x20000004)

int main(void)
{
	uint32_t volatile value = 0;
	while (1) {
		value = SRAM1_ADDR;
		if (value) {
			break;
		}
	}

	for(;;);

	return EXIT_SUCCESS;

}