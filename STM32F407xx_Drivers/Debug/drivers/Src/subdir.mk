################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../drivers/Src/STM32F407xx_I2C_Driver.c \
../drivers/Src/STM32F407xx_SPI_Driver.c \
../drivers/Src/STM32F407xx_USART_UART_Driver.c \
../drivers/Src/gpio_driver.c 

OBJS += \
./drivers/Src/STM32F407xx_I2C_Driver.o \
./drivers/Src/STM32F407xx_SPI_Driver.o \
./drivers/Src/STM32F407xx_USART_UART_Driver.o \
./drivers/Src/gpio_driver.o 

C_DEPS += \
./drivers/Src/STM32F407xx_I2C_Driver.d \
./drivers/Src/STM32F407xx_SPI_Driver.d \
./drivers/Src/STM32F407xx_USART_UART_Driver.d \
./drivers/Src/gpio_driver.d 


# Each subdirectory must supply rules for building sources it contributes
drivers/Src/STM32F407xx_I2C_Driver.o: ../drivers/Src/STM32F407xx_I2C_Driver.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32 -DSTM32F407G_DISC1 -DSTM32F4 -DSTM32F407VGTx -DDEBUG -c -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"drivers/Src/STM32F407xx_I2C_Driver.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
drivers/Src/STM32F407xx_SPI_Driver.o: ../drivers/Src/STM32F407xx_SPI_Driver.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32 -DSTM32F407G_DISC1 -DSTM32F4 -DSTM32F407VGTx -DDEBUG -c -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"drivers/Src/STM32F407xx_SPI_Driver.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
drivers/Src/STM32F407xx_USART_UART_Driver.o: ../drivers/Src/STM32F407xx_USART_UART_Driver.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32 -DSTM32F407G_DISC1 -DSTM32F4 -DSTM32F407VGTx -DDEBUG -c -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"drivers/Src/STM32F407xx_USART_UART_Driver.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
drivers/Src/gpio_driver.o: ../drivers/Src/gpio_driver.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32 -DSTM32F407G_DISC1 -DSTM32F4 -DSTM32F407VGTx -DDEBUG -c -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"drivers/Src/gpio_driver.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

