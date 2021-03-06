/*
 * stm32f407xx.h
 *
 *  Created on: Aug 17, 2020
 *      Author: Donavan Tran
 *      Description: This header file contains specific details related
 *      			 to the STM32F407xx microcontroller architecture
 */

#ifndef INC_STM32F407XX_H_
#define INC_STM32F407XX_H_
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <math.h>

/*
 * Some generic macros
 */
#define __vo   			volatile //making a shortcut for volatile
#define __weak          __attribute__((weak))
#define ENABLE 			1
#define DISABLE			0
#define SET				ENABLE
#define RESET			DISABLE
#define FLAG_SET		SET
#define FLAG_RESET		RESET

/*******************************************PROCESSOR SPECIFIC DETAILS*******************************************/
/*
 * @NVIC_IRQ_PRIORITY macros
 */
#define NVIC_IRQ_PR1		1U
#define NVIC_IRQ_PR2		2U
#define NVIC_IRQ_PR3		3U
#define NVIC_IRQ_PR4		4U
#define NVIC_IRQ_PR5		5U
#define NVIC_IRQ_PR6		6U
#define NVIC_IRQ_PR7		7U
#define NVIC_IRQ_PR8		8U
#define NVIC_IRQ_PR9		9U
#define NVIC_IRQ_PR10		10U
#define NVIC_IRQ_PR11		11U
#define NVIC_IRQ_PR12		12U
#define NVIC_IRQ_PR13		13U
#define NVIC_IRQ_PR14		14U
#define NVIC_IRQ_PR15		15U

/*
 * EXTI IRQ (Interrupt Request) Number of STM32F407xx MCU
 *  Note: Update these macros in accordance to MCU specific EXTI pins
 */
#define EXTI0_IRQ_NO		((uint8_t) 6)
#define EXTI1_IRQ_NO		((uint8_t) 7)
#define EXTI2_IRQ_NO		((uint8_t) 8)
#define EXTI3_IRQ_NO		((uint8_t) 9)
#define EXTI4_IRQ_NO		((uint8_t) 10)
#define EXTI5_9_IRQ_NO		((uint8_t) 23)
#define EXTI10_15_IRQ_NO	((uint8_t) 40)

/*
 * SPI IRQ Number of STM32F407xx MCU
 * Note: Look at the datasheet NVIC table
 */
#define SPI1_IRQ_NO			((uint8_t) 35)
#define SPI2_IRQ_NO			((uint8_t) 36)
#define SPI3_IRQ_NO			((uint8_t) 51)

/*
 * I2C IRQ Number of STM32F407xx MCU
 */
#define I2C1_EV_IRQ_NO		((uint8_t) 31)
#define I2C1_ER_IRQ_NO		((uint8_t) 32)
#define I2C2_EV_IRQ_NO		((uint8_t) 33)
#define I2C2_ER_IRQ_NO		((uint8_t) 34)
#define I2C3_EV_IRQ_NO		((uint8_t) 72)
#define I2C3_ER_IRQ_NO		((uint8_t) 73)

/*
 * USART IRQ Number of STM32F407xx MCU
 */
#define USART1_IRQ_NO		((uint8_t) 37)
#define USART2_IRQ_NO		((uint8_t) 38)
#define USART3_IRQ_NO		((uint8_t) 39)
#define UART4_IRQ_NO		((uint8_t) 52)
#define UART5_IRQ_NO		((uint8_t) 53)
#define USART6_IRQ_NO		((uint8_t) 71)

/*
 * ARM Cortex Mx Processor NVIC Interrupt Set-Enable Register (ISER) base address
 */
#define NVIC_ISER_BASEADDR	 (__vo uint32_t*) 0xE000E100
#define NVIC_ISER(__INDEX__) *((NVIC_ISER_BASEADDR) + ((__INDEX__))) //Pointer arithmetic

/*
 * ARM Cortex Mx Processor NVIC Interrupt Clear-Enable Register (ICER) base address
 */
#define NVIC_ICER_BASEADDR	 (__vo uint32_t*) 0xE000E180
#define NVIC_ICER(__INDEX__) *((NVIC_ICER_BASEADDR) + ((__INDEX__))) //pointer arithmetic

/*
 * ARM Cortex Mx Processor NVIC Interrupt Priority Register (IPR) base address
 */
#define NVIC_IPR_BASEADDR	(__vo uint32_t *) 0xE000E400
#define NVIC_IPR(__INDEX__) *((NVIC_IPR_BASEADDR) + (__INDEX__)) //Pointer arithmetic


/*
 * ARM Cortex Mx Processor NVIC Interrupt Priority Level Bit
 * Note: There are 0-255 values of interrupt priority -> 8 bits
 * 		 But, the processor stores them as 16 PRIORITY LEVELs -> 4 bits
 */
#define NO_IMPLEMENTED_IRQ_PRIORITY_BIT 			4U
#define IMPLEMENTED_IRQ_PRIORITY_BIT				8U - NO_IMPLEMENTED_IRQ_PRIORITY_BIT

/*
 * STM32F407xx System Clock
 */
#define RCC_HSI					0U
#define RCC_HSE					1U
#define RCC_PLL					2U

/*
 * HSE and HSI Clock Frequency
 */
#define HSE_CLK_FREQ			8000000U
#define HSI_CLK_FREQ			16000000U


/*
 * Flash and SRAM memories
 */
#define FLASH_BASEADDR			0x08000000UL
#define ROM_BASEADDR			0x1FFF0000UL
#define SRAM1_BASEADDR			0x20000000UL
#define SRAM2_BASEADDR			0x2001C000UL
#define SRAM					SRAM1_BASEADDR //SRAM1 is commonly used

/*
 * Bus clock AHBx and APBx peripheral base addresses
 */
#define PERIPH_BASEADDR			0x40000000UL
#define APB1_BASEADDR			PERIPH_BASEADDR
#define APB2_BASEADDR			0x40010000UL
#define AHB1_BASEADDR			0x40020000UL
#define AHB2_BASEADDR			0x50000000UL

/*
 * Base addresses of all peripherals that are hanging on AHB1 bus
 */
#define GPIOA_BASEADDR			(AHB1_BASEADDR + 0x0000)
#define GPIOB_BASEADDR			(AHB1_BASEADDR + 0x0400)
#define GPIOC_BASEADDR			(AHB1_BASEADDR + 0x0800)
#define GPIOD_BASEADDR			(AHB1_BASEADDR + 0x0C00)
#define GPIOE_BASEADDR			(AHB1_BASEADDR + 0x0100)
#define GPIOF_BASEADDR			(AHB1_BASEADDR + 0x1400)
#define GPIOG_BASEADDR			(AHB1_BASEADDR + 0x1800)
#define GPIOH_BASEADDR			(AHB1_BASEADDR + 0x1C00)
#define GPIOI_BASEADDR			(AHB1_BASEADDR + 0x2000)
#define GPIOJ_BASEADDR			(AHB1_BASEADDR + 0x2400)
#define GPIOK_BASEADDR			(AHB1_BASEADDR + 0x2800)
#define RCC_BASEADDR			(AHB1_BASEADDR + 0x3800)

/*
 * Base addresses of peripherals that are hanging to APB1 bus
 */
#define I2C1_BASEADDR			(APB1_BASEADDR + 0x5400)
#define I2C2_BASEADDR			(APB1_BASEADDR + 0x5800)
#define I2C3_BASEADDR			(APB1_BASEADDR + 0x5C00)
#define SPI2_BASEADDR			(APB1_BASEADDR + 0x3800)
#define SPI3_BASEADDR			(APB1_BASEADDR + 0x3C00)
#define USART2_BASEADDR			(APB1_BASEADDR + 0x4400)
#define USART3_BASEADDR			(APB1_BASEADDR + 0x4800)
#define UART4_BASEADDR			(APB1_BASEADDR + 0x4C00)
#define UART5_BASEADDR			(APB1_BASEADDR + 0x5000)

/*
 * Base addresses of peripherals that are hanging to APB2 bus
 */
#define SPI1_BASEADDR			(APB2_BASEADDR + 0x3000)
#define USART1_BASEADDR			(APB2_BASEADDR + 0x1000)
#define USART6_BASEADDR			(APB2_BASEADDR + 0x1400)
#define EXTI_BASEADDR			(APB2_BASEADDR + 0x3C00)
#define SYSCFG_BASEADDR			(APB2_BASEADDR + 0x3800)

/*
 * SYSCFG registers definition
 */
typedef struct SYSCFG_Register {
	__vo uint32_t MEMRMP; 		//offset: 0x00
	__vo uint32_t PMC;			//offset: 0x04
	__vo uint32_t EXTICR[4];	//offset: 0x08 - 0x14
	uint32_t RESERVED[2];		//reserved: 0x18 - 0x1C
	__vo uint32_t CMPCR;		//offset: 0x20
} SYSCFG_Reg_t;
/*
 * EXTI registers
 */
typedef struct EXTI_Register {
	__vo uint32_t IMR;   //offset: 0x00
	__vo uint32_t EMR;   //offset: 0x04
	__vo uint32_t RTSR;  //offset: 0x08
	__vo uint32_t FTSR;  //offset: 0x0C
	__vo uint32_t SWIER; //offset: 0x10
	__vo uint32_t PR; 	 //offset: 0x14
} EXTI_Reg_t;
/*
 * GPIOx peripheral registers declaration
 */
typedef struct GPIO_Register {
	__vo uint32_t MODER;   //offset: 0x00
	__vo uint32_t OTYPER;  //offset: 0x04
	__vo uint32_t OSPEEDR; //offset: 0x08
    __vo uint32_t PUPDR;   //offset: 0x0C
	__vo uint32_t IDR;     //offset: 0x10
	__vo uint32_t ODR;     //offset: 0x14
	__vo uint32_t BSRR;    //offset: 0x18
	__vo uint32_t LCKR;    //offset: 0x1C
	__vo uint32_t AFR[2];  //offset: 0x20(AFRL - 0x24(AFRH)
} GPIO_Reg_t;

/*
 * RCC peripheral registers declaration
 */
typedef struct RCC_Register {
	__vo uint32_t CR;         //offset: 0x00
	__vo uint32_t PLLCFGR;    //offset: 0x04
	__vo uint32_t CFGR;       //offset: 0x08
	__vo uint32_t CIR;        //offset: 0x0C
	__vo uint32_t AHB1RSTR;   //offset: 0x10
	__vo uint32_t AHB2RSTR;   //offset: 0x14
	__vo uint32_t AHB3RSTR;   //offset: 0x18
	uint32_t RESERVED0;       //reserved at 0x1C
	__vo uint32_t APB1RSTR;   //offset: 0x20
	__vo uint32_t APB2RSTR;   //offset: 0x24
	uint32_t RESERVED1[2];	  //reserved at 0x28-0x2C
	__vo uint32_t AHB1ENR;    //offset: 0x30
	__vo uint32_t AHB2ENR;    //offset: 0x34
	__vo uint32_t AHB3ENR;    //offset: 0x38
	uint32_t RESERVED2;		  //reserved at 0x3C
	__vo uint32_t APB1ENR;    //offset: 0x40
	__vo uint32_t APB2ENR;    //offset: 0x44
	uint32_t RESERVED3[2];    //reserved at 0x48-0x4C
	__vo uint32_t AHB1LPENR;  //offset: 0x50
	__vo uint32_t AHB2LPENR;  //offset: 0x54
	__vo uint32_t AHB3LPENR;  //offset: 0x58
	uint32_t RESERVED4;		  //reserved at 0x5C
	__vo uint32_t APB1LPENR;  //offset: 0x60
	__vo uint32_t APB2LPENR;  //offset: 0x64
	uint32_t RESERVED5[2];    //reserved at 0x68-0x6C
	__vo uint32_t BDCR; 	  //offset: 0x70
	__vo uint32_t CSR; 		  //offset: 0x74
	uint32_t RESERVED6[2];    //reserved at 0x78-0x7C
	__vo uint32_t SSCGR;      //offset: 0x80
	__vo uint32_t PLLI2SCFGR; //offset: 0x84
	__vo uint32_t PLLSAICFGR; //offset: 0x88
	__vo uint32_t DCKCFGR; 	  //offset: 0x8C
} RCC_Reg_t;

/******************************************COMMUNICATION PROTOCOL STRUCTURE******************************************/
/*
 * SPI register definition
 */
typedef struct SPI_Register {
	__vo uint32_t CR1;		//offset: 0x00
	__vo uint32_t CR2; 		//offset: 0x04
	__vo uint32_t SR;		//offset: 0x08
	__vo uint32_t DR;		//offset: 0x0C
	__vo uint32_t CRCPR;	//offset: 0x10
	__vo uint32_t RXCRCR;	//offset: 0x14
	__vo uint32_t TXCRCR;	//offset: 0x18
	__vo uint32_t I2SCFGR;	//offset: 0x1C
	__vo uint32_t I2SPR;	//offset: 0x20
} SPI_Reg_t;

/*
 * I2C register definition
 */
typedef struct I2C_Register {
	__vo uint32_t CR1;		//offset: 0x00
	__vo uint32_t CR2;		//offset: 0x04
	__vo uint32_t OAR1; 	//offset: 0x08
	__vo uint32_t OAR2; 	//offset: 0x0C
	__vo uint32_t DR;		//offset: 0x10
	__vo uint32_t SR1;		//offset: 0x14
	__vo uint32_t SR2;		//offset: 0x18
	__vo uint32_t CCR;		//offset: 0x1C
	__vo uint32_t TRISE;	//offset: 0x20
	__vo uint32_t FLTR;		//offset: 0x24
} I2C_Reg_t;

/*
 * USART register definition
 */
typedef struct USART_Register {
	__vo uint32_t SR;		//offset: 0x00
	__vo uint32_t DR;		//offset: 0x04
	__vo uint32_t BRR;		//offset: 0x08
	__vo uint32_t CR1;		//offset: 0x0C
	__vo uint32_t CR2;		//offset: 0x10
	__vo uint32_t CR3;		//offset: 0x14
	__vo uint32_t GTPR;		//offset: 0x18
} USART_Reg_t;
/********************************************************************************************************************/

/*
 * GPIO peripherals definition
 */
#define GPIOA			((GPIO_Reg_t*) GPIOA_BASEADDR)
#define GPIOB			((GPIO_Reg_t*) GPIOB_BASEADDR)
#define GPIOC			((GPIO_Reg_t*) GPIOC_BASEADDR)
#define GPIOD			((GPIO_Reg_t*) GPIOD_BASEADDR)
#define GPIOE			((GPIO_Reg_t*) GPIOE_BASEADDR)
#define GPIOF			((GPIO_Reg_t*) GPIOF_BASEADDR)
#define GPIOG			((GPIO_Reg_t*) GPIOG_BASEADDR)
#define GPIOH			((GPIO_Reg_t*) GPIOH_BASEADDR)
#define GPIOI			((GPIO_Reg_t*) GPIOI_BASEADDR)
#define GPIOJ			((GPIO_Reg_t*) GPIOJ_BASEADDR)
#define GPIOK			((GPIO_Reg_t*) GPIOK_BASEADDR)

/*
 * RCC clock bus definition
 */
#define RCC				((RCC_Reg_t*) RCC_BASEADDR)

/*
 * EXTI definition
 */
#define EXTI			((EXTI_Reg_t*) EXTI_BASEADDR)

/*
 * SYSCFG definition
 */
#define SYSCFG			((SYSCFG_Reg_t*) SYSCFG_BASEADDR)

/*
 * SPI Definition
 */
#define SPI1			((SPI_Reg_t*) SPI1_BASEADDR)
#define SPI2			((SPI_Reg_t*) SPI2_BASEADDR)
#define SPI3			((SPI_Reg_t*) SPI3_BASEADDR)

/*
 * I2C Peripherals macro
 */
#define I2C1			((I2C_Reg_t*) I2C1_BASEADDR)
#define I2C2			((I2C_Reg_t*) I2C2_BASEADDR)
#define I2C3			((I2C_Reg_t*) I2C3_BASEADDR)

/*
 * USART/UART peripheral macros
 */
#define USART1			((USART_Reg_t*) USART1_BASEADDR)
#define USART2			((USART_Reg_t*) USART2_BASEADDR)
#define USART3			((USART_Reg_t*) USART3_BASEADDR)
#define UART4			((USART_Reg_t*) UART4_BASEADDR)
#define UART5			((USART_Reg_t*) UART5_BASEADDR)
#define USART6			((USART_Reg_t*) USART6_BASEADDR)
/**********************************PERIPHERAL CLOCK ENABLE********************************/
/*
 * Clock enable for GPIOx
 */
#define GPIOA_PCLK_EN() (RCC->AHB1ENR |= (1 << 0))
#define GPIOB_PCLK_EN() (RCC->AHB1ENR |= (1 << 1))
#define GPIOC_PCLK_EN() (RCC->AHB1ENR |= (1 << 2))
#define GPIOD_PCLK_EN() (RCC->AHB1ENR |= (1 << 3))
#define GPIOE_PCLK_EN() (RCC->AHB1ENR |= (1 << 4))
#define GPIOF_PCLK_EN() (RCC->AHB1ENR |= (1 << 5))
#define GPIOG_PCLK_EN() (RCC->AHB1ENR |= (1 << 6))
#define GPIOH_PCLK_EN() (RCC->AHB1ENR |= (1 << 7))
#define GPIOI_PCLK_EN() (RCC->AHB1ENR |= (1 << 8))
#define GPIOJ_PCLK_EN() (RCC->AHB1ENR |= (1 << 9))
#define GPIOK_PCLK_EN() (RCC->AHB1ENR |= (1 << 10))

/*
 * Clock enable for SPIx
 */
#define SPI1_PCLK_EN()	(RCC->APB2ENR |= (1 << 12))
#define SPI2_PCLK_EN()	(RCC->APB1ENR |= (1 << 14))
#define SPI3_PCLK_EN()	(RCC->APB1ENR |= (1 << 15))


/*
 * Clock enable for I2Cx
 */
#define I2C1_PCLK_EN()	(RCC->APB1ENR |= (1 << 21))
#define I2C2_PCLK_EN()	(RCC->APB1ENR |= (1 << 22))
#define I2C3_PCLK_EN()	(RCC->APB1ENR |= (1 << 23))

/*
 * Clock enable for USARTx
 */
#define USART1_PCLK_EN() (RCC->APB2ENR |= (1 << 4))
#define USART2_PCLK_EN() (RCC->APB1ENR |= (1 << 17))
#define USART3_PCLK_EN() (RCC->APB1ENR |= (1 << 18))
#define USART6_PCLK_EN() (RCC->APB2ENR |= (1 << 5))

/*
 * Clock enable for UARTx
 */
#define UART4_PCLK_EN()	(RCC->APB1ENR |= (1 << 19))
#define UART5_PCLK_EN()	(RCC->APB1ENR |= (1 << 20))

/*
 * Clock enable for SYSCFG
 */
#define SYSCFG_PCLK_EN() (RCC->APB2ENR |= (1 << 14))


/********************************PERIPHERAL CLOCK DISABLE******************************/
/*
 * Clock disable for GPIOx
 */
#define GPIOA_PCLK_DI() (RCC->AHB1ENR &= ~(1 << 0))
#define GPIOB_PCLK_DI() (RCC->AHB1ENR &= ~(1 << 1))
#define GPIOC_PCLK_DI() (RCC->AHB1ENR &= ~(1 << 2))
#define GPIOD_PCLK_DI() (RCC->AHB1ENR &= ~(1 << 3))
#define GPIOE_PCLK_DI() (RCC->AHB1ENR &= ~(1 << 4))
#define GPIOF_PCLK_DI() (RCC->AHB1ENR &= ~(1 << 5))
#define GPIOG_PCLK_DI() (RCC->AHB1ENR &= ~(1 << 6))
#define GPIOH_PCLK_DI() (RCC->AHB1ENR &= ~(1 << 7))
#define GPIOI_PCLK_DI() (RCC->AHB1ENR &= ~(1 << 8))
#define GPIOJ_PCLK_DI() (RCC->AHB1ENR &= ~(1 << 9))
#define GPIOK_PCLK_DI() (RCC->AHB1ENR &= ~(1 << 10))

/*
 * Clock disable for SPIx
 */
#define SPI1_PCLK_DI()	(RCC->APB2ENR &= ~(1 << 12))
#define SPI2_PCLK_DI()	(RCC->APB1ENR &= ~(1 << 14))
#define SPI3_PCLK_DI()	(RCC->APB1ENR &= ~(1 << 15))


/*
 * Clock disable for I2Cx
 */
#define I2C1_PCLK_DI()	(RCC->APB1ENR &= ~(1 << 21))
#define I2C2_PCLK_DI()	(RCC->APB1ENR &= ~(1 << 22))
#define I2C3_PCLK_DI()	(RCC->APB1ENR &= ~(1 << 23))

/*
 * Clock disable for USARTx
 */
#define USART1_PCLK_DI() (RCC->APB2ENR &= ~(1 << 4))
#define USART2_PCLK_DI() (RCC->APB1ENR &= ~(1 << 17))
#define USART3_PCLK_DI() (RCC->APB1ENR &= ~(1 << 18))
#define USART6_PCLK_DI() (RCC->APB2ENR &= ~(1 << 5))

/*
 * Clock disable for UARTx
 */
#define UART4_PCLK_DI()	(RCC->APB1ENR &= ~(1 << 19))
#define UART5_PCLK_DI()	(RCC->APB1ENR &= ~(1 << 20))

/*
 * Clock disable for SYSCFG
 */
#define SYSCFG_PCLK_DI() (RCC->APB2ENR &= ~(1 << 14))

/*
 * Reset GPIO peripheral registers
 */
#define GPIOA_PCLK_RST() do { (RCC->AHB1RSTR |= (1 << 0));  (RCC->AHB1RSTR &= ~(1 << 0)); } while (0)
#define GPIOB_PCLK_RST() do { (RCC->AHB1RSTR |= (1 << 1));  (RCC->AHB1RSTR &= ~(1 << 1)); } while (0)
#define GPIOC_PCLK_RST() do { (RCC->AHB1RSTR |= (1 << 2));  (RCC->AHB1RSTR &= ~(1 << 2)); } while (0)
#define GPIOD_PCLK_RST() do { (RCC->AHB1RSTR |= (1 << 3));  (RCC->AHB1RSTR &= ~(1 << 3)); } while (0)
#define GPIOE_PCLK_RST() do { (RCC->AHB1RSTR |= (1 << 4));  (RCC->AHB1RSTR &= ~(1 << 4)); } while (0)
#define GPIOF_PCLK_RST() do { (RCC->AHB1RSTR |= (1 << 5));  (RCC->AHB1RSTR &= ~(1 << 5)); } while (0)
#define GPIOG_PCLK_RST() do { (RCC->AHB1RSTR |= (1 << 6));  (RCC->AHB1RSTR &= ~(1 << 6)); } while (0)
#define GPIOH_PCLK_RST() do { (RCC->AHB1RSTR |= (1 << 7));  (RCC->AHB1RSTR &= ~(1 << 7)); } while (0)
#define GPIOI_PCLK_RST() do { (RCC->AHB1RSTR |= (1 << 8));  (RCC->AHB1RSTR &= ~(1 << 8)); } while (0)
#define GPIOJ_PCLK_RST() do { (RCC->AHB1RSTR |= (1 << 9));  (RCC->AHB1RSTR &= ~(1 << 9)); } while (0)
#define GPIOK_PCLK_RST() do { (RCC->AHB1RSTR |= (1 << 10)); (RCC->AHB1RSTR &= ~(1 << 10));} while (0)

/*
 * Reset SPI Peripheral Registers
 */
#define SPI1_PCLK_RST()  do { RCC->APB2RSTR |= (1 << 12); RCC->APB2RSTR &= ~(1 << 12); } while (0)
#define SPI2_PCLK_RST()  do { RCC->APB1RSTR |= (1 << 14); RCC->APB1RSTR &= ~(1 << 14); } while (0)
#define SPI3_PCLK_RST()  do { RCC->APB1RSTR |= (1 << 15); RCC->APB1RSTR &= ~(1 << 15); } while (0)

/*
 * Reset the I2C peripheral registers
 */
#define I2C1_PCLK_RST()	 do { RCC->APB1RSTR |= (1 << 21); RCC->APB1RSTR &= ~(1 << 21); } while(0)
#define I2C2_PCLK_RST()	 do { RCC->APB1RSTR |= (1 << 22); RCC->APB1RSTR &= ~(1 << 22); } while(0)
#define I2C3_PCLK_RST()	 do { RCC->APB1RSTR |= (1 << 23); RCC->APB1RSTR &= ~(1 << 23); } while(0)

/*
 * Reset for USART/UART peripheral registers
 */
#define USART1_PCLK_RST() do { RCC->APB2RSTR |= (1 << 4);  RCC->APB2RSTR &= ~(1 << 4);  } while (0)
#define USART2_PCLK_RST() do { RCC->APB1RSTR |= (1 << 17); RCC->APB1RSTR &= ~(1 << 17); } while (0)
#define USART3_PCLK_RST() do { RCC->APB1RSTR |= (1 << 18); RCC->APB1RSTR &= ~(1 << 18); } while (0)
#define UART4_PCLK_RST()  do { RCC->APB1RSTR |= (1 << 19); RCC->APB1RSTR &= ~(1 << 19); } while (0)
#define UART5_PCLK_RST()  do { RCC->APB1RSTR |= (1 << 20); RCC->APB1RSTR &= ~(1 << 20); } while (0)
#define USART6_PCLK_RST() do { RCC->APB2RSTR |= (1 << 5);  RCC->APB2RSTR &= ~(1 << 5);  } while (0)
/********************BIT DEFINITION OF SPI_PERIPHERALS*********************/

/*
 * SPI_CR1 Register Bit Macro
 */
#define SPI_CR1_CPHA					0U		//Clock Phase bit
#define SPI_CR1_CPOL					1U		//Clock Polarity bit
#define SPI_CR1_MSTR					2U		//Device Mode bit
#define SPI_CR1_BR						3U 		//Baud Rate Control bit
#define SPI_CR1_SPE						6U 		//SPI Enable
#define SPI_CR1_LSBFIRST				7U		//Frame format
#define SPI_CR1_SSI						8U		//Slave Select Information
#define SPI_CR1_SSM						9U		//Slave Select Management Bit
#define SPI_CR1_RXONLY					10U		//Receive only bit
#define SPI_CR1_DFF						11U		//Data Frame format bit
#define SPI_CR1_CRCNEXT					12U		//CRC Transfer Next
#define SPI_CR1_CRCEN		 			13U		//Hardware CRC Calculation Enable
#define SPI_CR1_BIDIOE					14U		//Output enable in bidirectional mode
#define SPI_CR1_BIDIMODE				15U		//Bidirectional Data Mode Enable

/*
 * SPI_CR2 Register Bit Macro
 */
#define SPI_CR2_RXDMAEN					0U		//Rx Buffer DMA Enable
#define SPI_CR2_TXDMAEN					1U		//Tx Buffer DMA Enable
#define SPI_CR2_SSOE					2U		//SS Output Enable
#define SPI_CR2_FRF						4U		//Frame Format
#define SPI_CR2_ERRIE					5U		//Error Interrupt Enable
#define SPI_CR2_RXNEIE					6U		//Rx buffer not empty interrupt enable
#define SPI_CR2_TXEIE					7U		//Tx buffer interrupt enable

/*
 * SPI_SR Register Bit Macro
 */
#define SPI_SR_RXNE						0U		//Receive buffer not empty
#define SPI_SR_TXE						1U		//Transmit buffer empty
#define SPI_SR_CHSIDE					2U		//Channel side
#define SPI_SR_UDR						3U		//Underrun flag
#define SPI_SR_CRCERR					4U		//CRC Error flag
#define SPI_SR_MODF						5U		//Mode fault
#define SPI_SR_OVR						6U		//Overrun flag
#define SPI_SR_BSY						7U		//Busy flag
#define SPI_SR_FRE						8U		//Frame format error

/*
 * SPI_DR Register Bit Macro
 */
#define SPI_DR							0U		//Transmit or Receive Data Starting bit [15:0}

/**********************************************************************************************/
/***********************************I2C REGISTER BIT MACROS*****************************/

/*
 * @I2C_CR1 bit macros
 */
#define I2C_CR1_SWRST				15U		//Software Reset
#define I2C_CR1_ALERT				13U		//SMBus Alert
#define I2C_CR1_PEC					12U		//Packet Error Checking
#define I2C_CR1_POS					11U		//Acknowledge/PEC position (for data reception)
#define I2C_CR1_ACK					10U		//Acknowledge enable
#define I2C_CR1_STOP				9U		//Stop generation
#define I2C_CR1_START				8U		//Start generation
#define I2C_CR1_NOSTRETCH			7U		//Clock stretching disable (Slave mode)
#define I2C_CR1_ENGC				6U		//General call enable
#define I2C_CR1_ENPEC				5U		//PEC enable
#define I2C_CR1_ENARP				4U		//ARP enable
#define I2C_CR1_SMBTYPE				3U		//SMBus Type
#define I2C_CR1_SMBUS				1U		//SMBus mode
#define I2C_CR1_PE					0U 		//Peripheral enable

/*
 * @I2C_CR2 bit macros
 */
#define I2C_CR2_LAST				12U		//DMA Last Transfer
#define I2C_CR2_DMAEN				11U		//DMA Request Enable
#define I2C_CR2_ITBUFEN				10U		//Buffer Interrupt Enable
#define I2C_CR2_ITEVTEN				9U		//Event Interrupt Enable
#define I2C_CR2_ITERREN				8U		//Error Interrupt Enable
#define I2C_CR2_FREQ				0U		//Peripheral Clock Frequency

/*
 * @I2C_SR1 bit macros
 */
#define I2C_SR1_SMBALERT			15U		//SMBus Alert
#define I2C_SR1_TIMEOUT				14U		//Timeout or Tlow Error
#define I2C_SR1_PECERR				12U		//PEC Error in Reception
#define I2C_SR1_OVR					11U		//Overrun/underrun
#define I2C_SR1_AF					10U		//Acknowledge Failure
#define I2C_SR1_ARLO				9U		//Arbitration Lost (Master Mode)
#define I2C_SR1_BERR				8U		//Bus Error
#define I2C_SR1_TXE					7U		//Data RegisterEmpty (transmitters)
#define I2C_SR1_RXNE				6U		//Data Register not empty (receivers)
#define I2C_SR1_STOPF				4U		//Stop Detection (Slave Mode)
#define I2C_SR1_ADD10				3U		//10-bit Header Sent (Master Mode)
#define I2C_SR1_BTF					2U		//Byte Transfer Finished
#define I2C_SR1_ADDR				1U		//Address Sent (Master Mode)/matched (slave mode)
#define I2C_SR1_SB					0U		//Start Bit(Master Mode)

/*
 * @I2C_SR2 bit macros
 */
#define I2C_SR2_PEC					8U		//Packet error checking register
#define I2C_SR2_DUALF				7U		//Dual flag(Slave mode)
#define I2C_SR2_SMBHOST				6U		//SMBus host header (Slave mode)
#define I2C_SR2_SMBDEFAULT			5U		//SMBus device default address(Slave mode)
#define I2C_SR2_GENCALL				4U		//General call address (Slave mode)
#define I2C_SR2_TRA					2U		//Transmitter/Receiver
#define I2C_SR2_BUSY				1U		//Bus busy
#define I2C_SR2_MSL					0U		//Master/Slave

/*
 * @I2C_CCR bit macros
 */
#define I2C_CCR_F_S					15U		//I2C master mode selection
#define I2C_CCR_DUTY				14U		//Fm mode duty cycle
#define I2C_CCR_CCR					0U		//Clock control register in Fm/Sm mode (Master Mode)
/**********************************************************************************************/
/**********************************BIT DEFINITION OF USART/UART PERIPHERAL*********************/
/*
 * Status Register USART_SR macros
 * Note: Bits 31:10 must be kept at reset value
 */
#define USART_SR_CTS		9U		//CST flag
#define USART_SR_LBD		8U		//LIN break detection flag
#define USART_SR_TXE		7U		//Transmit data register empty
#define USART_SR_TC			6U		//Transmission complete
#define USART_SR_RXNE		5U		//Read data register not empty
#define USART_SR_IDLE		4U		//IDLE line is detected
#define USART_SR_ORE		3U		//Overrun error
#define USART_SR_NF			2U		//Noise detection flag
#define USART_SR_FE			1U		//Framing error
#define USART_SR_PE			0U		//Parity error

/*
 * Baud rate register (USART_BRR)
 * Note: Bits 31:16 must be kept at reset value
 */
#define USART_BRR_DIV_FRACTION		0U 	//Mantissa of USARTDIV, [11:0]
#define USART_BRR_DIV_MANTISSA		4U	//Fraction of USARTDIV, [3:0]

/*
 * Control register 1 (USART_CR1)
 * Note: Bit 31:16 must be kept at reset value
 */
#define USART_CR1_OVER8		15U		//Over-sampling mode
//Bit 14 must be kept at reset value
#define USART_CR1_UE		13U		//USART enable
#define USART_CR1_M			12U		//Word length
#define USART_CR1_WAKE		11U		//Wake-up method
#define USART_CR1_PCE		10U		//Parity control enable
#define USART_CR1_PS		9U		//Parity selection
#define USART_CR1_PEIE		8U		//Interrupt enable
#define USART_CR1_TXEIE		7U		//TXE interrupt enable
#define USART_CR1_TCIE		6U		//Transmission complete interrupt enable
#define USART_CR1_RXNEIE	5U		//RXNE interrupt enable
#define USART_CR1_IDLEIE	4U		//IDLE interrupt enable
#define USART_CR1_TE		3U		//Transmitter enable
#define USART_CR1_RE		2U		//Receiver enable
#define USART_CR1_RWU		1U		//Receiver wake-up
#define USART_CR1_SBK		0U		//Send break

/*
 * Control Register 2 (USART_CR2)
 * Note: Bits 31:15 must be kept at reset value
 */
#define USART_CR2_LINEN		14U		//LIN mode enable
#define USART_CR2_STOP		12U		//STOP bits
#define USART_CR2_CLKEN		11U		//Clock enable
#define USART_CR2_CPOL		10U		//Clock polarity
#define USART_CR2_CPHA		9U		//Clock phase
#define USART_CR2_BCL		8U		//Last bit clock pulse
//Bit 7 is reserved, must be kept at reset value
#define USART_CR2_LBDIE		6U		//LIN break detection interrupt enable
#define USART_CR2_LBDL		5U		//LIN break detection length
//Bit 4 is reserved, must be kept at reset value
#define USART_CR2_ADD		0U		//Address of the USART node

/*
 * Control Register 3 (USART_CR3)
 * Note: Bits 31:12 are reserved, must be kept at reset value
 */
#define USART_CR3_ONEBIT	11U		//One sample bit method enable
#define USART_CR3_CTSIE		10U		//CTS interrupt enable
#define USART_CR3_CTSE		9U		//CTS enable
#define USART_CR3_RTSE		8U		//RTS enable
#define USART_CR3_DMAT		7U		//DMA enable transmitter
#define USART_CR3_DMAR		6U		//DMA enable receiver
#define USART_CR3_SCEN		5U		//Smartcard mode enable
#define USART_CR3_NACK		4U		//Smartcard NACK enable
#define USART_CR3_HDSEl		3U		//Half-duplex selection
#define USART_CR3_IRLP		2U		//IrDA low-power
#define USART_CR3_IREN		1U		//IrDA mode enable
#define USART_CR3_EIE		0U		//Error interrupt enable


#include "../Inc/gpio_driver.h"
#include "../Inc/STM32F407xx_SPI_Driver.h"
#include "../Inc/STM32F407xx_I2C_Driver.h"
#include "../Inc/STM32F407xx_USART_UART_Driver.h"
#endif /* INC_STM32F407XX_H_ */
