################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/emodes.c \
../src/main.c 

OBJS += \
./src/emodes.o \
./src/main.o 

C_DEPS += \
./src/emodes.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/emodes.o: ../src/emodes.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m3 -mthumb '-DDEBUG_EFM=1' '-DEFM32GG990F1024=1' '-DDEBUG=1' -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/kits/EFM32GG_STK3700/examples/emode" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/kits/EFM32GG_STK3700/config" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/CMSIS/Include" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emlib/inc" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emdrv/common/inc" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emdrv/config" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emdrv/dmadrv/inc" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emdrv/gpiointerrupt/inc" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emdrv/nvm/inc" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emdrv/rtcdrv/inc" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emdrv/sleep/inc" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emdrv/spidrv/inc" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emdrv/uartdrv/inc" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emdrv/ustimer/inc" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/kits/common/bsp" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/kits/common/drivers" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/Device/SiliconLabs/EFM32GG/Include" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -std=c99 -MMD -MP -MF"src/emodes.d" -MT"src/emodes.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/main.o: ../src/main.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m3 -mthumb '-DDEBUG_EFM=1' '-DEFM32GG990F1024=1' '-DDEBUG=1' -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/kits/EFM32GG_STK3700/examples/emode" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/kits/EFM32GG_STK3700/config" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/CMSIS/Include" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emlib/inc" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emdrv/common/inc" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emdrv/config" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emdrv/dmadrv/inc" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emdrv/gpiointerrupt/inc" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emdrv/nvm/inc" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emdrv/rtcdrv/inc" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emdrv/sleep/inc" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emdrv/spidrv/inc" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emdrv/uartdrv/inc" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emdrv/ustimer/inc" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/kits/common/bsp" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/kits/common/drivers" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/Device/SiliconLabs/EFM32GG/Include" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -std=c99 -MMD -MP -MF"src/main.d" -MT"src/main.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


