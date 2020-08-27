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

/*
 * Some generic macros
 */
#define __vo volatile //making a shortcut for volatile
#define ENABLE 			1
#define DISABLE			0
#define SET				ENABLE
#define RESET			DISABLE

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
 * IRQ (Interrupt Request) Number of STM32F407xx MCU
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
 * ARM Cortex Mx Processor NVIC Interrupt Set-Enable Register (ISER) base address
 */
#define NVIC_ISER_BASEADDR	(__vo uint32_t*) 0xE000E100
#define NVIC_ISER(__INDEX__) *((NVIC_ISER_BASEADDR) + ((__INDEX__))) //Pointer arithmetic

/*
 * ARM Cortex Mx Processor NVIC Interrupt Clear-Enable Register (ICER) base address
 */
#define NVIC_ICER_BASEADDR	(__vo uint32_t*) 0xE000E180
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
	__vo uint32_t MEMRMP; 	//offset: 0x00
	__vo uint32_t PMC;		//offset: 0x04
	__vo uint32_t EXTICR[4];	//offset: 0x08 - 0x14
	uint32_t RESERVED[2];	//reserved: 0x18 - 0x1C
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
	__vo uint32_t PR; 	//offset: 0x14
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



#endif /* INC_STM32F407XX_H_ */
