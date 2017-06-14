################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/MPL3115A2.c \
../src/main.c \
../src/stm32l1xx_hal_msp.c \
../src/stm32l1xx_it.c 

OBJS += \
./src/MPL3115A2.o \
./src/main.o \
./src/stm32l1xx_hal_msp.o \
./src/stm32l1xx_it.o 

C_DEPS += \
./src/MPL3115A2.d \
./src/main.d \
./src/stm32l1xx_hal_msp.d \
./src/stm32l1xx_it.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -Wall -Wextra  -g -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32l162xx" -I"../system/include/stm32l1xx" -I"../system/include/cmsis/device" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


